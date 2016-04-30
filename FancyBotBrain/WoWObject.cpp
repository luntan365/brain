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
	ReadOffsetInto(AddOffset(address, GUID_OFFSET), &pObject->mGUID);
    ReadOffsetInto(AddOffset(address, OBJ_POS_X), &pObject->mPosition.x);
    ReadOffsetInto(AddOffset(address, OBJ_POS_Y), &pObject->mPosition.y);
    ReadOffsetInto(AddOffset(address, OBJ_POS_Z), &pObject->mPosition.z);
    ReadOffsetInto(AddOffset(address, OBJ_ROTATION), &pObject->mRotation);
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
    auto pObject = fn(guid, 0xFFFFFFFF);
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
