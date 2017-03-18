#include "FixedBlockAllocator/FixedBlockAllocator.h"

#if _DEBUG
#include <crtdbg.h>
#endif

#include <malloc.h>

void main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(79);
#endif
	
	const size_t num = 60;
	const size_t size = 4;
	FixedBlockAllocator* m_fixedAllocator = FixedBlockAllocator::Create(num, size);

	m_fixedAllocator->Destroy();
}