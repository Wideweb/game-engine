#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

namespace Engine {

template <typename TEvent, size_t TSize> class ObjectPool {
  public:
    void push(TEvent event) {
        m_Data[tail] = event;
        tail = (tail + 1) % TSize;
    }

    TEvent pop() {
        auto &event = m_Data[index];
        index = (index + 1) % TSize;
        return event;
    }

    bool empty() const { return index == tail; }

  private:
    std::array<TEvent, TSize> m_Data;
    size_t index, tail = 0;
};

} // namespace Engine
