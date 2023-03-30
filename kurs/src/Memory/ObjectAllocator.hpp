#pragma once

#include "kurspch.hpp"

#include "Memory/Allocator.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Assertion/Assert.hpp"

#include <new>
#include <type_traits>

namespace kurs
{
	template<typename T>
	class ObjectAllocator
	{
	public:
		static_assert(!std::is_void_v<T>, "Allocation of void is not allowed");
		static_assert(!std::is_reference_v<T>, "No references allowed");

		using ValueType = T;
		using PointerType = ValueType*;
		using SharedPointerType = std::shared_ptr<ValueType>;

		ObjectAllocator(std::unique_ptr<Allocator> placeAllocator)
			: m_PlaceAllocator(std::move(placeAllocator))
		{
			KURS_ASSERT(m_PlaceAllocator, "No allocator provided");

			KURS_ASSERT(
				ValidatePlaceAllocatorSpecification(), 
				"No suitable place allocator provided"
			);
		}

		template<typename... ArgsT>
		PointerType CreateInstance(ArgsT&&... args)
		{
			void* place = m_PlaceAllocator->Allocate();
			return new (place) ValueType(std::forward<ArgsT>(args)...);
		}

		template<typename... ArgsT>
		SharedPointerType CreateInstanceShared(ArgsT&&... args)
		{
			PointerType instance = CreateInstance(std::forward<ArgsT>(args)...);
			return SharedPointerType(
				instance, 
				[this](PointerType inst)
				{
					DestroyInstance(inst);
				}
			);
		}

		void DestroyInstance(PointerType instance)
		{
			if (instance)
			{
				instance->~ValueType();
				m_PlaceAllocator->Free(instance);
			}
		}

	private:
		bool ValidatePlaceAllocatorSpecification() const
		{
			constexpr std::size_t placeSize = sizeof(ValueType);
			constexpr std::size_t alignment = alignof(ValueType);
		
			AllocatorSpecification spec = m_PlaceAllocator->GetSpecification();

			return spec.BlockSize == placeSize 
				&& spec.Alignment == alignment;
		}

		std::unique_ptr<Allocator> m_PlaceAllocator = nullptr;
	};	
}
