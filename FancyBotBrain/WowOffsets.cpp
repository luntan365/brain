#include "stdafx.h"
#include "WowOffsets.h"

PVOID AddOffset(PVOID p, int offset)
{
	return (PVOID)((uintptr_t)p + offset);
}
