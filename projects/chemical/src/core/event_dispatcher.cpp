#include <pch.h>

#include "event_dispatcher.h"

namespace Chemical {

	void EventDispatcher::subscribe(const Event::DescriptorType& descriptor, ObserverType&& slot) {
		_observers[descriptor].emplace_back(slot);
	}

	void EventDispatcher::post(const Event& event) const {
		Event::DescriptorType type = event.type();

		auto observers = _observers.find(type);
		if (observers == _observers.end())
			return;

		for (auto&& observer : observers->second) {
			observer(event);
		}
	}
}