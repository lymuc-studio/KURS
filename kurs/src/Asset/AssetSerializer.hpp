#pragma once

#include "kurspch.hpp"

#include "Core/TypeID.hpp"

#include "Asset/AssetBase.hpp"

namespace kurs
{
	struct AssetSerializerBase
	{
		virtual ~AssetSerializerBase() = default;
	
		virtual TypeID GetAssetTypeID() const = 0;
	
		virtual AssetRef<AssetBase> LoadFromFile(std::string_view fileName) = 0;
	};

	template<typename AssetT>
	struct AssetSerializer : public AssetSerializerBase
	{
		using AssetType = AssetT;

		TypeID GetAssetTypeID() const override final
		{
			return getTypeID<AssetT>();
		}
	};
}
