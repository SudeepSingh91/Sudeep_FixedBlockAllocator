#ifndef FIXEDBLOCKALLOCATOR_H
#define FIXEDBLOCKALLOCATOR_H

class FixedBlockAllocator
{
public:
	static FixedBlockAllocator* Create(void* const i_memoryAddr, size_t i_memorySize, const size_t i_blockSize);
	void Destroy();

	void* Alloc();
	void Free(void* const i_memoryAddr);

	inline bool Contains(void* const i_memoryAddr) const;
	bool IsAllocated(void* const i_memoryAddr) const;
	size_t GetTotalFreeMemory() const;

private:
	FixedBlockAllocator(void* const i_memoryAddr, const size_t i_memorysize, const size_t i_blockSize);
	FixedBlockAllocator(const FixedBlockAllocator& i_fixedBlockAllocator);
	FixedBlockAllocator operator= (const FixedBlockAllocator& i_fixedBlockAllocator);

	~FixedBlockAllocator();

	inline bool IsPowerOfTwo(const unsigned int i_value);
	inline void* RoundUp(void* const i_memoryAddr, const unsigned int i_align);
	inline void* RoundDown(void* const i_memoryAddr, const unsigned int i_align);

	bool IsValidBlockToFree(void* const i_memoryAddr);

	const size_t m_heapSize;
	const size_t m_blockSize;
	void* const m_heapBase;
	void* m_allocatedList;
	void* m_freeList;

#if _DEBUG
	static const uint8_t m_gaurdBandSize;
	static const uint8_t m_gaurdBandValue;
#endif
};

#include "FixedBlockAllocator-inl.h"

#endif