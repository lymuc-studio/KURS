#pragma once

#include "kurspch.hpp"

#include "ECS/EntityID.hpp"
#include "ECS/ComponentBase.hpp"

#include "Debug/Assertion/Assert.hpp"

namespace kurs::detail
{
	class IComponentPool
	{
	public:
		virtual ~IComponentPool() = default;

		virtual bool HasComponent(EntityID eid) const = 0;
		virtual void RemoveComponent(EntityID eid) = 0;
	};

	template<typename ComponentT>
	class ComponentPool : public IComponentPool
	{
	public:
		using Traits = ComponentTraits<ComponentT>;
		using ValueType = typename Traits::ValueType;
		using ReferenceType = typename Traits::ReferenceType;
		using Storage = typename Traits::Storage;

		bool HasComponent(EntityID eid) const override
		{
			return eid < m_IsOwner.size() && m_IsOwner[eid];
		}

		void RemoveComponent(EntityID eid) override
		{
			KURS_ASSERT(
				HasComponent(eid),
				"Could not remove '%s' component: "
				"entity (ID = %zu) does not have such component",
				getTypeName<ValueType>().data(),
				eid
			);
		
			m_IsOwner[eid] = false;
			m_Components[eid].Destruct();
		}

		template<typename... ArgsT>
		ReferenceType EmplaceComponent(EntityID eid, ArgsT&&... args)
		{
			KURS_ASSERT(
				!HasComponent(eid),
				"Could not emplace '%s' component: "
				"entity (ID = %zu) already has that component",
				getTypeName<ValueType>().data(),
				eid
			);

			if (eid >= m_IsOwner.size())
			{
				m_IsOwner.resize(eid + 1);
			}
			m_IsOwner[eid] = true;

			if (eid >= m_Components.size())
			{
				m_Components.resize(eid + 1);
			}
			return m_Components[eid].Construct(std::forward<ArgsT>(args)...);
		}

		ReferenceType GetComponent(EntityID eid)
		{
			KURS_ASSERT(
				HasComponent(eid),
				"Could not retrieve '%s' component: "
				"entity (ID = %zu) does not have such component",
				getTypeName<ValueType>().data(),
				eid
			);

			return m_Components[eid].Get();
		}

		template<typename FunctorT>
		void ForEach(FunctorT functor)
		{
			for (EntityID eid = 0; eid < m_Components.size(); eid++)
			{
				if (HasComponent(eid))
				{
					functor(static_cast<const EntityID&>(eid), GetComponent(eid));
				}
			}
		}

	private:
		std::vector<bool> m_IsOwner;
		std::vector<Storage> m_Components;
	};
}
