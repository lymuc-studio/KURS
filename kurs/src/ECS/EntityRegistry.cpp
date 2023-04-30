#include "ECS/EntityRegistry.hpp"

#include "Debug/Assertion/Assert.hpp"

namespace kurs::detail
{
	EntityID EntityRegistry::CreateEntity()
	{
		EntityID eid = m_Entities.GenerateID();

		if (eid >= m_EntityRefCount.size())
		{
			m_EntityRefCount.resize(eid + 1);
		}
		m_EntityRefCount[eid] = 1;

		return eid;
	}

	std::size_t EntityRegistry::GetEntityRefCount(EntityID eid) const
	{
		KURS_ASSERT(
			m_Entities.CheckIDExists(eid),
			"Could not obtain entity reference count: "
			"entity (ID = %zu) does not exist",
			eid
		);

		KURS_ASSERT(eid < m_EntityRefCount.size());

		return m_EntityRefCount[eid];
	}

	void EntityRegistry::IncEntityRefCount(EntityID eid)
	{
		KURS_ASSERT(
			m_Entities.CheckIDExists(eid),
			"Could not increment entity reference count: "
			"entity (ID = %zu) does not exist",
			eid
		);

		KURS_ASSERT(eid < m_EntityRefCount.size());

		m_EntityRefCount[eid]++;
	}

	void EntityRegistry::ReleaseEntity(EntityID eid)
	{
		KURS_ASSERT(
			m_Entities.CheckIDExists(eid),
			"Could not destroy entity: entity (ID = %zu) does not exist",
			eid
		);

		KURS_ASSERT(eid < m_EntityRefCount.size());

		if (!--m_EntityRefCount[eid])
		{
			RemoveAllComponents(eid);
			m_Entities.DestroyID(eid);
		}
	}

	bool EntityRegistry::CheckEntityExists(EntityID eid) const
	{
		return m_Entities.CheckIDExists(eid);
	}

	void EntityRegistry::RemoveAllComponents(EntityID eid)
	{
		KURS_ASSERT(
			CheckEntityExists(eid),
			"Entity (ID = %zu) does not exist",
			eid
		);

		for (auto& pool : m_Pools)
		{
			if (pool && pool->HasComponent(eid))
			{
				pool->RemoveComponent(eid);
			}
		}
	}

	bool EntityRegistry::HasPoolOf(TypeID componentTypeID) const
	{
		return componentTypeID < m_Pools.size() && m_Pools[componentTypeID];
	}

	IComponentPool* EntityRegistry::GetPoolOf(TypeID componentTypeID) const
	{
		if (!HasPoolOf(componentTypeID))
		{
			return nullptr;
		}
		return m_Pools[componentTypeID].get();
	}
}
