#pragma once

#include <boost/optional.hpp>

#include "Position.h"

class WoWObject
{
public:
	static const uintptr_t DATA_PTR_OFFSET = 0x8;

    static const uintptr_t OBJ_POS_X = 0x9B8;
    static const uintptr_t OBJ_POS_Y = 0x9BC;
    static const uintptr_t OBJ_POS_Z = 0x9C0;
    static const uintptr_t OBJ_ROTATION = 0x9C0;

    static const uintptr_t GUID_OFFSET = 0x30;

    static const uintptr_t GetObjectPtrAddress = 0x468460;

    typedef void*(__stdcall *ObjectManagerGetObjectPtr)(uint64_t, uint32_t);

public:
	WoWObject();

	~WoWObject();

	static WoWObject Read(void* pObject);

    static void Read(WoWObject* pObject, void* address);

    static boost::optional<WoWObject> GetByGUID(uint64_t guid);

	uint64_t GetGUID() const;

protected:
    static void* GetDataPointer(void* pObject);

public:
    uint64_t mGUID;
    Position mPosition;
	float mRotation;
	std::string mName;
};

