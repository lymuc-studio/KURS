#include "Event/EventBus.hpp"

namespace kurs::detail
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// EventSubscriberRegistry
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	IDRegistry::IDType EventSubscriberRegistry::AddCallback(
		std::unique_ptr<IEventCallback> callback
	)
	{
		IDRegistry::IDType subscriberID = m_SubscriberRegistry.GenerateID();
		if (subscriberID >= m_Callbacks.size())
		{
			m_Callbacks.resize(subscriberID + 1);
		}
		
		m_Callbacks[subscriberID] = std::move(callback);

		KURS_LOG(Debug, "Event callback added. Subscriber ID %zu", subscriberID);

		return subscriberID;
	}

	void EventSubscriberRegistry::RemoveCallback(IDRegistry::IDType subscriberID)
	{
		KURS_ASSERT(
			m_SubscriberRegistry.CheckIDExists(subscriberID), 
			"Invalid subscriber ID"
		);

		m_Callbacks[subscriberID].reset();
		m_SubscriberRegistry.DestroyID(subscriberID);

		KURS_LOG(Debug, "Event callback removed. Subscriber ID %zu", subscriberID);
	}

	void EventSubscriberRegistry::Notify(EventBase& event)
	{
		KURS_LOG(Debug, "Event callbacks handle %s", event.GetName().data());
		
		for (auto& callback : m_Callbacks)
		{
			if (callback)
			{
				callback->Notify(event);
			}
		}
	}
}

namespace kurs
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// UniqueEventSubscription
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	EventSubscription::EventSubscription(
		TypeID eventTypeID, 
		IDRegistry::IDType subscriberID
	)
		: m_EventTypeID(eventTypeID)
		, m_SubscriberID(subscriberID)
	{
	}

	EventSubscription::EventSubscription(
		EventSubscription&& other
	) noexcept
		: m_EventTypeID(std::exchange(other.m_EventTypeID, TypeID_Undefined))
		, m_SubscriberID(
				std::exchange(
					other.m_SubscriberID, 
					IDRegistry::c_InvalidID
				)
			)
	{
	}

	EventSubscription& EventSubscription::operator=(
		EventSubscription&& other
	) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_EventTypeID = std::exchange(other.m_EventTypeID, TypeID_Undefined);
		m_SubscriberID = std::exchange(
			other.m_SubscriberID, 
			IDRegistry::c_InvalidID
		);

		return *this;
	}

	TypeID EventSubscription::GetEventTypeID() const
	{
		return m_EventTypeID;
	}

	IDRegistry::IDType EventSubscription::GetSubscriberID() const
	{
		return m_SubscriberID;
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// EventBus
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	void EventBus::Unsubscribe(EventSubscription subscription)
	{
		TypeID evTypeID = subscription.GetEventTypeID();
		IDRegistry::IDType subID = subscription.GetSubscriberID();
	
		KURS_ASSERT(
			TypeID_Undefined != evTypeID 
			&& evTypeID < m_EventSpecificRegistry.size()
			&& IDRegistry::c_InvalidID != subID
		);

		m_EventSpecificRegistry[evTypeID].RemoveCallback(subID);
	
		KURS_LOG(
			Debug,
			"Event callback removed. EventID %zu. Subscriber ID %zu", 
			evTypeID, 
			subID
		);
	}

	void EventBus::PostEvent(EventBase& event)
	{
		KURS_LOG(Debug, "Publishing %s", event.GetName().data());
		
		TypeID evTypeID = event.GetTypeID();
		
		if (evTypeID >= m_EventSpecificRegistry.size())
		{
			KURS_LOG(Warn, "No callbacks available for %s", event.GetName().data());
			return;
		}
	
		m_EventSpecificRegistry[evTypeID].Notify(event);
	}
}
