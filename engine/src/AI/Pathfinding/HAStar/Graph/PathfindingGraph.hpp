#pragma once

#include "GraphLayer.hpp"
#include "Node.hpp"
#include "Cluster.hpp"
#include "Edge.hpp"
#include "Map.hpp"
#include "MapObject.hpp"
#include "Point.hpp"
#include "Direction.hpp"
#include "Heuristic.hpp"
#include "PathfinderConcrete.hpp"
#include "PathfinderConcreteAdapter.hpp"

#include <vector>
#include <utility>
#include <memory>
#include <array>

namespace Engine {

/**
 * Граф поиска пути строится с использованием информации, извлеченной из карты.
 *
 * Разбивает игровую карту на набор смежных областей, соединенных входами. Каждая такая область может объединять как
 * ячейки карты, так и другие области на уровне ниже по иерархии.
 */
class PathfindingGraph {
  public:
    /**
     * Уровень.
     */
    uint32_t level;

    /**
     * Уровни иерархии графа.
     */
    std::vector<GraphLayer> layers;

    /**
     * Карта.
     */
    Map& map;

    /**
     * Объект, который перемещается по карте.
     */
    MapObject object;

    /**
     * Алгоритм поиска пути по ячейкам игровой карты.
     */
    PathfinderConcreteAdapter pathfinder;

    /**
     * Функция эвристической оценки расстояния.
     */
    Heuristic heuristic = Heuristic::MANHATTAN;

    /**
     * Если true, пути между узлами графа будут сохранены и использованы при расчете пути.
     */
    bool savePrecalculatedPaths;

    /**
     * Влияет на количество пограничных узлов областей.
     */
    uint32_t borderNodesIndent;

    /**
     * Отслеживаем добавленные в граф узлы, чтобы потом удалить их.
     */
    std::vector<std::shared_ptr<Node>> tempNodes;

    /**
     * Направления, по которым области рассматриваются как соседние.
     */
    std::array<Direction, 4> CLUSTER_DIRECTIONS = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };

    /**
     * Количество уровней иерархии в графе по умолчанию.
     */
    uint32_t DEFAULT_LAYERS_NUMBER = 2;

    /**
     * Коэффициент увеличения размера областей для более высоких уровней иерархии по умолчанию.
     */
    uint32_t DEFAULT_SCALING_FACTOR = 3;

    /**
     * Максимальный размер одной области.
     */
    uint32_t MAX_CLUSTER_SIZE = 20;

    /**
     * Создает граф поиска пути для карты.
     * @param layersNumber Количество уровень иерархии в графе.
     * @param clusterSize Размер области на первом уровне иерархии.
     * @param scalingFactor Коэффициент увеличения размера областей для более высоких уровней иерархии.
     * @param borderNodesIndent Влияет на количество пограничных узлов областей.
     * @param level Уровень.
     * @param map Карта.
     * @param object Объект для которого рассчитывается граф.
     * @param pathfinder Используется для поиска пути по ячейкам игровой карты.
     * @param heuristic Функция эвристической оценки расстояния.
     * @param savePrecalculatedPaths Если true, пути между узлами графа будут сохранены и использованы при расчете пути.
     */
    PathfindingGraph(uint32_t layersNumber,
                        uint32_t clusterSize,
                        uint32_t scalingFactor,
                        uint32_t borderNodesIndent,
                        uint32_t level,
                        Map& map,
                        const MapObject& object,
                        std::shared_ptr<PathfinderConcrete> pathfinder,
                        Heuristic heuristic,
                        bool savePrecalculatedPaths);

    /**
     * Добавляет прямую двустороннюю связь между двумя ячейками карты.
     * @param portal1 Позиция первого портала.
     * @param portal2 Позиция второго портала.
     */
    void addPortal(Point portal1, Point portal2);

    /**
     * Добавляет прямую двустороннюю связь между двумя графами.
     * @param portal1 Позиция портала в первом графе.
     * @param portal2 Позиция портала во втором графе.
     * @param graph Граф связь с которым будет установлена.
     */
    void addPortal(Point portal1, Point portal2, PathfindingGraph& graph);

    /**
     * Удаляет связь между двумя ячейками карты.
     * @param portal1 Позиция первого портала.
     * @param portal2 Позиция второго портала.
     */
    void removePortal(Point portal1, Point portal2);

    /**
     * Удаляет связь между двумя графами.
     * @param portal1 Позиция портала в первом графе.
     * @param portal2 Позиция портала во втором графе.
     * @param graph Граф связь с которым будет удалена.
     */
    void removePortal(Point portal1, Point portal2, PathfindingGraph& graph);

    /**
     * Обновить состояние региона на карте.
     */
    void updateRegion(Point from, Point to);

