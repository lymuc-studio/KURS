#pragma once

#include "kurspch.hpp"

#include "Core/TypeID.hpp"
#include "Core/Timer.hpp"

#include "ECS/EntityRegistry.hpp"

namespace kurs
{
	class EntitySystemBase
	{
	public:
		virtual ~EntitySystemBase() = default;

		virtual TypeID GetTypeID() const = 0;
		virtual std::string GetName() const = 0;
	
		virtual void OnAttach(std::weak_ptr<detail::EntityRegistry> registry) = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(Timestep ts) = 0;
	};

	template<typename DerivedT>
	class EntitySystem : public EntitySystemBase
	{
	public:
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
	template<typename SystemT>
	SystemT* entity_system_cast(EntitySystemBase* base)
	{
		if (!base)
		{
			return nullptr;
		}
	
		if (base->GetTypeID() != getTypeID<SystemT>())
		{
			return nullptr;
		}

		return static_cast<SystemT*>(base);
	}
}
