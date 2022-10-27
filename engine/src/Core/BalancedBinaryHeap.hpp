#pragma once

#include "CustomTraits.hpp"

#include <vector>

namespace Engine {

template<typename TNode>
class BalancedBinaryHeap {
  private:
    /**
     * Массив узлов в очереди. Представляет собой сбалансированную двоичную
     * кучу, где детьми элемента nodes[n] являются элементы nodes[2*n+1]
     * и nodes[2*(n+1)].
     */
    std::vector<TNode> m_Nodes;

    /**
     * Текущее количество узлов в очереди.
     */
    uint32_t m_Size = 0;

  public:

    /**
     * Вместимость очереди по умолчанию.
     */
    BalancedBinaryHeap() : BalancedBinaryHeap(11) {}

    BalancedBinaryHeap(int initialCapacity) {
        m_Nodes.resize(initialCapacity);
    }

    /**
     * Добавит узел в очередь.
     *
     * @param node узел.
     */
    void add(const TNode& node) {
        uint32_t index = m_Size++;
        if (index >= m_Nodes.size()) {
            grow();
        }
        siftUp(index, node);
    }

    /**
     * Обновит позицию узла в очереди на основе его комбинированной стоимости.
     *
     * @param node узел.
     */
    void update(const TNode& node) {
        siftUp(getHeapIndex(node), node);
    }

    /**
     * Удалит из очереди и вернет узел с наименьшей комбинированной стоимостью.
     *
     * @return узел с наименьшей стоимостью.
     */
    TNode poll() {
        uint32_t lastIndex = --m_Size;
        setHeapIndex(m_Nodes[0], -1);
        TNode first = m_Nodes[0];
        // m_Nodes[lastIndex] = null;
        if (lastIndex != 0) {
            siftDown(0, m_Nodes[lastIndex]);
        }
        return first;
    }

    /**
     * Вернет true, если узел находится в очереди.
     *
     * @param node узел.
     * @return true, если узел находится в очереди.
     */
    bool contains(const TNode& node) const {
        return getHeapIndex(node) >= 0;
    }

    /**
     * Определяет, пустая ли очередь.
     * @return true, если очередь пуста.
     */
    bool empty() {
        return m_Size == 0;
    }

  private:

    /**
     * Переместит узел вверх по дереву, отталкиваясь от его комбинированной
     * стоимости.
     *
     * @param index индекс узла.
     * @param node узел.
     */
    void siftUp(uint32_t index, const TNode& node) {
        while (index > 0) {
            uint32_t parentIndex = (index - 1) >> 1;
            if (greater(node, m_Nodes[parentIndex]))
            {
                break;
            }
            setHeapIndex(m_Nodes[parentIndex], index);
            m_Nodes[index] = std::move(m_Nodes[parentIndex]);
            index = parentIndex;
        }

        setHeapIndex(node, index);
        m_Nodes[index] = node;
    }

    /**
     * Переместит узел вниз по дереву, отталкиваясь от его комбинированной
     * стоимости.
     *
     * @param index индекс узла.
     * @param node узел.
     */
    void siftDown(uint32_t index, TNode& node) {
        uint32_t half = m_Size >> 1;
        while (index < half) {
            uint32_t curr = (index << 1) + 1;
            uint32_t right = curr + 1;

            if (right < m_Size && greater(m_Nodes[curr], m_Nodes[right])) {
                curr = right;
            }
            
            if (!greater(node, m_Nodes[curr])) {
                break;
            }
            
            setHeapIndex(m_Nodes[curr], index);
            m_Nodes[index] = m_Nodes[curr];
            index = curr;
        }

        setHeapIndex(node, index);
        m_Nodes[index] = node;
    }

    /**
     * Увеличит вместимость очереди.
     */
    void grow() {
        uint32_t size = m_Nodes.size();
        uint32_t newSize = size + (size < 64 ? size + 2 : size >> 1);
        m_Nodes.resize(newSize);
    }

    template <typename T>
    bool greater(const T& first, const T& second, typename std::enable_if<is_pointer_like_arrow_dereferencable_v<T>>::type* = 0) const {
        return (*first) > (*second);
    }

    template <typename T>
    bool greater(const T& first, const T& second, typename std::enable_if<!is_pointer_like_arrow_dereferencable_v<T>>::type* = 0) const {
        return first > second;
    }

    template <typename T>
    void setHeapIndex(const T& node, int32_t index, typename std::enable_if<is_pointer_like_arrow_dereferencable_v<T>>::type* = 0) const {
        node->setHeapIndex(index);
    }

    template <typename T>
    void setHeapIndex(const T& node, int32_t index, typename std::enable_if<!is_pointer_like_arrow_dereferencable_v<T>>::type* = 0) const {
        node.setHeapIndex(index);
    }

    template <typename T>
    int32_t getHeapIndex(const T& node, typename std::enable_if<is_pointer_like_arrow_dereferencable_v<T>>::type* = 0) const {
        return node->getHeapIndex();
    }

    template <typename T>
    int32_t getHeapIndex(const T& node, typename std::enable_if<!is_pointer_like_arrow_dereferencable_v<T>>::type* = 0) const {
        return node.getHeapIndex();
    }
};

} // namespace Engine
