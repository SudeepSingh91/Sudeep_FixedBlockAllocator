#ifndef FIXEDBLOCKALLOCATOR_H
#define FIXEDBLOCKALLOCATOR_H

#include <stdint.h>

class BitArray;

class FixedBlockAllocator
{
public:
	static FixedBlockAllocator* Create(const size_t i_numBlocks, const size_t i_blockSize);
	void Destroy();

	void* Alloc();
	void Free(void* const i_memoryAddr);

	inline bool Contains(void* const i_memoryAddr) const;
	size_t GetTotalFreeMemory() const;

private:
	FixedBlockAllocator(void* const i_memoryAddr, const size_t i_numBlocks, const size_t i_blockSize);
	FixedBlockAllocator(const FixedBlockAllocator& i_fixedBlockAllocator);
	FixedBlockAllocator operator= (const FixedBlockAllocator& i_fixedBlockAllocator);

	~FixedBlockAllocator();

	inline static bool IsPowerOfTwo(const unsigned int i_value);

	inline void* RoundUp(void* const i_memoryAddr, const unsigned int i_align);
	inline void* RoundDown(void* const i_memoryAddr, const unsigned int i_align);

	bool IsValidBlockToFree(void* const i_memoryAddr);

	const size_t m_numBlocks;
	const size_t m_blockSize;
	void* const m_heapBase;
	BitArray* m_bitArray;

#if _DEBUG
	static const uint8_t m_gaurdBandSize;
	static const uint8_t m_gaurdBandValue;
#endif
};

#include "FixedBlockAllocator-inl.h"

#endif