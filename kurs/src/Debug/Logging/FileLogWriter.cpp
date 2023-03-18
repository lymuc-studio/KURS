#include "Debug/Logging/FileLogWriter.hpp"

#include <ctime>

namespace kurs
{
	FileLogWriter::FileLogWriter(std::string_view fileName)
		: m_Out(fileName.data(), std::ios::app)
	{
		WriteDate();
	}

	FileLogWriter::~FileLogWriter()
	{
		m_Out << "\n\n";
	}

	void FileLogWriter::WriteLine(std::string_view message)
	{
		if (message.size())
		{
			m_Out << message << std::endl;
		}
	}

	void FileLogWriter::WriteDate()
	{
		std::time_t time = std::time(nullptr);

		m_Out << std::ctime(&time) << "\n";
	}
}