    /**
     * Добавляет в граф временный узел начала или конца искомого пути.
     * @param position Координаты начального узла на карте.
     * @return Ссылка на узел в графе, если он может быть создан для переданной координаты карты, иначе - null.
     */
    std::shared_ptr<Node> insertPathNode(Point position);

    /**
     * Добавляет в граф временные узлы начала и конца искомого пути.
     * @param start Координаты начального узла на карте.
     * @param dest Координаты конечного узла на карте.
     * @return Ссылки на начальный и конечный узлы, если они могут быть созданы для переданных координат, иначе - null.
     */
    std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> insertPathNodes(Point start, Point dest);

    /**
     * Удаляет временные узлы из графа.
     */
    void removeTempNodes();

    /**
     * На указанном уровне иерархии графа ищет абстрактную область, в которую попадает ячейка карты.
     * @param point Ячейка карты.
     * @param level Уровень иерархии графа.
     * @return Область.
     */
    const std::shared_ptr<Cluster>& findCluster(Point point, uint32_t level) const;

  private:
    /**
     * Добавляет портал для ячейки на карте во все области графа, в которые она входит.
     * @param position Позиция портала.
     * @return Узел
     */
    std::shared_ptr<Node> addPortal(Point position);

    /**
     * На указанном уровне иерархии графа ищет абстрактную область, в которую попадает ячейка карты.
     * @param point Ячейка карты.
     * @param layer Уровень иерархии графа.
     * @return Область.
     */
    const std::shared_ptr<Cluster>& findCluster(Point point, const GraphLayer& layer) const;

    /**
     * Соединяет ячейку карты с узлами области.
     * @param level Уровень иерархии.
     * @param position Ячейка карты.
     * @param cluster Область.
     * @param child Узел графа, представляющий ячейку карты, на дочернем для переданной области уровне иерархии.
     * @return Новый узел.
     */
    std::shared_ptr<Node> connectToCluster(uint32_t level, Point position, const std::shared_ptr<Cluster>& cluster, const std::shared_ptr<Node>& child);

    /**
     * Соединяет ячейку карты с границей области, создает новый узел.
     * @param level Уровень иерархии.
     * @param position Ячейка карты.
     * @param cluster Область.
     * @param child Узел графа, представляющий ячейку карты, на дочернем для переданной области уровне иерархии.
     * @return Новый узел.
     */
    std::shared_ptr<Node> connectToBorder(uint32_t level, Point position, const std::shared_ptr<Cluster>& cluster, const std::shared_ptr<Node>& child);

    /**
     * Соединяет узел с порталами области.
     * @param level Уровень иерархии.
     * @param node узел.
     * @param cluster Область.
     */
    void connectToPortals(uint32_t level, const std::shared_ptr<Node>& node, const std::shared_ptr<Cluster>& cluster);

    /**
     * Создает области для определенного уровня иерархии графа и связывает их пограничными узлами.
     * @param level Уровень иерархии.
     * @param clusterSize Размер одной области.
     * @param clusterNumberI количество областей по горизонтали.
     * @param clusterNumberJ количество областей по вертикали.
     * @return Список областей.
     */
    GraphLayer buildLayer(uint32_t level, uint32_t clusterSize, uint32_t clusterNumberI, uint32_t clusterNumberJ);

    /**
     * Создает граничные узлы для соседних областей и соединяет их вместе.
     * @param level Уровень иерархии.
     * @param cluster1 Первая область.
     * @param cluster2 Вторая область.
     * @param horizontal Если true, области рассматриваются как соседи по горизонтали, иначе по вертикали.
     */
    void createBorderNodes(uint32_t level, const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal);

    /**
     * Создает пограничные узлы для соседних областей и соединяет их вместе.
     * @param cluster1 Первая область.
     * @param cluster2 Вторая область.
     * @param horizontal Если true, области рассматриваются как соседи по горизонтали, иначе по вертикали.
     */
    void createConcreteBorderNodes(const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal);

    /**
     * Создает ребра связывающие две области вдоль границы.
     * @param cluster1 Первая область.
     * @param cluster2 Вторая область.
     * @param horizontal Если true, области рассматриваются как соседи по горизонтали, иначе по вертикали.
     * @param lineSize Ширину свободного от препятствий интервала.
     * @param borderNodeIndex Индекс, на котором мы остановились (либо это препятствие, либо конец области).
     */
    void createConcreteInterEdges(const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal,
                                          uint32_t lineSize, uint32_t borderNodeIndex);

