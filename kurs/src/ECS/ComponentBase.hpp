#pragma once

#include "kurspch.hpp"

namespace kurs::detail
{
	template<typename ComponentT>
	struct ComponentTraits
	{
		static_assert(
			!std::is_reference_v<ComponentT>
			&& !std::is_pointer_v<ComponentT>,
			"Component type must not be defined as a reference or a pointer: "
			"it is required to be a value type"
		);

		static_assert(
			!std::is_const_v<ComponentT>,
			"Component type must not be a const type"
		);

		using ValueType = ComponentT;
		using ReferenceType = ValueType&;
		using ConstReferenceType = const ValueType&;
		using PointerType = ValueType*;
		using ConstPointerType = const ValueType*;

		static constexpr std::size_t c_SizeOf = sizeof(ValueType);
		static constexpr std::size_t c_Alignment = alignof(ValueType);

		struct alignas(c_Alignment) Storage
		{
		public:
			PointerType GetPtr()
			{
				return reinterpret_cast<PointerType>(m_Bytes);
			}

			ConstPointerType GetPtr() const
			{
				return reinterpret_cast<ConstPointerType>(m_Bytes);
			}

			ReferenceType Get()
			{
				return reinterpret_cast<ReferenceType>(*m_Bytes);
			}

			ConstReferenceType Get() const
			{
				return reinterpret_cast<ConstReferenceType>(*m_Bytes);
			}

			template<typename... ArgsT>
			ReferenceType Construct(ArgsT&&... args)
			{
				return *new (m_Bytes) ValueType{ std::forward<ArgsT>(args)... };
			}

			void Destruct()
			{
				Get().~ValueType();
			}

		private:
			std::byte m_Bytes[c_SizeOf]{};
		};
	};
}
