
#pragma once

#include <functional>
#include <vector>

namespace Engine {

template <typename T> using EventDelegate = std::function<void(T &)>;

class IEventDispatcher {};

template <typename TEvent> class EventDispatcher : public IEventDispatcher {
  private:
    std::vector<EventDelegate<TEvent>> m_Delegates;

  public:
    void dispatch(TEvent &event) {
        for (auto &delegate : m_Delegates) {
            delegate(event);
        }
    }

    void addEventCallback(EventDelegate<TEvent> &delegate) {
        m_Delegates.push_back(delegate);
    }
};

} // namespace Engine
