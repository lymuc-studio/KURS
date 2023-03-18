#pragma once

#include "Debug/Logging/Logger.hpp"

namespace kurs
{
	class ConsoleLogWriter : public ILogWriter
	{
	public:
		void WriteLine(std::string_view message) override;
	};
}
