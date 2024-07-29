#pragma once

#include <functional>

namespace Chemical {

	namespace Core {

		struct Event {
		public:

			using DescriptorType = const char*;

			virtual DescriptorType GetType() const = 0;
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
			EventDispatcher(const EventDispatcher& dispatcher) = delete;

			EventDispatcher& operator=(const EventDispatcher& dispatcher) = delete;

			using ObserverType = std::function<void(const Event&)>;

			void Subscribe(const Event::DescriptorType& descriptor, ObserverType&& slot);

			void Post(const Event& event) const;

		private:

			std::unordered_map<Event::DescriptorType, std::vector<ObserverType>> m_observers;
		};
	}


}
