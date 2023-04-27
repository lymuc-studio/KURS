#pragma once

#include "kurspch.hpp"

#include "Core/TypeID.hpp"

namespace kurs
{
	struct EventBase
	{
		virtual ~EventBase() = default;

		virtual TypeID GetTypeID() const = 0;
		virtual std::string GetName() const = 0;
	};

	template<typename DerivedT>
	struct Event : public EventBase
	{
		TypeID GetTypeID() const override final
		{
			return getTypeID<DerivedT>();
		}

		std::string GetName() const override final
		{
			return getTypeName<DerivedT>();
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

		if (getTypeID<EventT>() != event->GetTypeID())
		{
			return nullptr;
		}

		return static_cast<EventT*>(event);
	}
}
