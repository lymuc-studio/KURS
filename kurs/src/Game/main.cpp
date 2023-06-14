#include <fstream>

#include <SDL3/SDL.h>

#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/ConsoleLogWriter.hpp"
#include "Debug/Logging/FileLogWriter.hpp"
#include "Debug/Logging/TimedLogFormatter.hpp"

#include "Asset/AssetBase.hpp"
#include "Asset/AssetRegistry.hpp"
#include "Asset/AssetSerializer.hpp"

#include "Game/App.hpp"

class TextAsset : public kurs::Asset<TextAsset>
{
public:
	explicit TextAsset(std::string&& buffer)
		: m_Buffer(std::move(buffer))
	{
	}

	~TextAsset() override
	{
		std::cout << "Text asset destroyed\n";
	}

	std::string_view GetBuffer() const
	{
		return m_Buffer;
	}

private:
	std::string m_Buffer;
};

class TextAssetSerializer : public kurs::AssetSerializer<TextAsset>
{
public:
	kurs::AssetRef<kurs::AssetBase> LoadFromFile(std::string_view fileName) override
	{
		std::ifstream fin(fileName.data());
		if (!fin.is_open())
		{
			return nullptr;
		}
	
		std::string buffer;
		std::string line;

		while (std::getline(fin, line))
		{
			buffer += line;
			buffer += '\n';
		}
	
		return kurs::MakeAssetRef<TextAsset>(std::move(buffer));
	}
};

int main()
{
	kurs::LogLevel verbosity = kurs::LogLevel_Debug;

	kurs::Logger::Get()
		.SetFormatter(std::make_unique<kurs::TimedLogFormatter>(verbosity))
		.AddWriter(std::make_unique<kurs::ConsoleLogWriter>())
		.AddWriter(std::make_unique<kurs::FileLogWriter>("kurs.log"));

	SDL_Init(SDL_INIT_EVERYTHING);
	{
		kurs::App app;
		app.Run();
	}
	SDL_Quit();
}
