#include "Event/EventPoster.hpp"

namespace kurs
{
	EventPoster::EventPoster(EventBus* bus)
		: m_Bus(bus)
	{
	}

	EventPoster::EventPoster(EventPoster&& other) noexcept
		: m_Bus(std::exchange(other.m_Bus, nullptr))
	{
	}

	void EventPoster::SetBus(EventBus* bus)
	{
		m_Bus = bus;
	}

	EventBus* EventPoster::GetBus() const
	{
		return m_Bus;
	}

	void EventPoster::Reset()
	{
		SetBus(nullptr);
	}

	void EventPoster::PostEvent(EventBase& event)
	{
		if (m_Bus)
		{
			m_Bus->PostEvent(event);
		}
	}

	EventPoster& EventPoster::operator=(EventPoster&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_Bus = std::exchange(other.m_Bus, nullptr);

		return *this;
	}
}
