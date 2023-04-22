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

	UniqueEventSubscription::UniqueEventSubscription(
		EventID eventID, 
		IDRegistry::IDType subscriberID
	)
		: m_EventID(eventID)
		, m_SubscriberID(subscriberID)
	{
	}

	UniqueEventSubscription::UniqueEventSubscription(
		UniqueEventSubscription&& other
	) noexcept
		: m_EventID(std::exchange(other.m_EventID, TypeID_Undefined))
		, m_SubscriberID(
				std::exchange(
					other.m_SubscriberID, 
					IDRegistry::c_InvalidID
				)
			)
	{
	}

	UniqueEventSubscription& UniqueEventSubscription::operator=(
		UniqueEventSubscription&& other
	) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_EventID = std::exchange(other.m_EventID, TypeID_Undefined);
		m_SubscriberID = std::exchange(
			other.m_SubscriberID, 
			IDRegistry::c_InvalidID
		);

		return *this;
	}

	EventID UniqueEventSubscription::GetEventID() const
	{
		return m_EventID;
	}

	IDRegistry::IDType UniqueEventSubscription::GetSubscriberID() const
	{
		return m_SubscriberID;
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// EventBus
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	void EventBus::Unsubscribe(UniqueEventSubscription subscription)
	{
		EventID evID = subscription.GetEventID();
		IDRegistry::IDType subID = subscription.GetSubscriberID();
	
		KURS_ASSERT(TypeID_Undefined != evID && IDRegistry::c_InvalidID != subID);

		m_EventSpecificRegistry[evID].RemoveCallback(subID);
	
		KURS_LOG(
			Debug,
			"Event callback removed. EventID %zu. Subscriber ID %zu", 
			evID, 
			subID
		);
	}

	void EventBus::Publish(EventBase& event)
	{
		KURS_LOG(Debug, "Publishing %s", event.GetName().data());
		
		EventID evID = event.GetID();
		
		if (evID >= m_EventSpecificRegistry.size())
		{
			KURS_LOG(Warn, "No callbacks available for %s", event.GetName().data());
			return;
		}
	
		m_EventSpecificRegistry[evID].Notify(event);
	}
}
