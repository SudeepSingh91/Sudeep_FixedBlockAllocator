#include "../DebugFunctions/DebugFunctions.h"

inline bool FixedBlockAllocator::IsPowerOfTwo(const size_t i_value)
{
	return i_value && !(i_value & (i_value - 1));
}

inline bool FixedBlockAllocator::Contains(void* const i_memoryAddr) const
{
	assert(i_memoryAddr != nullptr);

#if _DEBUG
	return (i_memoryAddr >= m_heapBase) && (i_memoryAddr < (reinterpret_cast<uint8_t*>(m_heapBase) + (m_numBlocks * (m_blockSize + (2 * m_gaurdBandSize)))));
#else
	return (i_memoryAddr >= m_heapBase) && (i_memoryAddr < (reinterpret_cast<uint8_t*>(m_heapBase) + (m_numBlocks * m_blockSize)));
#endif
}