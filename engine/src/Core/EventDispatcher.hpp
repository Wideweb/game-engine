
#pragma once

#include <functional>
#include <iterator>
#include <list>

namespace Engine {

template <typename T> using EventDelegate = std::function<void(const T &)>;

class IEventDispatcher {};

// template <typename TEvent> class EventSubscription {
//   private:
//     std::function<void()> m_Unsubscribe;
//     bool m_Disposed = false;

//   public:
//     EventSubscription(std::function<void()> &unsubscribe) : m_Unsubscribe(unsubscribe) {}
//     ~EventSubscription() { unsubscribe(); }

//     void unsubscribe() {
//         if (m_Disposed) {
//             return;
//         }

//         m_Unsubscribe();
//         m_Disposed = true;
//     }
// };

template <typename TEvent> class EventDispatcher : public IEventDispatcher {
  private:
    std::list<EventDelegate<TEvent>> m_Delegates;

  public:
    void dispatch(const TEvent &event) {
        for (auto &delegate : m_Delegates) {
            delegate(event);
        }
    }

    void addEventCallback(const EventDelegate<TEvent> &delegate) { m_Delegates.push_back(delegate); }
};

} // namespace Engine
