#pragma once

#include <hadesmem/process.hpp>

#include <unordered_map>
#include "WoWObject.h"
#include "WoWPlayer.h"

class WoWObjectManager
{
public:
	typedef std::unordered_map<uint64_t, const WoWObject> ObjectMap;

    typedef bool(__fastcall *EnumCallback)(uint64_t guid, void* arg);

    typedef bool(__fastcall *ObjectManagerEnumVisibleObjects)(EnumCallback callback, void* arg);

    typedef uint64_t(*ObjectManagerGetActivePlayerGUID)();

public:
    static const uintptr_t EnumVisableObjectsAddress = 0x468380;
    static const uintptr_t GetActivePlayerGUIDAddress = 0x468550;

public:
	WoWObjectManager();

	~WoWObjectManager();

	static void Read(WoWObjectManager* pManager, PVOID basePtr);

    void ReadObject(PVOID objPtr);

    uint64_t GetActivePlayerGUID();

    WoWPlayer GetPlayer();

    boost::optional<WoWObject> GetObject(uint64_t guid);

    void NewObject(uint64_t guid);

    void ClearObjects();

//private:
	ObjectMap mObjects;
};

