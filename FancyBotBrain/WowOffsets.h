#pragma once

#include <hadesmem/read.hpp>

#include "Global.h"
#include "WoWObjectManager.h"

#define DEFINE_STATIC_OFFSET(NAME, ADDRESS, TYPE) \
	struct NAME { \
		static const DWORD_PTR offsetAddress = ADDRESS; \
		static TYPE Read() \
		{ \
			return hadesmem::Read<TYPE>(GetThisProcess(), (PVOID)offsetAddress); \
		} \
	};

#define DEFINE_RELATIVE_OFFSET(PARENT_NAME, NAME, OFFSET, TYPE) \
	struct NAME { \
		static const DWORD_PTR offsetAddress = OFFSET; \
		static TYPE Read() \
		{ \
			return hadesmem::Read<TYPE>( \
				GetThisProcess(), \
				(PVOID)(offsetAddress + PARENT_NAME::offsetAddress)); \
		} \
	};

#define DEFINE_COMPLEX_STATIC_OFFSET(NAME, OFFSET, TYPE) \
	struct NAME { \
		static const DWORD_PTR offsetAddress = OFFSET; \
		static void Read(TYPE* pT) \
		{ \
			TYPE::Read(pT, (PVOID)OFFSET); \
		} \
	};

template <typename T>
void ReadOffsetInto(PVOID addr, T* out)
{
	*out = hadesmem::Read<T>(GetThisProcess(), addr);
}

PVOID AddOffset(PVOID p, int offset);

class Offsets
{
public:
	DEFINE_STATIC_OFFSET(IsInGame, 0x00B4B424, bool);

	DEFINE_COMPLEX_STATIC_OFFSET(ObjectManager, 0xB41414, WoWObjectManager);

	DEFINE_STATIC_OFFSET(CurrentTargetGUID, 0x74e2d8, uint64_t);
	DEFINE_STATIC_OFFSET(PlayerGUID, 0x741e30, uint64_t);
};

enum WoWObjectType
{
    OT_NONE = 0,
    OT_ITEM = 1,
    OT_CONTAINER = 2,
    OT_UNIT = 3,
    OT_PLAYER = 4,
    OT_GAMEOBJ = 5,
    OT_DYNOBJ = 6,
    OT_CORPSE = 7,
};
