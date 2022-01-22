
#pragma once

#include "FlatDictionary.hpp"

#include <functional>
#include <iterator>
#include <list>

namespace Engine {

template <class... TArgs> using EventDelegate = std::function<void(TArgs &&...args)>;

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

template <class... TArgs> class EventDispatcher : public IEventDispatcher {
  private:
    std::list<EventDelegate<TArgs...>> m_Delegates;
    // ObjectManager<uint32_t> m_Delegates;

  public:
    void dispatch(TArgs... args) {
        for (auto &delegate : m_Delegates) {
            delegate(std::forward<TArgs>(args)...);
        }
    }

    void addEventCallback(const EventDelegate<TArgs...> &delegate) { m_Delegates.emplace_back(delegate); }
};

} // namespace Engine