    /**
     * Создает ребра связывающие две области вдоль границы.
     * @param cluster1 Первая область.
     * @param cluster2 Вторая область.
     * @param horizontal Если true, области рассматриваются как соседи по горизонтали, иначе по вертикали.
     * @param borderNodeIndex Индекс пограничных узлов двух областей.
     */
    void createConcreteInterEdge(const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal,
                                         uint32_t borderNodeIndex);

    /**
     * Создает ребра связывающие две абстрактные области (области уровня 1 и больше) вдоль границы.
     * @param cluster1 Первая область.
     * @param cluster2 Вторая область.
     * @param horizontal Если true, области рассматриваются как соседи по горизонтали, иначе по вертикали.
     */
    void createAbstractBorderNodes(const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal);

    /**
     * Проверяет области на смежность.
     * @param cluster1 Первая область.
     * @param cluster2 Вторая область.
     * @param horizontal Если true, проверяет области на смежность, иначе по вертикали.
     * @return Возвращает true если области являются соседями, иначе - false.
     */
    bool checkAdjacent(const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal) const;

    /**
     *  Находит связи между пограничными дочерними областями, которые пересекают границу между родительскими областями.
     * @param cluster1 Первая родительская область.
     * @param cluster2 Вторая родительская область, которая примыкает к первой области.
     * @param cluster1Child Дочерняя область первой области.
     * @param cluster2Child Дочерняя область второй области.
     * @param horizontal Если true, проверяет области на смежность, иначе по вертикали.
     */
    void createAbstractInterEdges(const std::shared_ptr<Cluster>& cluster1,
                                  const std::shared_ptr<Cluster>& cluster2,
                                  const std::shared_ptr<Cluster>& cluster1Child,
                                  const std::shared_ptr<Cluster>& cluster2Child,
                                  bool horizontal);

    /**
     * Добавляет внутренние ребра для каждого пограничного узла области и находит путь между ними.
     * @param level Уровень иерархии.
     * @param cluster Область.
     */
    void generateIntraEdges(uint32_t level, const std::shared_ptr<Cluster>& cluster);

    /**
     * Соединяет два узла путем поиска пути между ними.
     * @param level Уровень иерархии.
     * @param start Узел начала пути.
     * @param end Узел конца пути.
     * @param cluster Область, внутри которой производится поиск пути.
     * @return Возвращает true если путь найден, иначе - false.
     */
    bool connectNodes(uint32_t level, const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end, const std::shared_ptr<Cluster>& cluster);

    /**
     * Соединяет два узла путем поиска пути между ними.
     * @param start Узел начала пути.
     * @param end Узел конца пути.
     * @param cluster Область, внутри которой производится поиск пути.
     * @return Возвращает true если путь найден, иначе - false.
     */
    bool connectConcreteNodes(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end, const std::shared_ptr<Cluster>& cluster);

    /**
     * Соединяет два узла путем поиска пути между ними.
     * @param start Узел начала пути.
     * @param end Узел конца пути.
     * @param cluster Область, внутри которой производится поиск пути.
     * @return Возвращает true если путь найден, иначе - false.
     */
    bool connectAbstractNodes(const std::shared_ptr<Node>& start, const std::shared_ptr<Node>& end, const std::shared_ptr<Cluster>& cluster);

    /**
     * Заново определяет пограничные узлы с соседними областями и связи между ними.
     * @param layer Уровень иерархии графа.
     * @param clusterPosI i-ая позиция области на текущем уровне графа.
     * @param clusterPosJ j-ая позиция области на текущем уровне графа.
     */
    void rebuildCluster(const GraphLayer& layer, uint32_t clusterPosX, uint32_t clusterPosY);

    /**
     * Удаляем связь между пограничными узлами двух областей и, если у узлов нет связей с другими областями, помечаем
     их как CHECK (нужно проверить).
     * @param cluster1 Первая область.
     * @param cluster2 Вторая область (соседняя с первой).
     * @param horizontal Если true, области рассматриваются как соседи по горизонтали, иначе по вертикали.
     */
    void prepareBorderNodesForCheck(const std::shared_ptr<Cluster>& cluster1, const std::shared_ptr<Cluster>& cluster2, bool horizontal);

    /**
     * Удаляем невалидные (лишние) узлы области, состояние которых не было изменено в ходе перестройки области.
     * @param cluster Область.
     */
    void removeInvalidBorderNodes(const std::shared_ptr<Cluster>& cluster);

    /**
     * Удаляем абстрактные узлы области, если их дочерние узлы были удалены.
     * @param cluster Абстрактная область.
     */
    void removeEmptyAbstractBorderNodes(const std::shared_ptr<Cluster>& cluster);

    /**
     * Отвязать узел от графа.
     * @param node Узел.
     */
    void unlinkNode(const std::shared_ptr<Node>& node);
};

} // namespace Engine