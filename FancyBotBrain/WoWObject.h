#pragma once

#include <boost/optional.hpp>

#include "Position.h"
#include "Util.h"

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

class WoWUnit;

class WoWObject
{
public:
	static const uintptr_t DATA_PTR_OFFSET = 0x8;

    static const uintptr_t OBJ_TYPE = 0x14;
    static const uintptr_t OBJ_MOVEMENT = 0x9A8;
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

    void* GetAddress() const;

	uint64_t GetGUID() const;

    const Position& GetPosition() const;

    WoWObjectType GetType() const;

    const WoWUnit* ToUnit() const;

protected:
    static void* GetDataPointer(void* pObject);

protected:
    void* mAddress;
    uint64_t mGUID;
    WoWObjectType mType;
    Position mPosition;
	float mRotation;
	std::string mName;
};
