#pragma once

#include "kurspch.hpp"

#include "Event/EventBus.hpp"

#include "Debug/Assertion/Assert.hpp"

namespace kurs
{
	class EventSubscriber;
}

namespace kurs::detail
{
	template<typename ThisT>
	class EventSubscriptionForwarder
	{
	public:
		EventSubscriptionForwarder(EventSubscriber& subscriber, ThisT* ptr)
			: m_Subscriber(subscriber)
			, m_Ptr(ptr)
		{
			KURS_ASSERT(m_Ptr, "Trying to bind a nullptr `this`");
		}

		template<typename EventT>
		EventSubscriptionForwarder& AddMethod(void(ThisT::*method)(EventT&));
		
		EventSubscriber& EndThis()
		{
			return m_Subscriber;
		}

	private:
		EventSubscriber& m_Subscriber;
		ThisT* m_Ptr = nullptr;
	};
}

namespace kurs
{
	class EventSubscriber
	{
	public:
		EventSubscriber() = default;
		explicit EventSubscriber(EventBus* bus);
		
		EventSubscriber(EventSubscriber&& other) noexcept;

		~EventSubscriber();

		EventSubscriber& operator=(EventSubscriber&& other) noexcept;

		void SetBus(EventBus* bus);
		EventBus* GetBus() const;
		void Reset();

		void AddCustomSubscription(EventSubscription subscription);

		template<typename EventT>
		EventSubscriber& AddFunction(void(*func)(EventT&))
		{
			KURS_ASSERT(func, "Trying to bind a nullptr function");
			KURS_ASSERT(m_Bus, "EventBus is not set");
			
			AddCustomSubscription(m_Bus->Subscribe<EventT>(func));

			return *this;
		}

		template<typename ThisT>
		detail::EventSubscriptionForwarder<ThisT> BeginThis(ThisT* ptr)
		{
			KURS_ASSERT(ptr, "Trying to bind a nullptr `this`");
			KURS_ASSERT(m_Bus, "EventBus is not set");
			return detail::EventSubscriptionForwarder<ThisT>(*this, ptr);
		}

	private:
		void Unsubscribe();

		EventBus* m_Bus = nullptr;
		std::vector<EventSubscription> m_Subscriptions;
	};
}

namespace kurs::detail
{
	template<typename ThisT>
	template<typename EventT>
	EventSubscriptionForwarder<ThisT>& EventSubscriptionForwarder<ThisT>::AddMethod(
		void(ThisT::*method)(EventT&)
	)
	{
		KURS_ASSERT(method, "Member function is nullptr");

		m_Subscriber.AddCustomSubscription(
			m_Subscriber.GetBus()->Subscribe<EventT>(
				[ptr = m_Ptr, method](EventT& event)
				{
					(ptr->*method)(event);
				}
			)
		);
	
		return *this;
	}
}
