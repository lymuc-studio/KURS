#pragma once

#include "kurspch.hpp"

#include "Core/IterableRange.hpp"

namespace kurs
{
	class IDRegistry
	{
	public:
		using IDType = std::size_t;
		static constexpr IDType c_InvalidID = -1;

		using ExistingIDRange = IterableRange<std::unordered_set<IDType>::iterator>;
		
		using ExistingIDConstRange 
			= IterableRange<std::unordered_set<IDType>::const_iterator>;

		IDType GenerateID();
		bool CheckIDExists(IDType id) const;
		void DestroyID(IDType id);

		ExistingIDRange GetExistingIDs();
		ExistingIDConstRange GetExistingIDs() const;

	private:
		IDType m_NextID = 0;
		std::unordered_set<IDType> m_ExistingIDs;
		std::stack<IDType, std::vector<IDType>> m_RecycledIDs;
	};
}
