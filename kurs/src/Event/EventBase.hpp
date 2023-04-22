#pragma once

#include "kurspch.hpp"

#include "Core/TypeID.hpp"

#include "Debug/Logging/Logger.hpp"

namespace kurs
{
	using EventID = TypeID;

	template<typename EventT>
	EventID getEventID()
	{
		return getTypeID<EventT>();
	}

	template<typename EventT>
	std::string getEventName()
	{
		return std::string(getTypeName<EventT>());
	}

	class EventBase
	{
	public:
		virtual ~EventBase() = default;

		virtual TypeID GetID() const = 0;
		virtual std::string GetName() const = 0;
	};

	template<typename DerivedT>
	class Event : public EventBase
	{
	public:
		TypeID GetID() const override final
		{
			return getEventID<DerivedT>();
		}

		std::string GetName() const override final
		{
			return getEventName<DerivedT>();
		}
	};
}

namespace kurs::detail
{
	template<typename EventT>
	EventT* event_cast(EventBase* event)
	{
		if (!event)
		{
			return nullptr;
		}

		KURS_LOG(
			Debug,
			"Casting an event from %s to %s", event->GetName().data(),
			getEventName<EventT>().data()
		);

		if (getEventID<EventT>() != event->GetID())
		{
			KURS_LOG(Debug, "Cast failed");
			return nullptr;
		}

		return static_cast<EventT*>(event);
	}
}
