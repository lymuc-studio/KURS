#pragma once

#include "kurspch.hpp"

#include "Core/TypeID.hpp"
#include "Core/TypeName.hpp"

namespace kurs
{
	struct AssetBase
	{
		virtual ~AssetBase() = default;

		virtual TypeID GetTypeID() const = 0;
		virtual std::string GetTypeName() const = 0;
	};

	template<typename DerivedT>
	struct Asset : public AssetBase
	{
		TypeID GetTypeID() const override final
		{
			return getTypeID<DerivedT>();
		}

		std::string GetTypeName() const override final
		{
			return getTypeName<DerivedT>();
		}
	};

	template<typename AssetT>
	using AssetRef = std::shared_ptr<AssetT>;

	template<typename AssetT, typename... ArgsT>
	AssetRef<AssetT> MakeAssetRef(ArgsT&&... args)
	{
		return std::make_shared<AssetT>(std::forward<ArgsT>(args)...);
	}
}

namespace kurs::detail
{
	template<typename AssetT>
	AssetRef<AssetT> asset_cast(AssetRef<AssetBase> asset)
	{
		if (!asset)
		{
			return nullptr;
		}

		if (asset->GetTypeID() != getTypeID<AssetT>())
		{
			return nullptr;
		}

		return std::static_pointer_cast<AssetT>(asset);
	}
}
