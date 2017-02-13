#include "../DebugFunctions/DebugFunctions.h"

inline bool FixedBlockAllocator::IsPowerOfTwo(const unsigned int i_value)
{
	return i_value && !(i_value & (i_value - 1));
}

inline void* FixedBlockAllocator::RoundUp(void* const i_memoryAddr, const unsigned int i_alignment)
{
	assert(i_memoryAddr != nullptr);
	assert(IsPowerOfTwo(i_alignment));

	return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(i_memoryAddr) + (i_alignment - 1)) & ~uintptr_t(i_alignment - 1));
}

inline void* FixedBlockAllocator::RoundDown(void* const i_memoryAddr, const unsigned int i_alignment)
{
	assert(i_memoryAddr != nullptr);
	assert(IsPowerOfTwo(i_alignment));

	return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(i_memoryAddr) & ~uintptr_t((i_alignment - 1)));
}

inline bool FixedBlockAllocator::Contains(void* const i_memoryAddr) const
{
	assert(i_memory != nullptr);

	return (i_memory >= m_heapBase) && (i_memory < (reinterpret_cast<uint8_t*>(m_heapBase) + m_heapSize));
}