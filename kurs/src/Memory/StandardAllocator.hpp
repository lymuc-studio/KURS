#pragma once

#include "Memory/Allocator.hpp"

namespace kurs
{
	class StandardAllocator : public Allocator
	{
	public:
		StandardAllocator(const AllocatorSpecification& specification);

		StandardAllocator(const StandardAllocator& other) = delete; 

		AllocatorSpecification GetSpecification() const override;

		void* Allocate() override;
		void Free(void* mem) override;

	private:
		bool ValidateSpecification() const;

		std::uintptr_t AlignAddress(std::uintptr_t address) const;
		void* AlignPointer(void* ptr) const;

		AllocatorSpecification m_Specification;
	};
}
