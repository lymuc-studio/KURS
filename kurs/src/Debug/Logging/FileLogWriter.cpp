#include "Debug/Logging/FileLogWriter.hpp"

#include <ctime>

#include <mutex>

namespace kurs
{
	FileLogWriter::FileLogWriter(std::string_view fileName)
		: m_OutFile(fileName.data(), std::ios::app)
	{
		WriteDate();
	}

	FileLogWriter::~FileLogWriter()
	{
		std::lock_guard outFileGuard(m_OutFileMutex);
		m_OutFile << "\n\n";
	}

	void FileLogWriter::WriteLine(std::string_view message)
	{
		if (message.size())
		{
			std::lock_guard outFileGuard(m_OutFileMutex);
			m_OutFile << message << std::endl;
		}
	}

	void FileLogWriter::WriteDate()
	{
		std::time_t time = std::time(nullptr);

		std::lock_guard outFileGuard(m_OutFileMutex);
		m_OutFile << std::ctime(&time) << "\n";
	}
}
