#include "FixedBlockAllocator.h"

#include <stdint.h>
#include <new>

#include "../DebugFunctions/DebugFunctions.h"

#ifdef _DEBUG
	const uint8_t FixedBlockAllocator::m_gaurdBandSize = sizeof(void*);
	const uint8_t FixedBlockAllocator::m_gaurdBandValue = 0xFD;
#endif

FixedBlockAllocator* FixedBlockAllocator::Create(void* i_memoryAddr, size_t i_memorySize, const size_t i_blockSize)
{
	assert(i_memoryAddr != nullptr);
	assert(i_blockSize > 0);
	assert(i_memorySize > (sizeof(FixedBlockAllocator) + (sizeof(void*) * 2) + i_blockSize));

	void* FixedBlockAllocatorMem = i_memoryAddr;
	i_memoryAddr = static_cast<uint8_t*>(i_memoryAddr) + sizeof(FixedBlockAllocator);
	i_memorySize -= sizeof(FixedBlockAllocator);

	FixedBlockAllocator* m_FixedBlockAllocator = new (FixedBlockAllocatorMem) FixedBlockAllocator(i_memoryAddr, i_memorySize, i_blockSize);
	
	DEBUG_PRINT("Created FixedBlockAllocator with size %d", i_memorySize);

	return m_FixedBlockAllocator;
}

void FixedBlockAllocator::Destroy()
{
	void* memoryToFree = static_cast<uint8_t*>(m_heapBase) - sizeof(FixedBlockAllocator);
	this->~FixedBlockAllocator();
	_aligned_free(memoryToFree);
}

FixedBlockAllocator::FixedBlockAllocator(void* const i_memoryAddr, const size_t i_memorysize, const size_t i_blockSize) 
	: m_heapSize(i_memorysize), m_blockSize(i_blockSize), m_heapBase(i_memoryAddr), m_allocatedList(nullptr), m_freeList(nullptr)
{
	assert(i_memoryAddr != nullptr);
	assert(i_blockSize > 0);
	assert(i_memorysize > ((sizeof(void*) * 2) + i_blockSize));

	DEBUG_PRINT("Called constructor of Fixed Block Allocator");

	void* firstBlock = static_cast<uint8_t*>(i_memoryAddr) + sizeof(void*);
	size_t numBytesToCheckAhead = (2 * i_blockSize) + sizeof(void*);

#ifdef _DEBUG
	firstBlock = firstBlock + m_gaurdBandSize;
	numBytesToCheckAhead = numBytesToCheckAhead + (3 * m_gaurdBandSize);
#endif

	uint8_t* iterBlock = static_cast<uint8_t*>(firstBlock);
	iterBlock = static_cast<uint8_t*>(RoundUp(iterBlock, i_blockSize));
	uint8_t* firstLinkNode = iterBlock - sizeof(void*);

#ifdef _DEBUG
	firstLinkNode = firstLinkNode - m_gaurdBandSize;
#endif

	m_freeList = firstLinkNode;

	while ((iterBlock + numBytesToCheckAhead) <= (static_cast<uint8_t*>(m_heapBase) + i_memorysize))
	{
		uint8_t** prevLinkNode = reinterpret_cast<uint8_t**>(iterBlock - sizeof(void*));
		uint8_t* nextLinkNode = iterBlock + i_blockSize + sizeof(void*);

#ifdef _DEBUG
		uint8_t* gaurdbandStart = iterBlock - m_gaurdBandSize;
		uint8_t* gaurdbandEnd = iterBlock + i_blockSize;

		for (uint8_t i = 0; i < m_gaurdBandSize; i++)
		{
			gaurdbandStart = m_gaurdBandValue;
			gaurdbandEnd = m_gaurdBandValue;
			gaurdbandStart++;
			gaurdbandEnd++;
		}

		prevLinkNode = reinterpret_cast<uint8_t**>(iterBlock - m_gaurdBandSize - sizeof(void*));
		nextLinkNode = nextLinkNode + (2 * m_gaurdBandSize);
#endif

		iterBlock = static_cast<uint8_t*>(RoundUp(nextLinkNode, i_blockSize));
		nextLinkNode = iterBlock - sizeof(void*);

#ifdef _DEBUG
		nextLinkNode = nextLinkNode - m_gaurdBandSize;
#endif

		*prevLinkNode = nextLinkNode;
	}

	uint8_t** endLinkNode = reinterpret_cast<uint8_t**>(iterBlock - sizeof(void*));

#ifdef _DEBUG
	endLinkNode = reinterpret_cast<uint8_t**>(iterBlock - sizeof(void*) - m_gaurdBandSize);
#endif

	*endLinkNode = nullptr;
}

