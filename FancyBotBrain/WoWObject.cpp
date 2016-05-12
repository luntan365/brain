#include "stdafx.h"
#include "WoWObject.h"

#include <hadesmem/read.hpp>
#include "WowOffsets.h"

WoWObject::WoWObject()
{
}


WoWObject::~WoWObject()
{
}

WoWObject WoWObject::Read(void* pObject)
{
    WoWObject obj;
    Read(&obj, pObject);
    return obj;
}

void WoWObject::Read(WoWObject* pObject, void* address)
{
    pObject->mAddress = address;
    ReadOffsetInto(AddOffset(address, OBJ_TYPE), &pObject->mType);
	ReadOffsetInto(AddOffset(address, GUID_OFFSET), &pObject->mGUID);
}

void* WoWObject::GetDataPointer(void* pObject)
{
    void* pDescriptor;
    ReadOffsetInto(AddOffset(pObject, DATA_PTR_OFFSET), &pDescriptor);
    return pDescriptor;
}

boost::optional<WoWObject> WoWObject::GetByGUID(uint64_t guid)
{
    auto fn = (ObjectManagerGetObjectPtr)GetObjectPtrAddress;
    auto pObject = fn(guid);
    if (pObject == nullptr)
    {
        return boost::none;
    }
    else
    {
        return boost::make_optional(WoWObject::Read(pObject));
    }
}

uint64_t WoWObject::GetGUID() const
{
	return mGUID;
}


const Position& WoWObject::GetPosition() const
{
    return mPosition;
}

WoWObjectType WoWObject::GetType() const
{
    return mType;
}

void* WoWObject::GetAddress() const
{
    return mAddress;
}

const WoWUnit* WoWObject::ToUnit() const
{
    if (GetType() == OT_UNIT || GetType() == OT_PLAYER)
    {
        return (WoWUnit*)this;
    }
    return nullptr;
}
