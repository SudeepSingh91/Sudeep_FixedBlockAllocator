#include "FixedBlockAllocator.h"

#include <new>

#include "../DebugFunctions/DebugFunctions.h"
#include "../BitArray/BitArray.h"

#ifdef _DEBUG
	const uint8_t FixedBlockAllocator::m_gaurdBandSize = sizeof(void*);
	const uint8_t FixedBlockAllocator::m_gaurdBandValue = 0xFD;
#endif

FixedBlockAllocator* FixedBlockAllocator::Create(const size_t i_numBlocks, const size_t i_blockSize)
{
	assert(i_numBlocks > 0);
	assert(IsPowerOfTwo(i_blockSize));

	const size_t cacheLineWidth = 64;

#if _DEBUG
	void* FixedBlockAllocatorMem = _aligned_malloc((sizeof(FixedBlockAllocator) + (i_numBlocks * (i_blockSize + (2 * m_gaurdBandSize)))), cacheLineWidth);
#else
	void* FixedBlockAllocatorMem = _aligned_malloc((sizeof(FixedBlockAllocator) + (i_numBlocks * i_blockSize)), cacheLineWidth);
#endif

	void* heapBase = static_cast<uint8_t*>(FixedBlockAllocatorMem) + sizeof(FixedBlockAllocator);

	FixedBlockAllocator* m_fixedBlockAllocator = new (FixedBlockAllocatorMem) FixedBlockAllocator(heapBase, i_numBlocks, i_blockSize);
	
	DEBUG_PRINT("Created FixedBlockAllocator with number of blocks %d and size of blocks %d", i_numBlocks, i_blockSize);

	return m_fixedBlockAllocator;
}

void FixedBlockAllocator::Destroy()
{
	void* memoryToFree = static_cast<uint8_t*>(m_heapBase) - sizeof(FixedBlockAllocator);
	this->~FixedBlockAllocator();
	_aligned_free(memoryToFree);
}

FixedBlockAllocator::FixedBlockAllocator(void* const i_memoryAddr, const size_t i_numBlocks, const size_t i_blockSize)
	: m_numBlocks(i_numBlocks), m_blockSize(i_blockSize), m_heapBase(i_memoryAddr), m_bitArray(nullptr)
{
	assert(i_memoryAddr != nullptr);
	assert(i_numBlocks > 0);
	assert(IsPowerOfTwo(i_blockSize));

	DEBUG_PRINT("Called constructor of Fixed Block Allocator");

	const bool isBitArrayClear = true;
	m_bitArray = new BitArray(i_numBlocks, isBitArrayClear);

#ifdef _DEBUG

	uint8_t* blockAddr = static_cast<uint8_t*>(m_heapBase);
	for (size_t i = 0; i < i_numBlocks; i++)
	{
		for (short i = 0; i < m_gaurdBandSize; i++)
		{
			*blockAddr = m_gaurdBandValue;
			blockAddr++;
		}

		blockAddr = blockAddr + m_blockSize;

		for (short i = 0; i < m_gaurdBandSize; i++)
		{
			*blockAddr = m_gaurdBandValue;
			blockAddr++;
		}
	}
#endif
}

FixedBlockAllocator::~FixedBlockAllocator()
{

}

void* FixedBlockAllocator::Alloc()
{
	size_t freeBit = 0;

	if(m_bitArray->GetFirstClearbit(freeBit))
	{
		m_bitArray->SetBit(freeBit);

#if _DEBUG
		uint8_t* addrToReturn = static_cast<uint8_t*>(m_heapBase) + ((freeBit - 1) * (m_blockSize + (2 * m_gaurdBandSize))) + m_gaurdBandSize;
#elif
		uint8_t* addrToReturn = static_cast<uint8_t*>(m_heapBase) + ((freeBit - 1) * m_blockSize);
#endif

		DEBUG_PRINT("Allocated block from free list");

		return addrToReturn;
	}
	else
	{
		DEBUG_PRINT("No free blocks in the Fixed Block Allocator of size %d", m_blockSize);
		
		return nullptr;
	}
}

void FixedBlockAllocator::Free(void* const i_memoryAddr)
{
	assert(i_memoryAddr != nullptr);
	assert(IsValidBlockToFree(i_memoryAddr));

#if _DEBUG
	size_t bitNumber = ((static_cast<uint8_t*>(i_memoryAddr) - static_cast<uint8_t*>(m_heapBase)) / (m_blockSize + (2 * m_gaurdBandSize))) + 1;
#elif
	size_t bitNumber = ((static_cast<uint8_t*>(i_memoryAddr) - static_cast<uint8_t*>(m_heapBase)) / m_blockSize) + 1;
#endif

	if (m_bitArray->IsBitSet(bitNumber))
	{
		m_bitArray->ClearBit(bitNumber);

		DEBUG_PRINT("Freed %p", i_memoryAddr);
	}
	else
	{
		DEBUG_PRINT("%p block is already free", i_memoryAddr);
	}	
}


size_t FixedBlockAllocator::GetTotalFreeMemory() const
{
	size_t totalFreeBlockSize = 0;
	for (size_t i = 1; i <= m_numBlocks; i++)
	{
		if (m_bitArray->IsBitClear(i))
		{
			totalFreeBlockSize += m_blockSize;
		}
	}

	DEBUG_PRINT("Total free memory has size %d", totalFreeBlockSize);

	return totalFreeBlockSize;
}

bool FixedBlockAllocator::IsValidBlockToFree(void* const i_memoryAddr)
{
	uint8_t* heapIter = static_cast<uint8_t*>(m_heapBase);
	
#if _DEBUG
	uint8_t* heapEnd = static_cast<uint8_t*>(m_heapBase) + (m_numBlocks * (m_blockSize + (2 * m_gaurdBandSize)));
#elif
	uint8_t* heapEnd = static_cast<uint8_t*>(m_heapBase) + (m_numBlocks * m_blockSize);
#endif

	while (heapIter < heapEnd)
	{
#ifdef _DEBUG
		heapIter = heapIter + m_gaurdBandSize;
#endif
		
		if (heapIter == i_memoryAddr)
		{
			DEBUG_PRINT("%p is a valid block to free", i_memoryAddr);
			
			return true;
		}
		
		heapIter = heapIter + m_blockSize;

#ifdef _DEBUG
		heapIter = heapIter + m_gaurdBandSize;
#endif
	}

	DEBUG_PRINT("%p is not a valid block to free", i_memoryAddr);

	return false;
}