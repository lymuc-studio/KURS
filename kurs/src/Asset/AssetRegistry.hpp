#pragma once

#include "kurspch.hpp"

#include "Core/TypeID.hpp"
#include "Core/TypeName.hpp"

#include "Asset/AssetBase.hpp"
#include "Asset/AssetSerializer.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Assertion/Assert.hpp"

namespace kurs
{
	struct AssetRegistryConfig
	{
		std::string AssetFolderPath = ".";
	};

	class AssetRegistry
	{
	public:
		explicit AssetRegistry(const AssetRegistryConfig& config);
		
		template<typename AssetSerializerT, typename... ArgsT>
		void RegisterSerializer(ArgsT&&... args)
		{
			using AssetType = typename AssetSerializerT::AssetType;

			TypeID assetTypeID = getTypeID<AssetType>();
		
			if (assetTypeID >= m_Serializers.size())
			{
				m_Serializers.resize(assetTypeID + 1);
			}
		
			m_Serializers[assetTypeID] = std::make_unique<AssetSerializerT>(
				std::forward<ArgsT>(args)...
			);
		
			KURS_LOG(
				Debug, 
				"'%s' asset serializer registered",
				getTypeName<AssetSerializerT>().data()
			);
		}

		template<typename AssetT>
		AssetRef<AssetT> LoadFromFile(std::string_view fileName)
		{
			std::string fullPath = GetFullPath(fileName);

			if (AssetRef<AssetT> asset = TryLoadCachedAsset<AssetT>(fullPath))
			{
				return asset;
			}
		
			AssetSerializerBase* serializer = PickAssetSerializer<AssetT>();
			if (!serializer)
			{
				KURS_LOG(
					Error,
					"Could not load '%s' asset file ('%s'): asset serializer is not "
					"provided",
					getTypeName<AssetT>().data(),
					fullPath.data()
				);
				return nullptr;
			}
		
			AssetRef<AssetBase> assetInstance = serializer->LoadFromFile(
				fullPath
			);
		
			if (!assetInstance)
			{
				KURS_LOG(
					Error, 
					"Could not load '%s' asset from file ('%s')",
					getTypeName<AssetT>().data(),
					fullPath.data()
				);
				return nullptr;
			}
		
			return detail::asset_cast<AssetT>(
				m_Assets[fullPath] = assetInstance
			);
		}

		void CollectGarbage();

	private:
		std::string GetFullPath(std::string_view fileName) const;
		
		template<typename AssetT>
		AssetRef<AssetT> TryLoadCachedAsset(const std::string& fullAssetPath)
		{
			auto assetIterator = m_Assets.find(fullAssetPath);
			if (m_Assets.end() != assetIterator)
			{
				return detail::asset_cast<AssetT>(assetIterator->second);
			}

			return nullptr;
		}

		template<typename AssetT>
		AssetSerializerBase* PickAssetSerializer()
		{
			TypeID assetTypeID = getTypeID<AssetT>();
		
			if (assetTypeID >= m_Serializers.size())
			{
				return nullptr;
			}
		
			return m_Serializers[assetTypeID].get();
		}

		std::vector<std::unique_ptr<AssetSerializerBase>> m_Serializers;
		std::unordered_map<std::string, AssetRef<AssetBase>> m_Assets;
		AssetRegistryConfig m_Config;
	};
}
