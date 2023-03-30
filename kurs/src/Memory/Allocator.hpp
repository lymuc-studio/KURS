#pragma once

#include "kurspch.hpp"

namespace kurs
{
	struct AllocatorSpecification
	{
		std::size_t BlockSize = sizeof(std::byte);
		std::size_t Alignment = alignof(std::byte);
	};

	class Allocator
	{
	public:
		virtual ~Allocator() = default;

		virtual AllocatorSpecification GetSpecification() const = 0;
	
		virtual void* Allocate() = 0;
		virtual void Free(void* mem) = 0; 
	};
}
