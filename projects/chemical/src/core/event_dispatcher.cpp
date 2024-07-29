#include <pch.h>

#include "event_dispatcher.h"

namespace Chemical {

	namespace Core {
		void EventDispatcher::Subscribe(const Event::DescriptorType& descriptor, ObserverType&& slot) {
			m_observers[descriptor].emplace_back(slot);
		}

		void EventDispatcher::Post(const Event& event) const {
			Event::DescriptorType type = event.GetType();

			auto observers = m_observers.find(type);
			if (observers == m_observers.end())
				return;

			for (auto&& observer : observers->second) {
				observer(event);
			}
		}
	}

}