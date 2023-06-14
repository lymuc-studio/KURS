#include "Core/IDRegistry.hpp"

namespace kurs
{
	IDRegistry::IDType IDRegistry::GenerateID()
	{
		if (m_RecycledIDs.size())
		{
			IDType id = m_RecycledIDs.top();
			m_RecycledIDs.pop();
		
			m_ExistingIDs.insert(id);

			return id;
		}

		return *m_ExistingIDs.insert(m_NextID++).first;
	}

	bool IDRegistry::CheckIDExists(IDType id) const
	{
		return m_ExistingIDs.find(id) != m_ExistingIDs.end();
	}

	void IDRegistry::DestroyID(IDType id)
	{
		m_ExistingIDs.erase(id);
		m_RecycledIDs.push(id);
	}

	IDRegistry::ExistingIDRange IDRegistry::GetExistingIDs()
	{
		return ExistingIDRange(m_ExistingIDs.begin(), m_ExistingIDs.end());
	}
	
	IDRegistry::ExistingIDConstRange IDRegistry::GetExistingIDs() const
	{
		return ExistingIDConstRange(m_ExistingIDs.begin(), m_ExistingIDs.end());
	}
}
