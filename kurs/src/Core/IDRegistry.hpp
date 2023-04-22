#pragma once

#include "kurspch.hpp"

namespace kurs
{
	class IDRegistry
	{
	public:
		using IDType = std::size_t;

		IDType GenerateID();
		bool CheckIDExists(IDType id);
		void DestroyID(IDType id);

	private:
		IDType RecycleID();
		void PreserveID(IDType id);
		bool CheckPreservedIDsAvailable() const;
		bool CheckIDInUse(IDType id) const;

		IDType CreateID();

		std::vector<bool> m_InUse;
		std::vector<std::ptrdiff_t> m_PrevFree;

		static constexpr std::ptrdiff_t c_InvalidListIndex = -1;
		std::ptrdiff_t m_FreeListTail = c_InvalidListIndex;
	};
}
