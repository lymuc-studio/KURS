#pragma once

#include "kurspch.hpp"

#include "Core/TypeID.hpp"

#include "ECS/EntityID.hpp"
#include "ECS/ComponentPool.hpp"

namespace kurs::detail
{
	class EntityRegistry
	{
	public:
		static constexpr EntityID c_NullEntityID = IDRegistry::c_InvalidID;

		EntityID CreateEntity();
		std::size_t GetEntityRefCount(EntityID eid) const;
		void IncEntityRefCount(EntityID eid);
		void ReleaseEntity(EntityID eid);
		bool CheckEntityExists(EntityID eid) const;

		void RemoveAllComponents(EntityID eid);

		template<typename ComponentT>
		bool HasComponent(EntityID eid) const
		{
			if (!HasPoolOf<ComponentT>())
			{
				return false;
			}

			return GetPoolOf<ComponentT>()->HasComponent(eid);
		}

		template<typename... ComponentsT>
		bool HasAllOf(EntityID eid) const
		{
			return (HasComponent<ComponentsT>(eid) && ...);
		}

		template<typename ComponentT, typename... ArgsT>
		ComponentT& EmplaceComponent(EntityID eid, ArgsT&&... args)
		{
			if (!HasPoolOf<ComponentT>())
			{
				return MakePoolOf<ComponentT>()->EmplaceComponent(
					eid, 
					std::forward<ArgsT>(args)...
				);
			}
		
			return GetPoolOf<ComponentT>()->EmplaceComponent(
				eid,
				std::forward<ArgsT>(args)...
			);
		}

		template<typename ComponentT>
		ComponentT& GetComponent(EntityID eid)
		{
			KURS_ASSERT(
				HasComponent<ComponentT>(eid),
				"Could not retrieve '%s' component: "
				"entity (ID = %zu) does not have such component",
				getTypeName<ComponentT>().data(),
				eid
			);

			return GetPoolOf<ComponentT>()->GetComponent(eid);
		}

		template<typename ComponentT>
		void RemoveComponent(EntityID eid)
		{
			KURS_ASSERT(
				HasComponent<ComponentT>(eid),
				"Could not remove '%s' component: "
				"entity (ID = %zu) does not have such component",
				getTypeName<ComponentT>().data(),
				eid
			);
			GetPoolOf<ComponentT>()->RemoveComponent(eid);
		}

		template<typename ComponentT, typename FunctorT>
		void ForEach(FunctorT functor)
		{
			if (HasPoolOf<ComponentT>())
			{
				GetPoolOf<ComponentT>()->ForEach(functor);
			}
		}

	private:
		template<typename ComponentT>
		bool HasPoolOf() const
		{
			return HasPoolOf(getTypeID<ComponentT>());
		}
		
		template<typename ComponentT>
		ComponentPool<ComponentT>* GetPoolOf() const
		{
			return static_cast<ComponentPool<ComponentT>*>(
				GetPoolOf(getTypeID<ComponentT>())
			);
		}

		template<typename ComponentT>
		ComponentPool<ComponentT>* MakePoolOf()
		{
			KURS_ASSERT(
				!HasPoolOf<ComponentT>(), 
				"Pool of '%s' components already exists",
				getTypeName<ComponentT>().data()
			);
			
			TypeID componentTypeID = getTypeID<ComponentT>();
		
			if (componentTypeID >= m_Pools.size())
			{
				m_Pools.resize(componentTypeID + 1);
			}

			auto pool = std::make_unique<ComponentPool<ComponentT>>();
			ComponentPool<ComponentT>* poolPtr = pool.get();
			m_Pools[componentTypeID] = std::move(pool);
		
			return poolPtr;
		}

		bool HasPoolOf(TypeID componentTypeID) const;
		IComponentPool* GetPoolOf(TypeID componentTypeID) const;

		IDRegistry m_Entities;
		std::vector<std::size_t> m_EntityRefCount;
		std::vector<std::unique_ptr<IComponentPool>> m_Pools;
	};
}
