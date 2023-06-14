#include "Asset/AssetRegistry.hpp"

namespace kurs
{
	AssetRegistry::AssetRegistry(const AssetRegistryConfig& config)
		: m_Config(config)
	{
	}

	void AssetRegistry::CollectGarbage()
	{
		for (auto it = m_Assets.begin(); it != m_Assets.end();)
		{
			// Erase if the registry is the only entity that references the asset.
			if (it->second.use_count() == 1)
			{
				it = m_Assets.erase(it);
				continue;
			}
			it++;
		}
	}

	std::string AssetRegistry::GetFullPath(std::string_view fileName) const
	{
		namespace fs = std::filesystem;

		fs::path assetPath = m_Config.AssetFolderPath;
		assetPath /= fileName;
	
		return fs::absolute(assetPath).string();
	}
}
