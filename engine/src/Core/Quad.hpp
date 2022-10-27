#pragma once

#include <array>
#include <optional>

namespace Engine {

/**
 * Сегмент словаря. Хранит узлы с соответствующими этому сегменту
 * координатами.
 */
template<typename T, size_t SIZE>
struct Quad
{
    /**
     * Массив узлов сегмента.
     */
    std::array<std::optional<T>, SIZE * SIZE> nodes;

    /**
     * Найдет в сегменте и вернет узел с указанными координатами.
     *
     * @param i i-координата узла.
     * @return узел или null, если таковой отсутствует.
     */
    std::optional<T>& getNode(int i) {
        return nodes[i];
    }

    T* getNodePtr(int i) {
        return &nodes[i].value();
    }

    template <class... TArgs>
    T* emplaceNode(int i, TArgs &&... args) {
         nodes[i] = std::make_optional<T>(std::forward<TArgs>(args)...);
         return &nodes[i].value();
    }

    /**
     * Поместит указанный узел в сегмент.
     *
     * @param node узел.
     */
    T* putNode(int i, const T& node) {
        nodes[i] = std::make_optional<T>(node);
        return &nodes[i].value();
    }

    void clear() {
        for (size_t i = 0; i < nodes.size(); i++) {
            nodes[i] = std::nullopt;
        }
    }
};

} // namespace Engine