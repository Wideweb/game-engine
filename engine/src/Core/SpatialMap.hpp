#pragma once

#include "Quad.hpp"

#include <vector>
#include <memory>

namespace Engine {

/**
 * Словарь для быстрого получения доступа к узлам с помощью их двумерных
 * координат. Использует так называемый "Spatial Hashing".
 */
template<typename T, size_t QUAD_SIZE>
class SpatialMap
{
  private:
    /**
     * Массив сегментов словаря.
     */
    std::vector<std::unique_ptr<Quad<T, QUAD_SIZE>>> m_Quads;

    /**
     * Активные сегменты словаря.
     */
    std::vector<int> m_ActiveQuads;

    /**
     * Количество вертикальных сегментов.
     */
    int m_QuadRows;

    /**
     * Количество горизонтальных сегментов.
     */
    int m_QuadCols;

  public:

    /**
     * Создаст словарь, соответствующий физическому полю с указанными высотой
     * и шириной. Поделит его на сегменты заданного размера.
     *
     * @param mapHeight высота физического поля.
     * @param mapWidth  ширина физического поля.
     */
    void init(int mapHeight, int mapWidth) {
        m_QuadRows = (mapHeight - 1) / QUAD_SIZE + 1;
        m_QuadCols = (mapWidth  - 1) / QUAD_SIZE + 1;

        if (m_Quads.size() != m_QuadRows * m_QuadCols) {
            m_Quads.resize(m_QuadRows * m_QuadCols);
        }
    }

    void clear() {
        for (int qIdx: m_ActiveQuads) {
            m_Quads[qIdx]->clear();
            m_Quads[qIdx] = nullptr;
        }
        m_ActiveQuads.clear();
    }

    bool hasNode(int x, int y) {
        auto& quad = m_Quads[quadIndex(x, y)];
        if (quad == nullptr) {
            return false;
        }
        auto& node = quad->getNode(itemIndex(x, y));
        return node.has_value();
    }

    T& getNode(int x, int y) {
        auto& quad = m_Quads[quadIndex(x, y)];
        return quad->getNode(itemIndex(x, y)).value();
    }

    T* getNodePtr(int x, int y) {
        auto& quad = m_Quads[quadIndex(x, y)];
        return quad->getNodePtr(itemIndex(x, y));
    }

    T& putNode(int x, int y, const T& node) {
        int qIdx = quadIndex(x, y);
        if (m_Quads[qIdx] == nullptr) {
            m_Quads[qIdx] = std::make_unique<Quad<T, QUAD_SIZE>>();
            m_ActiveQuads.push_back(qIdx);
        }
        int iIdx = itemIndex(x, y);
        return m_Quads[qIdx]->putNode(iIdx, node);
    }

    template <class... TArgs>
    T* emplaceNode(int x, int y, TArgs &&... args) {
         int qIdx = quadIndex(x, y);
         if (m_Quads[qIdx] == nullptr) {
             m_Quads[qIdx] = std::make_unique<Quad<T, QUAD_SIZE>>();
             m_ActiveQuads.push_back(qIdx);
        }
        int iIdx = itemIndex(x, y);
        return m_Quads[qIdx]->emplaceNode(iIdx, std::forward<TArgs>(args)...);
    }

    void forEach(std::function<void(T&)> consumer) {
        for (int i = 0; i < m_QuadRows * m_QuadCols; i++) {
            auto& quad = m_Quads[i];
            
            if (quad == nullptr) {
                continue;
            }

            for (int j = 0; j < QUAD_SIZE * QUAD_SIZE; j++) {
                auto& node = quad->getNode(j);
                if (node.has_value()) {
                    consumer(node.value());
                }
            }
        }
    }

  private:

    int quadIndex(int x, int y) {
        return (y / QUAD_SIZE) * m_QuadCols + (x / QUAD_SIZE);
    }

    int itemIndex(int x, int y) {
        int i = x & (QUAD_SIZE - 1);
        int j = y & (QUAD_SIZE - 1);
        return j * QUAD_SIZE + i;
    }
};

} // namespace Engine
