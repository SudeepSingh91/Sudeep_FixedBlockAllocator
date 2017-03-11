#include "FixedBlockAllocator/FixedBlockAllocator.h"

#include <malloc.h>

void main()
{
	void* pmemory = _aligned_malloc(128, 64);
	
	FixedBlockAllocator* fixedAllocator = FixedBlockAllocator::Create(pmemory, 128, 4);

	float* m1 = static_cast<float*>(fixedAllocator->Alloc());
	*m1 = 4.0f;
	size_t total1 = fixedAllocator->GetTotalFreeMemory();
	float* m2 = static_cast<float*>(fixedAllocator->Alloc());
	*m2 = 4.0f;
	size_t total2 = fixedAllocator->GetTotalFreeMemory();
	float* m3 = static_cast<float*>(fixedAllocator->Alloc());
	*m3 = 4.0f;
	size_t total3 = fixedAllocator->GetTotalFreeMemory();
	float* m4 = static_cast<float*>(fixedAllocator->Alloc());
	*m4 = 4.0f;
	size_t total4 = fixedAllocator->GetTotalFreeMemory();
	float* m5 = static_cast<float*>(fixedAllocator->Alloc());
	*m5 = 4.0f;
	size_t total5 = fixedAllocator->GetTotalFreeMemory();
	float* m6 = static_cast<float*>(fixedAllocator->Alloc());
	*m6 = 4.0f;
	size_t total6 = fixedAllocator->GetTotalFreeMemory();
	float* m7 = static_cast<float*>(fixedAllocator->Alloc());
	size_t total7 = fixedAllocator->GetTotalFreeMemory();

	/*bool b1 = fixedAllocator->Contains(m2);
	bool b2 = fixedAllocator->Contains(m1 - 100);
	bool b3 = fixedAllocator->IsAllocated(m3);*/

	//fixedAllocator->Free(m2);
	//fixedAllocator->Free(m4);

	fixedAllocator->Destroy();
}