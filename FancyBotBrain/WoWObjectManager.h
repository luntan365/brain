#pragma once

#include <unordered_map>
#include "WoWInventory.h"
#include "WoWObject.h"
#include "WoWPlayer.h"

class WoWObjectManager
{
public:
	typedef std::unordered_map<uint64_t, const WoWObject> ObjectMap;
	typedef std::vector<WoWUnit> UnitContainer;

    typedef bool(__fastcall *EnumCallback)(uint64_t guid, void* arg);

    typedef bool(__fastcall *ObjectManagerEnumVisibleObjects)(EnumCallback callback, void* arg);

    typedef uint64_t(*ObjectManagerGetActivePlayerGUID)();

public:
    static const uintptr_t EnumVisableObjectsAddress = 0x468380;
    static const uintptr_t GetActivePlayerGUIDAddress = 0x468550;

public:
	WoWObjectManager();

	~WoWObjectManager();

	static void Read(WoWObjectManager* pManager, void* basePtr);

    void ReadObject(void* objPtr);

    void UpdatePlayer();

    uint64_t GetActivePlayerGUID();

    const WoWPlayer& GetPlayer();

    boost::optional<WoWObject> GetObjectByGuid(uint64_t guid);

    void NewObject(uint64_t guid);

    void ClearObjects();

    const ObjectMap& Objects() const;

    const UnitContainer& Units() const;

    const UnitContainer& GetEnemyUnits() const;

private:
    void NewUnit(const WoWObject& object);

    void NewPlayer(const WoWObject& object);

    uint32_t GetUnitReaction(const WoWUnit& unit) const;

    bool IsUnitEnemy(const WoWUnit& unit) const;

    bool IsUnitFriendly(const WoWUnit& unit) const;

private:
    WoWPlayer mPlayer;
	ObjectMap mObjects;
    UnitContainer mUnits;
    UnitContainer mEnemyUnits;
};
