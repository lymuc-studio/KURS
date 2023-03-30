#pragma once

#include "kurspch.hpp"

namespace kurs::detail
{
	struct AssertData
	{
		std::string FileName;
		std::uint32_t LineNumber = 0;
		std::string ScopeName;
		std::string Message;
	};

	class AssertImpl
	{
	public:
		AssertImpl(
			std::string_view fileName,
			std::uint32_t lineNumber,
			std::string_view scopeName,
			std::string_view format = "",
			...
		);

		const AssertImpl& LogMessage() const;
		const AssertImpl& DisplayMessage() const;
		bool DebugBreak() const;

	private:
		AssertData m_Data;
	};
}
