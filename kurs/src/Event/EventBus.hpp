#pragma once

#include "kurspch.hpp"

#include "Core/IDRegistry.hpp"

#include "Event/EventBase.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Assertion/Assert.hpp"

namespace kurs::detail
{
	template<typename EventT>
	using EventCallbackFunc = std::function<void(EventT&)>;

	class IEventCallback
	{
	public:
		virtual ~IEventCallback() = default;

		virtual void Notify(EventBase& event) = 0;
	};

	template<typename EventT>
	class EventCallback : public IEventCallback
	{
	public:
		EventCallback(EventCallbackFunc<EventT> callbackFunc)
			: m_CallbackFunc(callbackFunc)
		{
			KURS_LOG(
				Debug, 
				"EventCallback<%s> initialized", 
				getEventName<EventT>().data()
			);
		}

		void Notify(EventBase& event)
		{
			if (EventT* actualEvent = event_cast<EventT>(&event))
			{
				m_CallbackFunc(*actualEvent);
				return;
			}

			KURS_LOG(
				Warn,
				"Event type mismatch: %s expected, but %s provied",
				getEventName<EventT>().data(),
				event.GetName().data()
			);
		}

	private:
		EventCallbackFunc<EventT> m_CallbackFunc;
	};

	class EventSubscriberRegistry
	{
	public:
		IDRegistry::IDType AddCallback(std::unique_ptr<IEventCallback> callback);
		void RemoveCallback(IDRegistry::IDType subscriberID);
		void Notify(EventBase& event);

	private:
		std::vector<std::unique_ptr<IEventCallback>> m_Callbacks;
		IDRegistry m_SubscriberRegistry;
	};
}

namespace kurs
{
	class UniqueEventSubscription
	{
	public:
		UniqueEventSubscription() = default;
		UniqueEventSubscription(EventID eventID, IDRegistry::IDType subscriberID);

		UniqueEventSubscription(const UniqueEventSubscription& other) = delete;
		UniqueEventSubscription(UniqueEventSubscription&& other) noexcept;

		UniqueEventSubscription& operator=(
			const UniqueEventSubscription& other
		) = delete;
		
		UniqueEventSubscription& operator=(
			UniqueEventSubscription&& other
		) noexcept;

		EventID GetEventID() const;
		IDRegistry::IDType GetSubscriberID() const;

	private:
		EventID m_EventID = TypeID_Undefined;
		IDRegistry::IDType m_SubscriberID = IDRegistry::c_InvalidID;
	};

	class EventBus
	{
	public:
		template<typename EventT>
		UniqueEventSubscription Subscribe(
			detail::EventCallbackFunc<EventT> callbackFunc
		)
		{
			EventID evID = getEventID<EventT>();
			if (evID >= m_EventSpecificRegistry.size())
			{
				m_EventSpecificRegistry.resize(evID + 1);
			}
			
			auto callback = std::make_unique<detail::EventCallback<EventT>>(
				callbackFunc
			);
		
			IDRegistry::IDType subID = m_EventSpecificRegistry[evID].AddCallback(
				std::move(callback)
			);
		
			return UniqueEventSubscription(evID, subID);
		}

		void Unsubscribe(UniqueEventSubscription subscription);

		void Publish(EventBase& event);

	private:
		std::vector<detail::EventSubscriberRegistry> m_EventSpecificRegistry;
	};
}
