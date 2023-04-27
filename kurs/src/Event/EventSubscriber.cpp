#include "Event/EventSubscriber.hpp"

namespace kurs
{
	EventSubscriber::EventSubscriber(EventBus* bus)
		: m_Bus(bus)
	{
	}

	EventSubscriber::EventSubscriber(EventSubscriber&& other) noexcept
		: m_Bus(std::exchange(other.m_Bus, nullptr))
		, m_Subscriptions(std::move(other.m_Subscriptions))
	{
	}

	EventSubscriber::~EventSubscriber()
	{
		Unsubscribe();
	}

	EventSubscriber& EventSubscriber::operator=(EventSubscriber&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		Reset();
		m_Bus = std::exchange(other.m_Bus, nullptr);
		m_Subscriptions = std::move(other.m_Subscriptions);
	
		return *this;
	}

	void EventSubscriber::SetBus(EventBus* bus)
	{
		Unsubscribe();
		m_Bus = bus;
	}

	EventBus* EventSubscriber::GetBus() const
	{
		return m_Bus;
	}

	void EventSubscriber::Reset()
	{
		SetBus(nullptr);
	}

	void EventSubscriber::AddCustomSubscription(
		EventSubscription subscription
	)
	{
		m_Subscriptions.push_back(std::move(subscription));
	}

	void EventSubscriber::Unsubscribe()
	{
		if (m_Bus)
		{
			for (auto& sub : m_Subscriptions)
			{
				m_Bus->Unsubscribe(std::move(sub));
			}
		}
	
		m_Subscriptions.clear();
	}
}
