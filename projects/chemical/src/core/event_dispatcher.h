#pragma once

#include <functional>

namespace Chemical {

	struct Event {
	public:

		using DescriptorType = const char*;

		virtual DescriptorType type() const = 0;
	};

	/*struct DemoEvent : public Event {
	public:

		static constexpr const char* descriptor = "DemoEvent";

		virtual DescriptorType type() const override {
			return descriptor;
		}
	};*/

	class EventDispatcher {
	public:

		EventDispatcher() = default;

		using ObserverType = std::function<void(const Event&)>;

		void subscribe(const Event::DescriptorType& descriptor, ObserverType&& slot);

		void post(const Event& event) const;

	private:

		EventDispatcher(const EventDispatcher& dispatcher) = delete;

		EventDispatcher& operator=(const EventDispatcher& dispatcher) = delete;

		std::unordered_map<Event::DescriptorType, std::vector<ObserverType>> _observers;
	};

}
