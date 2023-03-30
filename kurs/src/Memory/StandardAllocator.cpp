#include "Memory/StandardAllocator.hpp"

#include "Debug/Assertion/Assert.hpp"
#include "Debug/Logging/Logger.hpp"

namespace kurs
{
	StandardAllocator::StandardAllocator(
		const AllocatorSpecification& specification
	)
		: m_Specification(specification)
	{
		KURS_ASSERT(
			ValidateSpecification(), 
			"Invalid StandardAllocator specs (alignment must be a power of 2)"
		);
	
		KURS_LOG(
			Debug,
			"StandardAllocator instance created: block size - %zu bytes,"
			" alignment - %zu bytes",
			m_Specification.BlockSize,
			m_Specification.Alignment
		);
	}
	
	AllocatorSpecification StandardAllocator::GetSpecification() const
	{
		return m_Specification;
	}

	void* StandardAllocator::Allocate()
	{	
		/// TODO: Add header and footer with magic number to check for memory block 
		/// corruption

		// Store pointer to the original block in the beginning
		constexpr std::size_t headerSize = sizeof(void*);
		std::size_t offset = headerSize + m_Specification.Alignment - 1;
		std::size_t worstSize = offset + m_Specification.BlockSize;

		KURS_LOG(
			Debug, 
			"StandardAllocator trying to allocate %zu additional bytes",
			worstSize
		);
		
		std::byte* mem = new (std::nothrow) std::byte[worstSize]{};
		KURS_ASSERT(mem, "Allocation of %zu additional bytes failed", worstSize);
		KURS_LOG(Debug, "mem allocated: %p", mem);

		std::byte* userdata = reinterpret_cast<std::byte*>(
			AlignPointer(mem + offset)
		);
		KURS_LOG(Debug, "mem (aligned userdata): %p", userdata);

		reinterpret_cast<std::byte**>(userdata)[-1] = mem;
	
		return userdata;
	}

	void StandardAllocator::Free(void* mem)
	{
		if (!mem)
		{
			KURS_LOG(Warn, "Trying to free nullptr");
			return;
		}


		KURS_LOG(Debug, "Freeing mem: %p", mem);
		
		std::byte* original = reinterpret_cast<std::byte**>(mem)[-1];
		KURS_LOG(Debug, "Freeing mem (original): %p", original);

		delete[] original;
	}

	bool StandardAllocator::ValidateSpecification() const
	{
		return m_Specification.Alignment % 2 == 0
			&& m_Specification.BlockSize;
	}

	std::uintptr_t StandardAllocator::AlignAddress(std::uintptr_t address) const
	{
		std::size_t alignment = m_Specification.Alignment;
		std::size_t increment = alignment - address % alignment;
		std::uintptr_t resultAddr = address + increment;

		return resultAddr;
	}

	void* StandardAllocator::AlignPointer(void* ptr) const
	{
		return reinterpret_cast<void*>(
			AlignAddress(reinterpret_cast<std::uintptr_t>(ptr))
		);
	}
}
