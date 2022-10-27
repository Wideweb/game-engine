#include "PathfindingGraph.hpp"

#include "PathfinderAbstract.hpp"

#include <cmath>

namespace Engine {

PathfindingGraph::PathfindingGraph(
                        uint32_t layersNumber,
                        uint32_t clusterSize,
                        uint32_t scalingFactor,
                        uint32_t borderNodesIndent,
                        uint32_t level,
                        Map& map,
                        const MapObject& object,
                        std::shared_ptr<PathfinderConcrete> pathfinder,
                        Heuristic heuristic,
                        bool savePrecalculatedPaths) : map(map), object(object) {

    this->savePrecalculatedPaths = savePrecalculatedPaths;
    this->borderNodesIndent = borderNodesIndent;
    this->level = level;

    this->pathfinder = PathfinderConcreteAdapter(pathfinder);
    this->heuristic = heuristic;

    // Вычисляем размеры областей, чтобы вписаться в нужное количество слоев.
    if (layersNumber == 0) {
        layersNumber = DEFAULT_LAYERS_NUMBER;
        scalingFactor = DEFAULT_SCALING_FACTOR;

        uint32_t lowLevelScale = static_cast<uint32_t>(std::pow(DEFAULT_SCALING_FACTOR, DEFAULT_LAYERS_NUMBER));
        uint32_t lowLevelClusterSizeX = this->map.width / lowLevelScale;
        uint32_t lowLevelClusterSizeY = this->map.height / lowLevelScale;

        if (lowLevelClusterSizeX > MAX_CLUSTER_SIZE || lowLevelClusterSizeY > MAX_CLUSTER_SIZE) {
            clusterSize = MAX_CLUSTER_SIZE;
        }
        else {
            clusterSize = std::max(lowLevelClusterSizeX, lowLevelClusterSizeY);
        }

        this->borderNodesIndent = std::max(clusterSize / 2u, 1u);
    }

    for (uint32_t i = 0; i < layersNumber; i++) {
        if (i != 0) {
            // Увеличиваем размера области для более высоких уровней
            clusterSize = clusterSize * scalingFactor;
        }

        // Количество областей в горизонтальном и вертикальном направлении
        uint32_t clusterNumberX = static_cast<uint32_t>(std::ceil(static_cast<float>(this->map.width / clusterSize)));
        uint32_t clusterNumberY = static_cast<uint32_t>(std::ceil(static_cast<float>(this->map.height / clusterSize)));

        if (clusterNumberY <= 1 && clusterNumberX <= 1) {
            // На этом уровне будет только одна область. Следовательно, этот уровень бесполезен.
            break;
        }

        clusterNumberX = std::max(clusterNumberX, 1u);
        clusterNumberY = std::max(clusterNumberY, 1u);

        this->layers.push_back(this->buildLayer(i, clusterSize, clusterNumberY, clusterNumberX));
    }
}


void PathfindingGraph::addPortal(Point portal1, Point portal2) {
    addPortal(portal1, portal2, *this);
}


void PathfindingGraph::addPortal(Point portal1, Point portal2, PathfindingGraph& graph) {
    if (layers.size() != graph.layers.size()) {
        return;
    }

    std::shared_ptr<Node> portalNode1 = addPortal(portal1);
    std::shared_ptr<Node> portalNode2 = graph.addPortal(portal2);

    while (portalNode1 != nullptr && portalNode2 != nullptr) {
        const auto newEdge1 = std::make_shared<Edge>(portalNode1, portalNode2, Edge::Type::PORTAL, heuristic.D);
        portalNode1->edges.push_front(newEdge1);

        const auto newEdge2 = std::make_shared<Edge>(portalNode2, portalNode1, Edge::Type::PORTAL, heuristic.D);
        portalNode2->edges.push_front(newEdge2);

        portalNode1 = portalNode1->child;
        portalNode2 = portalNode2->child;
    }

    updateRegion(portal1, portal1);
    graph.updateRegion(portal2, portal2);
}


std::shared_ptr<Node> PathfindingGraph::addPortal(Point position) {
    std::shared_ptr<Node> node = nullptr;
    for (GraphLayer& layer: layers) {
        const auto& cluster = findCluster(position, layer);
        std::shared_ptr<Node>& newNode = cluster->portals[position];
        if (newNode == nullptr)
        {
            newNode = std::make_shared<Node>(position, this->level);
            newNode->child = node;
            cluster->portals.add(position, newNode);
        }
        node = newNode;
    }

    return node;
}

void PathfindingGraph::removePortal(Point portal1, Point portal2) {
    removePortal(portal1, portal2, *this);
}

void PathfindingGraph::removePortal(Point portal1, Point portal2, PathfindingGraph& graph) {
    for (int i = 0; i < layers.size(); i++) {
        const auto& cluster1 = findCluster(portal1, i);
        const auto& cluster2 = graph.findCluster(portal2, i);

        const auto portalNode1 = cluster1->portals[portal1];
        const auto portalNode2 = cluster2->portals[portal2];

        if (portalNode1 == nullptr || portalNode2 == nullptr) {
            return;
        }

        portalNode1->removeEdge(portal2, Edge::Type::PORTAL, graph.level);
        if (!portalNode1->hasEdge(Edge::Type::PORTAL)) {
            cluster1->portals.remove(portal1);
            tempNodes.push_back(portalNode1);
        }

        portalNode2->removeEdge(portal1, Edge::Type::PORTAL, level);
        if (!portalNode2->hasEdge(Edge::Type::PORTAL)) {
            cluster2->portals.remove(portal2);
            tempNodes.push_back(portalNode2);
        }
    }

    removeTempNodes();

    updateRegion(portal1, portal1);
    graph.updateRegion(portal2, portal2);
}

void PathfindingGraph::updateRegion(Point from, Point to) {
    // Определяем области, в которые новая позиция входит, и пересобираем их.
    for (const GraphLayer& layer: layers) {
        const auto& cluster = layer.clusters[0];

        uint32_t fromClusterX = from.x / cluster->boundaries.height();
        uint32_t fromClusterY = from.y / cluster->boundaries.width();

        uint32_t toClusterX = to.x / cluster->boundaries.width();
        uint32_t toClusterY = to.y / cluster->boundaries.height();

        for (uint32_t y = fromClusterY; y <= toClusterY; y++) {
            for (uint32_t x = fromClusterX; x <= toClusterX; x++) {
                rebuildCluster(layer, x, y);
            }
        }
    }

    tempNodes.clear();
}

std::shared_ptr<Node> PathfindingGraph::insertPathNode(Point position) {
    if (map.out(position)) {
        return nullptr;
    }

    std::shared_ptr<Node> node;
    for (const GraphLayer& layer: layers) {
        const auto& cluster = findCluster(position, layer);
        node = connectToCluster(layer.level, position, cluster, node);

    }
    return node;
}

std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> PathfindingGraph::insertPathNodes(Point start, Point dest) {
    if (map.out(start) || map.out(dest)) {
        return {nullptr, nullptr};
    }

    std::shared_ptr<Node> startNode = nullptr;
    std::shared_ptr<Node> destNode = nullptr;

    std::shared_ptr<Node> newStart;
    std::shared_ptr<Node> newDest;
    bool isConnected;

    for (const GraphLayer& layer: layers) {
        isConnected = false;

        const auto& startCluster = findCluster(start, layer);
        const auto& destCluster = findCluster(dest, layer);

        // Ищем область, которая содержит обе позиции
        if (startCluster == destCluster) {
            newStart = std::make_shared<Node>(start, this->level);
            newStart->child = startNode;
            tempNodes.push_back(newStart);

            newDest = std::make_shared<Node>(dest, this->level);
            newDest->child = destNode;
            tempNodes.push_back(newDest);


            isConnected = connectNodes(layer.level, newStart, newDest, startCluster);
            if (isConnected) {
                connectToPortals(layer.level, newStart, startCluster);
                connectToPortals(layer.level, newDest, destCluster);
                // Если они достижимы, мы устанавливаем их как узлы. В противном случае пытаемся связать их на
                // уровне выше.
                startNode = newStart;
                destNode = newDest;
            }
        }

        if (!isConnected) {
            startNode = connectToCluster(layer.level, start, startCluster, startNode);
            destNode = connectToCluster(layer.level, dest, destCluster, destNode);
        }
    }

    return {startNode, destNode};
}

void PathfindingGraph::removeTempNodes() {
    for (const auto& node: tempNodes) {
        unlinkNode(node);
    }
    tempNodes.clear();
}

void PathfindingGraph::unlinkNode(const std::shared_ptr<Node>& node) {
    std::vector<std::shared_ptr<Edge>> edgesToRemove;
    for (const auto& edge: node->edges) {
        edgesToRemove.clear();
        for (const auto& endNodeEdge: edge->end.lock()->edges) {
            if (endNodeEdge->end.lock() == node) {
                edgesToRemove.push_back(endNodeEdge);
            }
        }
        for (const auto& edgeToRemove: edgesToRemove) {
            const auto it = std::find(edge->end.lock()->edges.begin(), edge->end.lock()->edges.end(), edgeToRemove);
            edge->end.lock()->edges.erase(it);
        }
    }
}

const std::shared_ptr<Cluster>& PathfindingGraph::findCluster(Point point, uint32_t level) const {
    return findCluster(point, layers[level]);
}

const std::shared_ptr<Cluster>& PathfindingGraph::findCluster(Point point, const GraphLayer& layer) const {
    const auto& cluster = layer.clusters[0];

    const uint32_t clusterX = point.x / cluster->boundaries.width();
    const uint32_t clusterY = point.y / cluster->boundaries.height();

    return layer.clusters.at(clusterY * layer.width + clusterX);
}

std::shared_ptr<Node> PathfindingGraph::connectToCluster(uint32_t level, Point position, const std::shared_ptr<Cluster>& cluster, const std::shared_ptr<Node>& child) {
    const std::shared_ptr<Node> node = connectToBorder(level, position, cluster, child);
    connectToPortals(level, node, cluster);
    return node;
}

std::shared_ptr<Node> PathfindingGraph::connectToBorder(uint32_t level, Point position, const std::shared_ptr<Cluster>& cluster, const std::shared_ptr<Node>& child) {
    // Если позиция является граничный узлом области, возвращаем узел
    if (cluster->borders.hasKey(position)) {
        return cluster->borders.getValue(position);
    }

    // В противном случае создаем новый узел и находим путь через граничные узлы.
    const auto newNode = std::make_shared<Node>(position, this->level);
    newNode->child = child;

    for (const auto& clusterNode: cluster->borders.values()) {
        connectNodes(level, newNode, clusterNode, cluster);
    }

    // Поскольку этот узел не является частью графа, мы отслеживаем его, чтобы удалить позже.
    tempNodes.push_back(newNode);

    return newNode;
}

void PathfindingGraph::connectToPortals(uint32_t level, const std::shared_ptr<Node>& node, const std::shared_ptr<Cluster>& cluster) {
    for (const auto& portal: cluster->portals.values()) {
        connectNodes(level, node, portal, cluster);
    }
}

GraphLayer PathfindingGraph::buildLayer(uint32_t level, uint32_t clusterSize, uint32_t clusterNumberY, uint32_t clusterNumberX) {
    std::vector<std::shared_ptr<Cluster>> clusters;

    for (uint32_t y = 0; y < clusterNumberY; y++) {
        for (uint32_t x = 0; x < clusterNumberX; x++) {
            // Определяем границы области.
            const auto cluster = std::make_shared<Cluster>();
            cluster->boundaries.min.x = x * clusterSize;
            cluster->boundaries.min.y = y * clusterSize;
            cluster->boundaries.max.x = std::min(cluster->boundaries.min.x + clusterSize - 1, map.height - 1);
            cluster->boundaries.max.y = std::min(cluster->boundaries.min.y + clusterSize - 1, map.width - 1);

            // Абстрактные области хранят список дочерних областей.
            if (level > 0) {
                const auto& lowerLevelClusters = layers[level - 1].clusters;
                for (const auto& lowerLevelCluster : lowerLevelClusters) {
                    if (cluster->contains(lowerLevelCluster)) {
                        cluster->clusters.push_back(lowerLevelCluster);
                    }
                }
            }

            clusters.push_back(std::move(cluster));
        }
    }

    // Ищем соседние области и связываем их пограничными узлами.
    for (uint32_t y = 0; y < clusterNumberY; y++) {
        for (uint32_t x = 0; x < clusterNumberX; x++) {
            const auto& currentCluster = clusters[y * clusterNumberX + x];

            // Соседняя область снизу.
            if (y + 1 < clusterNumberY) {
                const auto& adjacent = clusters[(y + 1) * clusterNumberX + x];
                createBorderNodes(level, currentCluster, adjacent, false);
            }

            // Соседняя область справа.
            if (x + 1 < clusterNumberX) {
                const auto& adjacent = clusters[y * clusterNumberX + (x + 1)];
                createBorderNodes(level, currentCluster, adjacent, true);
            }
        }
    }

    // Связываем пограничные узлы области между собой.
    for (const auto& cluster: clusters) {
        generateIntraEdges(level, cluster);
    }

    return GraphLayer(level, clusterNumberY, clusterNumberX, clusters);
}

void PathfindingGraph::createBorderNodes(uint32_t level, const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal) {
    if (level == 0) {
        // Области первого уровня иерархии работают с ячейками карты.
        createConcreteBorderNodes(cluster1, cluster2, horizontal);
    } else {
        // Области более высоких уровней работают с областями уровня ниже.
        createAbstractBorderNodes(cluster1, cluster2, horizontal);
    }
}

void PathfindingGraph::createConcreteBorderNodes(const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal) {
    // Определяем границу-интервал, вдоль которой будем создавать пограничные узлы между двумя областями.
    int32_t iMin;
    int32_t iMax;
    if (horizontal) {
        iMin = cluster1->boundaries.min.y;
        iMax = cluster1->boundaries.max.y;
    } else {
        iMin = cluster1->boundaries.min.x;
        iMax = cluster1->boundaries.max.x;
    }

    uint32_t lineSize = 0;
    for (int32_t i = iMin; i <= iMax; ++i) {
        // Определяем две соседние ячейки карты двух областей.
        const int32_t cluster1BorderPointY = horizontal ? i : cluster1->boundaries.max.y;
        const int32_t cluster1BorderPointX = horizontal ? cluster1->boundaries.max.x : i;

        const int32_t cluster2BorderPointY = horizontal ? i : cluster2->boundaries.min.y;
        const int32_t cluster2BorderPointX = horizontal ? cluster2->boundaries.min.x : i;

        // Проверяем доступны ли ячейки для перемещения
        const bool isCluster1BorderPointAvailable = map.canPlace(object, Point(cluster1BorderPointX, cluster1BorderPointY));

        const bool isCluster2BorderPointAvailable = map.canPlace(object, Point(cluster2BorderPointX, cluster2BorderPointY));

        // Если наткнулись на препятствие, пытаемся добавить узел в пройденный интервал.
        if ((!isCluster1BorderPointAvailable || !isCluster2BorderPointAvailable)) {
            createConcreteInterEdges(cluster1, cluster2, horizontal, lineSize, i - 1);
            lineSize = 0;
            continue;
        }

        // Если ширина свободного от препятствий интервала на границе достаточно велика.
        if (lineSize > borderNodesIndent) {
            createConcreteInterEdges(cluster1, cluster2, horizontal, lineSize, i);
            lineSize = 0;
            continue;
        }

        // Определяем ширину свободного от препятствий интервала на границе.
        lineSize++;
    }

    // Если размер строки > 0 после цикла, то у нас есть еще одна строка для заполнения.
    createConcreteInterEdges(cluster1, cluster2, horizontal, lineSize, iMax);
}

void PathfindingGraph::createConcreteInterEdges(const std::shared_ptr<Cluster>& cluster1,
                                                const std::shared_ptr<Cluster>& cluster2,
                                                bool horizontal,
                                                uint32_t lineSize,
                                                uint32_t borderNodeIndex) {
    if (lineSize == 0) {
        return;
    }

    createConcreteInterEdge(cluster1, cluster2, horizontal, borderNodeIndex - (lineSize / 2));
}

void PathfindingGraph::createConcreteInterEdge(const std::shared_ptr<Cluster>& cluster1,
                                               const std::shared_ptr<Cluster>& cluster2,
                                               bool horizontal,
                                               uint32_t borderNodeIndex) {
    // Определяем позиции соседних ячеек на границе двух областей.
    Point cluster1BoundaryPoint;
    Point cluster2BoundaryPoint;

    if (horizontal) {
        cluster1BoundaryPoint = Point(cluster1->boundaries.max.x, borderNodeIndex);
        cluster2BoundaryPoint = Point(cluster2->boundaries.min.x, borderNodeIndex);
    } else {
        cluster1BoundaryPoint = Point(borderNodeIndex, cluster1->boundaries.max.y);
        cluster2BoundaryPoint = Point(borderNodeIndex, cluster2->boundaries.min.y);
    }

    // Создаем новый узел в области, если для данной позиции в области его еще нет.
    std::shared_ptr<Node> cluster1BoundaryNode;
    if (cluster1->borders.hasKey(cluster1BoundaryPoint)) {
        cluster1BoundaryNode = cluster1->borders.getValue(cluster1BoundaryPoint);
    }

    if (cluster1BoundaryNode == nullptr) {
        cluster1BoundaryNode = std::make_shared<Node>(cluster1BoundaryPoint, this->level);
        cluster1->borders.add(cluster1BoundaryPoint, cluster1BoundaryNode);
        cluster1BoundaryNode->child = nullptr;
    } else if (cluster1BoundaryNode->state == Node::State::REPLACE) {
        cluster1BoundaryNode->edges.clear();
        cluster1BoundaryNode->state = Node::State::NEW;
    } else if (cluster1BoundaryNode->state == Node::State::CHECK) {
        cluster1BoundaryNode->state = Node::State::LINKED;
    }

    std::shared_ptr<Node> cluster2BoundaryNode;
    if (cluster2->borders.hasKey(cluster2BoundaryPoint)) {
        cluster2BoundaryNode = cluster2->borders.getValue(cluster2BoundaryPoint);
    }

    if (cluster2BoundaryNode == nullptr) {
        cluster2BoundaryNode = std::make_shared<Node>(cluster2BoundaryPoint, this->level);
        cluster2->borders.add(cluster2BoundaryPoint, cluster2BoundaryNode);
        cluster2BoundaryNode->child = nullptr;
    } else if (cluster2BoundaryNode->state == Node::State::REPLACE) {
        cluster2BoundaryNode = cluster2->borders.getValue(cluster2BoundaryPoint);
        cluster2BoundaryNode->edges.clear();
        cluster2BoundaryNode->state = Node::State::NEW;
    } else if (cluster2BoundaryNode->state == Node::State::CHECK) {
        cluster2BoundaryNode->state = Node::State::LINKED;
    }

    // Создаем новую двустороннюю связь между соседними узлами двух областей.
    const auto edgeForward = std::make_shared<Edge>(cluster1BoundaryNode, cluster2BoundaryNode, Edge::Type::INTER, heuristic.D);
    const auto edgeBackward = std::make_shared<Edge>(cluster2BoundaryNode, cluster1BoundaryNode, Edge::Type::INTER, heuristic.D);

    cluster1BoundaryNode->edges.push_back(edgeForward);
    cluster2BoundaryNode->edges.push_back(edgeBackward);
}

void PathfindingGraph::createAbstractBorderNodes(const std::shared_ptr<Cluster>& cluster1,
                                                 const std::shared_ptr<Cluster>& cluster2,
                                                 bool horizontal) {
    for (const auto& cluster1Child: cluster1->clusters) {
        for (const auto& cluster2Child: cluster2->clusters) {
            if (checkAdjacent(cluster1Child, cluster2Child, horizontal)) {
                createAbstractInterEdges(cluster1, cluster2, cluster1Child, cluster2Child, horizontal);
            }
        }
    }
}

bool PathfindingGraph::checkAdjacent(const std::shared_ptr<Cluster>& cluster1,
                                     const std::shared_ptr<Cluster>& cluster2, 
                                     bool horizontal) const {
    if (horizontal) {
        return cluster1->boundaries.min.y == cluster2->boundaries.min.y && cluster1->boundaries.max.x + 1 == cluster2->boundaries.min.x;
    }
    return cluster1->boundaries.min.x == cluster2->boundaries.min.x && cluster1->boundaries.max.y + 1 == cluster2->boundaries.min.y;
}

void PathfindingGraph::createAbstractInterEdges(const std::shared_ptr<Cluster>& cluster1, 
                                                const std::shared_ptr<Cluster>& cluster2,
                                                const std::shared_ptr<Cluster>& cluster1Child,
                                                const std::shared_ptr<Cluster>& cluster2Child,
                                                bool horizontal) {
    Point neighbourPos;

    for (const auto& childNode1: cluster1Child->borders) {
        neighbourPos.x = childNode1->position.x + (horizontal ? 1 : 0);
        neighbourPos.y = childNode1->position.y + (horizontal ? 0 : 1);

        if (!cluster2Child->contains(neighbourPos)) {
            continue;
        }

        if (!cluster2Child->borders.hasKey(neighbourPos)) {
            continue;
        }
        const auto& childNode2 = cluster2Child->borders.getValue(neighbourPos);

        std::shared_ptr<Node> node1;
        if (cluster1->borders.hasKey(childNode1->position)) {
            node1 = cluster1->borders[childNode1->position];
        }

        if (node1 == nullptr) {
            node1 = std::make_shared<Node>(childNode1->position, this->level);
            node1->child = childNode1;
            cluster1->borders.add(node1->position, node1);
        } else if (node1->state == Node::State::REPLACE) {
            node1->edges.clear();
            node1->child = childNode1;
            node1->state = Node::State::NEW;
        } else if (node1->state == Node::State::CHECK) {
            node1->state = Node::State::LINKED;
        }

        std::shared_ptr<Node> node2;
        if (cluster2->borders.hasKey(childNode2->position)) {
            node2 = cluster2->borders[childNode2->position];
        }

        if (node2 == nullptr) {
            node2 = std::make_shared<Node>(childNode2->position, this->level);
            node2->child = childNode2;
            cluster2->borders.add(node2->position, node2);
        } else if (node2->state == Node::State::REPLACE) {
            node2->edges.clear();
            node2->child = childNode2;
            node2->state = Node::State::NEW;
        } else if (node2->state == Node::State::CHECK) {
            node2->state = Node::State::LINKED;
        }

        const auto newEdge1 = std::make_shared<Edge>(node1, node2, Edge::Type::INTER, heuristic.D);
        const auto newEdge2 = std::make_shared<Edge>(node2, node1, Edge::Type::INTER, heuristic.D);

        node1->edges.push_back(newEdge1);
        node2->edges.push_back(newEdge2);
    }
}

void PathfindingGraph::generateIntraEdges(uint32_t level, const std::shared_ptr<Cluster>& cluster) {
    for (uint32_t i = 0; i < cluster->portals.size(); i++) {
        const auto& portal1 = cluster->portals.values()[i];
        for (uint32_t j = i + 1; j < cluster->portals.size(); j++) {
            auto& portal2 = cluster->portals.values()[j];
            if (portal1->state == Node::State::NEW || portal2->state == Node::State::NEW) {
                connectNodes(level, portal1, portal2, cluster);
            }
        }
    }
    for (uint32_t i = 0; i < cluster->borders.size(); i++) {
        const auto& node1 = cluster->borders.values()[i];
        for (uint32_t j = i + 1; j < cluster->borders.size(); j++) {
            const auto& node2 = cluster->borders.values()[j];
            if (node1->state == Node::State::NEW || node2->state == Node::State::NEW) {
                connectNodes(level, node1, node2, cluster);
            }
        }

        for (const auto& portal: cluster->portals.values()) {
            if (node1->state == Node::State::NEW || portal->state == Node::State::NEW) {
                connectNodes(level, node1, portal, cluster);
            }
        }

        node1->state = Node::State::LINKED;
    }

    for (const auto& portal: cluster->portals) {
        portal->state = Node::State::LINKED;
    }
}

bool PathfindingGraph::connectNodes(uint32_t level, const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end, const std::shared_ptr<Cluster>& cluster) {
    if (level == 0) {
        // Области первого уровня иерархии работают с ячейками карты.
        return connectConcreteNodes(start, end, cluster);
    } else {
        // Области более высоких уровней работают с областями уровня ниже.
        return connectAbstractNodes(start, end, cluster);
    }
}

bool PathfindingGraph::connectConcreteNodes(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end, const std::shared_ptr<Cluster>& cluster) {
    const auto edge = start->findEdge(end);
    if (edge != nullptr && edge->type == Edge::Type::PORTAL) {
        return true;
    }

    const auto path = pathfinder.findPath(start->position, end->position, map, object, heuristic, nullptr, false, cluster->boundaries);
    if (path.empty()) {
        return false;
    }

    const auto edge1 = std::make_shared<Edge>(start, end, Edge::Type::INTER, 0);
    if (savePrecalculatedPaths) {
        edge1->underlyingMapPath = path;
    }

    const auto edge2 = std::make_shared<Edge>(end, start, Edge::Type::INTER, 0);
    if (savePrecalculatedPaths) {
        edge2->underlyingMapPath.reserve(path.size());
    }

    // Сохраняем обратный путь в узле конца пути. Одновременно суммируем веса базовых ребер.
    uint32_t weight = 0;
    for (uint32_t i = path.size() - 1; i > 0; i--) {
        Point current = path[i];
        Point next = path[i - 1];

        const Direction direction = Direction::detect(current, next);
        weight += direction.alongAxes() ? heuristic.D : heuristic.D2;

        if (savePrecalculatedPaths) {
            edge2->underlyingMapPath.push_back(current);
        }
    }
    if (savePrecalculatedPaths) {
        edge2->underlyingMapPath.push_back(start->position);
    }

    edge1->weight = weight;
    edge2->weight = weight;

    start->edges.push_back(edge1);
    end->edges.push_back(edge2);

    return true;
}

bool PathfindingGraph::connectAbstractNodes(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end, const std::shared_ptr<Cluster>& cluster) {
    const auto edge = start->findEdge(end);
    if (edge != nullptr && edge->type == Edge::Type::PORTAL) {
        return true;
    }

    const auto path = PathfinderAbstract::findPath(start->child, end->child, heuristic, cluster->boundaries);
    if (path.empty()) {
        return false;
    }

    const auto edge1 = std::make_shared<Edge>(start, end, Edge::Type::INTRA, 0);
    edge1->underlyingPath = path;

    const auto edge2 = std::make_shared<Edge>(end, start, Edge::Type::INTRA, 0);
    edge2->underlyingPath.reserve(path.size());

    // Сохраняем обратный путь в узле конца пути. Одновременно суммируем веса базовых ребер.
    uint32_t weight = 0;
    for (uint32_t i = edge1->underlyingPath.size() - 1; i >= 0; i--) {
        const auto& transition = edge1->underlyingPath[i];
        const auto& pathNode = transition->end.lock();
        for (const auto& pathNodeEdge: pathNode->edges) {
            if (pathNodeEdge->start.lock() == transition->end.lock() && pathNodeEdge->end.lock() == transition->start.lock()) {
                edge2->underlyingPath.push_back(pathNodeEdge);
                weight += pathNodeEdge->weight;
                break;
            }
        }
    }

    edge1->weight = weight;
    edge2->weight = weight;

    start->edges.push_back(edge1);
    end->edges.push_back(edge2);

    return true;
}

void PathfindingGraph::rebuildCluster(const GraphLayer& layer, uint32_t clusterPosX, uint32_t clusterPosY) {
    const auto& cluster = layer.clusters[clusterPosY * layer.width + clusterPosX];

    if (layer.level > 0) {
        // Если дочерний узел абстрактного узла был удален, то его нужно тоже удалить.
        removeEmptyAbstractBorderNodes(cluster);
    }

    // Все узлы области должны быть определены заново.
    for (const auto& node: cluster->borders) {
        node->state = Node::State::REPLACE;
    }

    // Если в пределах области есть портал, то его связи с узлами области должны быть удалены. Далее они будут
    // заново определены.
    for (const auto& portal: cluster->portals) {
        portal->state = Node::State::NEW;
        while (portal->edges.size() > 0 && portal->edges[portal->edges.size() - 1]->type != Edge::Type::PORTAL) {
            portal->edges.pop_back();
        }
    }

    for (const Direction direction : CLUSTER_DIRECTIONS) {
        int32_t x = clusterPosX + direction.x;
        int32_t y = clusterPosY + direction.y;
        if (x < 0 || x >= layer.width || y < 0 || y >= layer.height) {
            continue;
        }

        const auto& adjacent = layer.clusters[(y) * layer.width + x];
        if (layer.level > 0) {
            removeEmptyAbstractBorderNodes(adjacent);
        }

        auto first = cluster;
        auto second = adjacent;
        if (direction != Direction::DOWN && direction != Direction::RIGHT) {
            first = adjacent;
            second = cluster;
        }

        // Помечаем соседние узлы как CHECK (нужно проверить).
        prepareBorderNodesForCheck(first, second, direction.x != 0);
        // Если позиция пограничного узла не поменялось, то:
        // Узел соседней области: CHECK   -> LINKED.
        // Узел текущей области:  REPLACE -> NEW.
        createBorderNodes(layer.level, first, second, direction.x != 0);

        // Удаляем узлы, которые остались в CHECK.
        removeInvalidBorderNodes(adjacent);
        // Генерируем новые связи внутри соседней области только с теми узлами, которые сместились (статус NEW).
        generateIntraEdges(layer.level, adjacent);
    }

    // Удаляем узлы, которые остались в REPLACE.
    removeInvalidBorderNodes(cluster);
    // Генерируем новые связи внутри текущей области только с теми узлами, которые сместились (статус NEW).
    generateIntraEdges(layer.level, cluster);
}

void PathfindingGraph::prepareBorderNodesForCheck(const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal) {
    Point position = Point(0, 0);

    for (const auto& node1: cluster1->borders.values()) {
        position.x = node1->position.x + (horizontal ? 1 : 0);
        position.y = node1->position.y + (horizontal ? 0 : 1);

        
        if (!cluster2->borders.hasKey(position)) {
            continue;
        }
        const auto& node2 = cluster2->borders.getValue(position);

        node1->removeEdge(node2->position, Edge::Type::INTER);
        node2->removeEdge(node1->position, Edge::Type::INTER);

        if (node1->state == Node::State::LINKED && !node1->hasEdge(Edge::Type::INTER)) {
            node1->state = Node::State::CHECK;
        }

        if (node2->state == Node::State::LINKED && !node2->hasEdge(Edge::Type::INTER)) {
            node2->state = Node::State::CHECK;
        }
    }
}

void PathfindingGraph::removeInvalidBorderNodes(const std::shared_ptr<Cluster>& cluster) {
    std::vector<std::shared_ptr<Node>> removedNodes;

    for (const auto& node: cluster->borders.values()) {
        if (node->state == Node::State::CHECK || node->state == Node::State::REPLACE) {
            removedNodes.push_back(node);
        }
    }

    // Не удаляем узлы сразу, так как на них могут ссылаться узлы более высокого уровня.
    // Если мы вернем узел в пул сразу, то он может быть заново востребован до того как мы успеем проверить
    // абстрактный узел, который на него ссылается.
    for (const auto& node: removedNodes) {
        cluster->borders.remove(node->position);
        unlinkNode(node);
        node->state = Node::State::REMOVED;
        tempNodes.push_back(node);
    }
}

void PathfindingGraph::removeEmptyAbstractBorderNodes(const std::shared_ptr<Cluster>& cluster) {
    std::vector<std::shared_ptr<Node>> removedNodes;

    for (const auto& node: cluster->borders.values()) {
        if (node->child->state == Node::State::REMOVED) {
            removedNodes.push_back(node);
        }
    }

    for (const auto& node: removedNodes) {
        cluster->borders.remove(node->position);
        unlinkNode(node);
        node->state = Node::State::REMOVED;
        tempNodes.push_back(node);
    }
}

} // namespace Engine