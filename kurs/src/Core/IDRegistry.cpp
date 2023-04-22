#include "Core/IDRegistry.hpp"

#include "Debug/Assertion/Assert.hpp"

namespace kurs
{
	IDRegistry::IDType IDRegistry::GenerateID()
	{
		if (!CheckPreservedIDsAvailable())
		{
			return CreateID();
		}
	
		return RecycleID();
	}

	bool IDRegistry::CheckIDExists(IDType id)
	{
		return id < m_InUse.size() && CheckIDInUse(id);
	}

	void IDRegistry::DestroyID(IDType id)
	{
		KURS_ASSERT(
			CheckIDExists(id), 
			"Trying to destroy ID (%zu) that does not exist", 
			id
		);
		PreserveID(id);
	}

	IDRegistry::IDType IDRegistry::RecycleID()
	{
		KURS_ASSERT(CheckPreservedIDsAvailable());

		IDType id = m_FreeListTail;

		m_InUse[id] = true;
		m_FreeListTail = m_PrevFree[id];

		return id;
	}

	void IDRegistry::PreserveID(IDType id)
	{
		KURS_ASSERT(CheckIDExists(id));
		
		m_InUse[id] = false;

		if (!CheckPreservedIDsAvailable())
		{
			m_FreeListTail = id;
			return;
		}
	
		m_PrevFree[id] = m_FreeListTail;
		m_FreeListTail = id;
	}

	bool IDRegistry::CheckPreservedIDsAvailable() const
	{
		return c_InvalidListIndex != m_FreeListTail;
	}
	
	bool IDRegistry::CheckIDInUse(IDType id) const
	{
		return m_InUse[id];
	}

	IDRegistry::IDType IDRegistry::CreateID()
	{
		m_InUse.push_back(true);
		m_PrevFree.push_back(c_InvalidListIndex);
	
		IDType id = m_InUse.size() - 1;
		KURS_ASSERT(id + 1 == m_PrevFree.size());
	
		return id;
	}
}
