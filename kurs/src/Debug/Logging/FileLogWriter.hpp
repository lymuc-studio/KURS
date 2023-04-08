#pragma once

#include "Debug/Logging/Logger.hpp"

#include <fstream>
#include <mutex>

namespace kurs
{
	class FileLogWriter : public ILogWriter
	{
	public:
		FileLogWriter(std::string_view fileName = "kurs.log");
		~FileLogWriter();

		void WriteLine(std::string_view message) override;

	private:
		void WriteDate();

		std::ofstream m_OutFile;
		std::mutex m_OutFileMutex;
	};
}