FixedBlockAllocator::~FixedBlockAllocator()
{

}

void* FixedBlockAllocator::Alloc()
{
	if (m_freeList != nullptr)
	{
		void* iterFreeList = m_freeList;
		void** nextFreeList = reinterpret_cast<void**>(iterFreeList);
		m_freeList = *nextFreeList;

		if (m_allocatedList != nullptr)
		{
			m_allocatedList = iterFreeList;
		}
		else
		{
			*nextFreeList = m_allocatedList;
			m_allocatedList = iterFreeList;
		}

		uint8_t* addrToReturn = static_cast<uint8_t*>(iterFreeList) + sizeof(void*);

#ifdef _DEBUG
		addrToReturn = addrToReturn + m_gaurdBandSize;
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
	
	if (m_allocatedList != nullptr)
	{
		uint8_t** iterAllocList = reinterpret_cast<uint8_t**>(static_cast<uint8_t*>(i_memoryAddr) - sizeof(void*));

#ifdef _DEBUG
		*iterAllocList = *iterAllocList - m_gaurdBandSize;
#endif

		assert(IsValidBlockToFree(*iterAllocList));

		if (*iterAllocList != m_allocatedList)
		{
			uint8_t** prevAllocList = reinterpret_cast<uint8_t**>(static_cast<uint8_t*>(i_memoryAddr) - (2 * sizeof(void*)) - m_blockSize);

#ifdef _DEBUG
			*prevAllocList = *prevAllocList - (3 * m_gaurdBandSize);
#endif

			*prevAllocList = *iterAllocList;
		}
		else
		{
			m_allocatedList = *iterAllocList;
		}

		DEBUG_PRINT("Freed %p", i_memoryAddr);

		if (m_freeList == nullptr)
		{
			m_freeList = reinterpret_cast<uint8_t*>(iterAllocList);
		}
		else
		{
			*iterAllocList = static_cast<uint8_t*>(m_freeList);
			m_freeList = *iterAllocList;
		}
	}
	else
	{
		DEBUG_PRINT("There is no block to free in this fixed block allocator");
	}
}

bool FixedBlockAllocator::IsAllocated(void* const i_memoryAddr) const
{
	assert(i_memoryAddr != nullptr);

	void* iterAllocList = m_allocatedList;

	while (iterAllocList != nullptr)
	{
		uint8_t* memAddrAllocated = static_cast<uint8_t*>(iterAllocList) + sizeof(void*);

#ifdef _DEBUG
		memAddrAllocated = memAddrAllocated + m_gaurdBandSize;
#endif

		if (memAddrAllocated == i_memoryAddr)
		{
			DEBUG_PRINT("%p is allocated", i_memoryAddr);

			return true;
		}

		void** nextAllocList = reinterpret_cast<void**>(iterAllocList);
		iterAllocList = *nextAllocList;
	}

	DEBUG_PRINT("%p is not allocated", i_memoryAddr);

	return false;
}

size_t FixedBlockAllocator::GetTotalFreeMemory() const
{
	size_t totalFreeBlockSize = 0;
	void* iterFreeList = m_freeList;

	while (iterFreeList != nullptr)
	{
		totalFreeBlockSize += m_blockSize;

		void** nextFreeList = reinterpret_cast<void**>(iterFreeList);
		iterFreeList = *nextFreeList;
	}

	DEBUG_PRINT("Total free memory has size %d", totalFreeBlockSize);

	return totalFreeBlockSize;
}

bool FixedBlockAllocator::IsValidBlockToFree(void* const i_memoryAddr)
{
	assert(IsValidBlockToFree != nullptr);

	void* iterAllocList = m_allocatedList;

	while (iterAllocList != nullptr)
	{
		if (iterAllocList == i_memoryAddr)
		{
			DEBUG_PRINT("%p is a valid block to free", i_memoryAddr);
			
			return true;
		}
		
		void** nextAllocList = reinterpret_cast<void**>(iterAllocList);
		iterAllocList = *nextAllocList;
	}

	DEBUG_PRINT("%p is not a valid block to free", i_memoryAddr);

	return false;
}