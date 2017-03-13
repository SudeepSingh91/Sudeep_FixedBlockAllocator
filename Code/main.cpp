#include "FixedBlockAllocator/FixedBlockAllocator.h"

#include <malloc.h>

void main()
{
	const size_t num = 6;
	const size_t size = 4;
	FixedBlockAllocator* m_fixedAllocator = FixedBlockAllocator::Create(num, size);

	float* m1 = static_cast<float*>(m_fixedAllocator->Alloc());
	*m1 = 4.0f;
	size_t t1 = m_fixedAllocator->GetTotalFreeMemory();

	float* m2 = static_cast<float*>(m_fixedAllocator->Alloc());
	*m2 = 4.0f;
	size_t t2 = m_fixedAllocator->GetTotalFreeMemory();

	float* m3 = static_cast<float*>(m_fixedAllocator->Alloc());
	*m3 = 4.0f;
	size_t t3 = m_fixedAllocator->GetTotalFreeMemory();

	float* m4 = static_cast<float*>(m_fixedAllocator->Alloc());
	*m4 = 4.0f;
	size_t t4 = m_fixedAllocator->GetTotalFreeMemory();

	/*m_fixedAllocator->Free(m2);
	size_t t5 = m_fixedAllocator->GetTotalFreeMemory();

	m_fixedAllocator->Free(m3);
	size_t t6 = m_fixedAllocator->GetTotalFreeMemory();*/

	float* m5 = static_cast<float*>(m_fixedAllocator->Alloc());
	*m5 = 8.0f;

	float* m6 = static_cast<float*>(m_fixedAllocator->Alloc());
	*m6 = 8.0f;

	float* m7 = static_cast<float*>(m_fixedAllocator->Alloc());

	m_fixedAllocator->Destroy();
}