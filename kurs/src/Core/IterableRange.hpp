#pragma once

#include "kurspch.hpp"

namespace kurs
{
	template<typename IteratorT>
	class IterableRange
	{
	public:
		static_assert(!std::is_reference_v<IteratorT>);

		explicit IterableRange(IteratorT begin, IteratorT end)
			: m_Begin(begin)
			, m_End(end)
		{
		}

		auto begin() const
		{
			return m_Begin;
		}

		auto end() const
		{
			return m_End;
		}

	private:
		IteratorT m_Begin;
		IteratorT m_End;
	};
}
