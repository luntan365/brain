#pragma once

#include <hadesmem/read.hpp>

#include "Global.h"
#include "WoWObjectManager.h"

#define DEFINE_STATIC_OFFSET(NAME, ADDRESS, TYPE) \
	struct NAME { \
		static const DWORD_PTR offsetAddress = ADDRESS; \
		static TYPE Read() \
		{ \
			return hadesmem::Read<TYPE>(GetThisProcess(), (void*)offsetAddress); \
		} \
	};

#define DEFINE_RELATIVE_OFFSET(PARENT_NAME, NAME, OFFSET, TYPE) \
	struct NAME { \
		static const DWORD_PTR offsetAddress = OFFSET; \
		static TYPE Read() \
		{ \
			return hadesmem::Read<TYPE>( \
				GetThisProcess(), \
				(void*)(offsetAddress + PARENT_NAME::offsetAddress)); \
		} \
	};

#define DEFINE_COMPLEX_STATIC_OFFSET(NAME, OFFSET, TYPE) \
	struct NAME { \
		static const DWORD_PTR offsetAddress = OFFSET; \
		static void Read(TYPE* pT) \
		{ \
			TYPE::Read(pT, (void*)OFFSET); \
		} \
	};

void* AddOffset(void* p, int offset);

template <typename T>
void ReadOffsetInto(uintptr_t addr, T* out)
{
	*out = hadesmem::Read<T>(GetThisProcess(), (void*)addr);
}

template <typename T>
void ReadOffsetInto(void* addr, T* out)
{
	*out = hadesmem::Read<T>(GetThisProcess(), addr);
}

template <typename T>
void ReadOffsetInto(void* base, uintptr_t offset, T* out)
{
	ReadOffsetInto(AddOffset(base, offset), out);
}

template <typename T>
T ReadOffset(uintptr_t base)
{
	T t;
	ReadOffsetInto(base, &t);
	return t;
}

template <typename T>
T ReadOffset(void* base)
{
	T t;
	ReadOffsetInto(base, &t);
	return t;
}

template <typename T>
T ReadOffset(void* base, uintptr_t offset)
{
	T t;
	ReadOffsetInto(base, offset, &t);
	return t;
}

class Offsets
{
public:
	DEFINE_STATIC_OFFSET(IsInGame, 0x00B4B424, bool);

	DEFINE_COMPLEX_STATIC_OFFSET(ObjectManager, 0xB41414, WoWObjectManager);

	DEFINE_STATIC_OFFSET(CurrentTargetGUID, 0x74e2d8, uint64_t);
	DEFINE_STATIC_OFFSET(PlayerGUID, 0x741e30, uint64_t);
};

struct StaticFields
{
	static constexpr void* STATIC_CORPSE_X = (void*)0xB4E284;
	static constexpr void* STATIC_CORPSE_Y = (void*)0xB4E288;
	static constexpr void* STATIC_CORPSE_Z = (void*)0xB4E28C;
	static constexpr void* STATIC_AUTO_REPEATING_SPELL = (void*)0xCEAC30;
	static constexpr void* STATIC_MERCHANT_GUID = (void*)0xBDDFA0;
	static constexpr void* STATIC_MERCHANT_ITEMS_COUNT = (void*)0xBDDFA8;
	static constexpr uintptr_t STATIC_MERCHANT_ITEMS_START = 0xC4C4A0;
	static constexpr void* STATIC_GOSSIP_GUID = (void*)0xBC3F58;
    static constexpr uintptr_t STATIC_OPTIONS_OFFSET = 0xBBBE90;
    static constexpr uintptr_t STATIC_OPTIONS_END = 0xBC3F50;
    static constexpr uintptr_t STATIC_OPTIONS_END_OFFSET = 2048;
    static constexpr uintptr_t STATIC_OPTIONS_ITER = 2060;
    static constexpr uintptr_t STATIC_GOSSIP_TYPE = 2056;
    static constexpr void* STATIC_SPELLBOOK = (void*)0xB700F0;
    static constexpr void* STATIC_GET_SPELL_COOLDOWN = (void*)0x6E2EA0;
    static constexpr void* STATIC_SPELL_COOLDOWN_PTR = (void*)0xCECAEC;
    static constexpr void* STATIC_SPELL_COOLDOWN_OFFSET = (void*)0xCECAF4;
	static constexpr uintptr_t STATIC_GET_PERFORMANCE_COUNTER = 0x42C010;        
};

struct ObjectFields
{
    static constexpr uintptr_t OBJECT_FIELD_ID = 0x0C;
	static constexpr uintptr_t OBJECT_END = 0x18;
};

struct UnitFields
{
	static constexpr uintptr_t UNIT_FIELD_CHARM = ObjectFields::OBJECT_END + 0x0;
	static constexpr uintptr_t UNIT_FIELD_SUMMON = ObjectFields::OBJECT_END + 0x8;
	static constexpr uintptr_t UNIT_FIELD_CHARMEDBY = ObjectFields::OBJECT_END + 0x10;
	static constexpr uintptr_t UNIT_FIELD_SUMMONEDBY = ObjectFields::OBJECT_END + 0x18;
	static constexpr uintptr_t UNIT_FIELD_CREATEDBY = ObjectFields::OBJECT_END + 0x20;
	static constexpr uintptr_t UNIT_FIELD_TARGET = ObjectFields::OBJECT_END + 0x28;
	static constexpr uintptr_t UNIT_FIELD_PERSUADED = ObjectFields::OBJECT_END + 0x30;
	static constexpr uintptr_t UNIT_FIELD_CHANNEL_OBJECT = ObjectFields::OBJECT_END + 0x38;
	static constexpr uintptr_t UNIT_FIELD_HEALTH = ObjectFields::OBJECT_END + 0x40;
	static constexpr uintptr_t UNIT_FIELD_POWER1 = ObjectFields::OBJECT_END + 0x44;
	static constexpr uintptr_t UNIT_FIELD_POWER2 = ObjectFields::OBJECT_END + 0x48;
	static constexpr uintptr_t UNIT_FIELD_POWER3 = ObjectFields::OBJECT_END + 0x4C;
	static constexpr uintptr_t UNIT_FIELD_POWER4 = ObjectFields::OBJECT_END + 0x50;
	static constexpr uintptr_t UNIT_FIELD_POWER5 = ObjectFields::OBJECT_END + 0x54;
	static constexpr uintptr_t UNIT_FIELD_MAXHEALTH = ObjectFields::OBJECT_END + 0x58;
	static constexpr uintptr_t UNIT_FIELD_MAXPOWER1 = ObjectFields::OBJECT_END + 0x5C;
	static constexpr uintptr_t UNIT_FIELD_MAXPOWER2 = ObjectFields::OBJECT_END + 0x60;
	static constexpr uintptr_t UNIT_FIELD_MAXPOWER3 = ObjectFields::OBJECT_END + 0x64;
	static constexpr uintptr_t UNIT_FIELD_MAXPOWER4 = ObjectFields::OBJECT_END + 0x68;
	static constexpr uintptr_t UNIT_FIELD_MAXPOWER5 = ObjectFields::OBJECT_END + 0x6C;
	static constexpr uintptr_t UNIT_FIELD_LEVEL = ObjectFields::OBJECT_END + 0x70;
	static constexpr uintptr_t UNIT_FIELD_FACTIONTEMPLATE = ObjectFields::OBJECT_END + 0x74;
	static constexpr uintptr_t UNIT_FIELD_BYTES_0 = ObjectFields::OBJECT_END + 0x78;
	static constexpr uintptr_t UNIT_VIRTUAL_ITEM_SLOT_DISPLAY = ObjectFields::OBJECT_END + 0x7C;
	static constexpr uintptr_t UNIT_VIRTUAL_ITEM_INFO = ObjectFields::OBJECT_END + 0x88;
	static constexpr uintptr_t UNIT_FIELD_FLAGS = ObjectFields::OBJECT_END + 0xA0;
	static constexpr uintptr_t UNIT_FIELD_AURA = ObjectFields::OBJECT_END + 0xA4;
	static constexpr uintptr_t UNIT_FIELD_AURA_DEBUFF = ObjectFields::OBJECT_END + 0x124;
	static constexpr uintptr_t UNIT_FIELD_AURAFLAGS = ObjectFields::OBJECT_END + 0x164;
	static constexpr uintptr_t UNIT_FIELD_AURALEVELS = ObjectFields::OBJECT_END + 0x17C;
	static constexpr uintptr_t UNIT_FIELD_AURAAPPLICATIONS = ObjectFields::OBJECT_END + 0x1AC;
	static constexpr uintptr_t UNIT_FIELD_AURASTATE = ObjectFields::OBJECT_END + 0x1DC;
	static constexpr uintptr_t UNIT_FIELD_BASEATTACKTIME = ObjectFields::OBJECT_END + 0x1E0;
	static constexpr uintptr_t UNIT_FIELD_RANGEDATTACKTIME = ObjectFields::OBJECT_END + 0x1E8;
	static constexpr uintptr_t UNIT_FIELD_BOUNDINGRADIUS = ObjectFields::OBJECT_END + 0x1EC;
	static constexpr uintptr_t UNIT_FIELD_COMBATREACH = ObjectFields::OBJECT_END + 0x1F0;
	static constexpr uintptr_t UNIT_FIELD_DISPLAYID = ObjectFields::OBJECT_END + 0x1F4;
	static constexpr uintptr_t UNIT_FIELD_NATIVEDISPLAYID = ObjectFields::OBJECT_END + 0x1F8;
	static constexpr uintptr_t UNIT_FIELD_MOUNTDISPLAYID = ObjectFields::OBJECT_END + 0x1FC;
	static constexpr uintptr_t UNIT_FIELD_MINDAMAGE = ObjectFields::OBJECT_END + 0x200;
	static constexpr uintptr_t UNIT_FIELD_MAXDAMAGE = ObjectFields::OBJECT_END + 0x204;
	static constexpr uintptr_t UNIT_FIELD_MINOFFHANDDAMAGE = ObjectFields::OBJECT_END + 0x208;
	static constexpr uintptr_t UNIT_FIELD_MAXOFFHANDDAMAGE = ObjectFields::OBJECT_END + 0x20C;
	static constexpr uintptr_t UNIT_FIELD_BYTES_1 = ObjectFields::OBJECT_END + 0x210;
	static constexpr uintptr_t UNIT_FIELD_PETNUMBER = ObjectFields::OBJECT_END + 0x214;
	static constexpr uintptr_t UNIT_FIELD_PET_NAME_TIMESTAMP = ObjectFields::OBJECT_END + 0x218;
	static constexpr uintptr_t UNIT_FIELD_PETEXPERIENCE = ObjectFields::OBJECT_END + 0x21C;
	static constexpr uintptr_t UNIT_FIELD_PETNEXTLEVELEXP = ObjectFields::OBJECT_END + 0x220;
	static constexpr uintptr_t UNIT_DYNAMIC_FLAGS = ObjectFields::OBJECT_END + 0x224;
	static constexpr uintptr_t UNIT_CHANNEL_SPELL = ObjectFields::OBJECT_END + 0x228;
	static constexpr uintptr_t UNIT_MOD_CAST_SPEED = ObjectFields::OBJECT_END + 0x22C;
	static constexpr uintptr_t UNIT_CREATED_BY_SPELL = ObjectFields::OBJECT_END + 0x230;
	static constexpr uintptr_t UNIT_NPC_FLAGS = ObjectFields::OBJECT_END + 0x234;
	static constexpr uintptr_t UNIT_NPC_EMOTESTATE = ObjectFields::OBJECT_END + 0x238;
	static constexpr uintptr_t UNIT_TRAINING_POINTS = ObjectFields::OBJECT_END + 0x23C;
	static constexpr uintptr_t UNIT_FIELD_STAT0 = ObjectFields::OBJECT_END + 0x240;
	static constexpr uintptr_t UNIT_FIELD_STAT1 = ObjectFields::OBJECT_END + 0x244;
	static constexpr uintptr_t UNIT_FIELD_STAT2 = ObjectFields::OBJECT_END + 0x248;
	static constexpr uintptr_t UNIT_FIELD_STAT3 = ObjectFields::OBJECT_END + 0x24C;
	static constexpr uintptr_t UNIT_FIELD_STAT4 = ObjectFields::OBJECT_END + 0x250;
	static constexpr uintptr_t UNIT_FIELD_RESISTANCES = ObjectFields::OBJECT_END + 0x254;
	static constexpr uintptr_t UNIT_FIELD_BASE_MANA = ObjectFields::OBJECT_END + 0x270;
	static constexpr uintptr_t UNIT_FIELD_BASE_HEALTH = ObjectFields::OBJECT_END + 0x274;
	static constexpr uintptr_t UNIT_FIELD_BYTES_2 = ObjectFields::OBJECT_END + 0x278;
	static constexpr uintptr_t UNIT_FIELD_ATTACK_POWER = ObjectFields::OBJECT_END + 0x27C;
	static constexpr uintptr_t UNIT_FIELD_ATTACK_POWER_MODS = ObjectFields::OBJECT_END + 0x280;
	static constexpr uintptr_t UNIT_FIELD_ATTACK_POWER_MULTIPLIER = ObjectFields::OBJECT_END + 0x284;
	static constexpr uintptr_t UNIT_FIELD_RANGED_ATTACK_POWER = ObjectFields::OBJECT_END + 0x288;
	static constexpr uintptr_t UNIT_FIELD_RANGED_ATTACK_POWER_MODS = ObjectFields::OBJECT_END + 0x28C;
	static constexpr uintptr_t UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER = ObjectFields::OBJECT_END + 0x290;
	static constexpr uintptr_t UNIT_FIELD_MINRANGEDDAMAGE = ObjectFields::OBJECT_END + 0x294;
	static constexpr uintptr_t UNIT_FIELD_MAXRANGEDDAMAGE = ObjectFields::OBJECT_END + 0x298;
	static constexpr uintptr_t UNIT_FIELD_POWER_COST_MODIFIER = ObjectFields::OBJECT_END + 0x29C;
	static constexpr uintptr_t UNIT_FIELD_POWER_COST_MULTIPLIER = ObjectFields::OBJECT_END + 0x2B8;
	static constexpr uintptr_t UNIT_FIELD_PADDING = ObjectFields::OBJECT_END + 0x2D4;
	static constexpr uintptr_t UNIT_END = ObjectFields::OBJECT_END + 0x2D8;
};

struct ItemFields
{
	static constexpr uintptr_t ITEM_FIELD_OWNER = ObjectFields::OBJECT_END + 0x0;
	static constexpr uintptr_t ITEM_FIELD_CONTAINED = ObjectFields::OBJECT_END + 0x8;
	static constexpr uintptr_t ITEM_FIELD_CREATOR = ObjectFields::OBJECT_END + 0x10;
	static constexpr uintptr_t ITEM_FIELD_GIFTCREATOR = ObjectFields::OBJECT_END + 0x18;
	static constexpr uintptr_t ITEM_FIELD_STACK_COUNT = ObjectFields::OBJECT_END + 0x20;
	static constexpr uintptr_t ITEM_FIELD_DURATION = ObjectFields::OBJECT_END + 0x24;
	static constexpr uintptr_t ITEM_FIELD_SPELL_CHARGES = ObjectFields::OBJECT_END + 0x28;
	static constexpr uintptr_t ITEM_FIELD_FLAGS = ObjectFields::OBJECT_END + 0x3C;
	static constexpr uintptr_t ITEM_FIELD_ENCHANTMENT = ObjectFields::OBJECT_END + 0x40;
	static constexpr uintptr_t ITEM_FIELD_PROPERTY_SEED = ObjectFields::OBJECT_END + 0x94;
	static constexpr uintptr_t ITEM_FIELD_RANDOM_PROPERTIES_ID = ObjectFields::OBJECT_END + 0x98;
	static constexpr uintptr_t ITEM_FIELD_ITEM_TEXT_ID = ObjectFields::OBJECT_END + 0x9C;
	static constexpr uintptr_t ITEM_FIELD_DURABILITY = ObjectFields::OBJECT_END + 0xA0;
	static constexpr uintptr_t ITEM_FIELD_MAXDURABILITY = ObjectFields::OBJECT_END + 0xA4;
	static constexpr uintptr_t ITEM_END = ObjectFields::OBJECT_END + 0xA8;
};

struct PlayerFields
{
	static constexpr uintptr_t PLAYER_DUEL_ARBITER = UnitFields::UNIT_END + 0x0;
	static constexpr uintptr_t PLAYER_FLAGS = UnitFields::UNIT_END + 0x8;
	static constexpr uintptr_t PLAYER_GUILDID = UnitFields::UNIT_END + 0xC;
	static constexpr uintptr_t PLAYER_GUILDRANK = UnitFields::UNIT_END + 0x10;
	static constexpr uintptr_t PLAYER_BYTES = UnitFields::UNIT_END + 0x14;
	static constexpr uintptr_t PLAYER_BYTES_2 = UnitFields::UNIT_END + 0x18;
	static constexpr uintptr_t PLAYER_BYTES_3 = UnitFields::UNIT_END + 0x1C;
	static constexpr uintptr_t PLAYER_DUEL_TEAM = UnitFields::UNIT_END + 0x20;
	static constexpr uintptr_t PLAYER_GUILD_TIMESTAMP = UnitFields::UNIT_END + 0x24;
	static constexpr uintptr_t PLAYER_QUEST_LOG_1_1 = UnitFields::UNIT_END + 0x28;
	static constexpr uintptr_t PLAYER_QUEST_LOG_1_2 = UnitFields::UNIT_END + 0x2C;
	static constexpr uintptr_t PLAYER_QUEST_LOG_2_1 = UnitFields::UNIT_END + 0x34;
	static constexpr uintptr_t PLAYER_QUEST_LOG_2_2 = UnitFields::UNIT_END + 0x38;
	static constexpr uintptr_t PLAYER_QUEST_LOG_3_1 = UnitFields::UNIT_END + 0x40;
	static constexpr uintptr_t PLAYER_QUEST_LOG_3_2 = UnitFields::UNIT_END + 0x44;
	static constexpr uintptr_t PLAYER_QUEST_LOG_4_1 = UnitFields::UNIT_END + 0x4C;
	static constexpr uintptr_t PLAYER_QUEST_LOG_4_2 = UnitFields::UNIT_END + 0x50;
	static constexpr uintptr_t PLAYER_QUEST_LOG_5_1 = UnitFields::UNIT_END + 0x58;
	static constexpr uintptr_t PLAYER_QUEST_LOG_5_2 = UnitFields::UNIT_END + 0x5C;
	static constexpr uintptr_t PLAYER_QUEST_LOG_6_1 = UnitFields::UNIT_END + 0x64;
	static constexpr uintptr_t PLAYER_QUEST_LOG_6_2 = UnitFields::UNIT_END + 0x68;
	static constexpr uintptr_t PLAYER_QUEST_LOG_7_1 = UnitFields::UNIT_END + 0x70;
	static constexpr uintptr_t PLAYER_QUEST_LOG_7_2 = UnitFields::UNIT_END + 0x74;
	static constexpr uintptr_t PLAYER_QUEST_LOG_8_1 = UnitFields::UNIT_END + 0x7C;
	static constexpr uintptr_t PLAYER_QUEST_LOG_8_2 = UnitFields::UNIT_END + 0x80;
	static constexpr uintptr_t PLAYER_QUEST_LOG_9_1 = UnitFields::UNIT_END + 0x88;
	static constexpr uintptr_t PLAYER_QUEST_LOG_9_2 = UnitFields::UNIT_END + 0x8C;
	static constexpr uintptr_t PLAYER_QUEST_LOG_10_1 = UnitFields::UNIT_END + 0x94;
	static constexpr uintptr_t PLAYER_QUEST_LOG_10_2 = UnitFields::UNIT_END + 0x98;
	static constexpr uintptr_t PLAYER_QUEST_LOG_11_1 = UnitFields::UNIT_END + 0xA0;
	static constexpr uintptr_t PLAYER_QUEST_LOG_11_2 = UnitFields::UNIT_END + 0xA4;
	static constexpr uintptr_t PLAYER_QUEST_LOG_12_1 = UnitFields::UNIT_END + 0xAC;
	static constexpr uintptr_t PLAYER_QUEST_LOG_12_2 = UnitFields::UNIT_END + 0xB0;
	static constexpr uintptr_t PLAYER_QUEST_LOG_13_1 = UnitFields::UNIT_END + 0xB8;
	static constexpr uintptr_t PLAYER_QUEST_LOG_13_2 = UnitFields::UNIT_END + 0xBC;
	static constexpr uintptr_t PLAYER_QUEST_LOG_14_1 = UnitFields::UNIT_END + 0xC4;
	static constexpr uintptr_t PLAYER_QUEST_LOG_14_2 = UnitFields::UNIT_END + 0xC8;
	static constexpr uintptr_t PLAYER_QUEST_LOG_15_1 = UnitFields::UNIT_END + 0xD0;
	static constexpr uintptr_t PLAYER_QUEST_LOG_15_2 = UnitFields::UNIT_END + 0xD4;
	static constexpr uintptr_t PLAYER_QUEST_LOG_16_1 = UnitFields::UNIT_END + 0xDC;
	static constexpr uintptr_t PLAYER_QUEST_LOG_16_2 = UnitFields::UNIT_END + 0xE0;
	static constexpr uintptr_t PLAYER_QUEST_LOG_17_1 = UnitFields::UNIT_END + 0xE8;
	static constexpr uintptr_t PLAYER_QUEST_LOG_17_2 = UnitFields::UNIT_END + 0xEC;
	static constexpr uintptr_t PLAYER_QUEST_LOG_18_1 = UnitFields::UNIT_END + 0xF4;
	static constexpr uintptr_t PLAYER_QUEST_LOG_18_2 = UnitFields::UNIT_END + 0xF8;
	static constexpr uintptr_t PLAYER_QUEST_LOG_19_1 = UnitFields::UNIT_END + 0x100;
	static constexpr uintptr_t PLAYER_QUEST_LOG_19_2 = UnitFields::UNIT_END + 0x104;
	static constexpr uintptr_t PLAYER_QUEST_LOG_20_1 = UnitFields::UNIT_END + 0x10C;
	static constexpr uintptr_t PLAYER_QUEST_LOG_20_2 = UnitFields::UNIT_END + 0x110;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_1_CREATOR = UnitFields::UNIT_END + 0x118;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_1_0 = UnitFields::UNIT_END + 0x120;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_1_PROPERTIES = UnitFields::UNIT_END + 0x140;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_1_PAD = UnitFields::UNIT_END + 0x144;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_2_CREATOR = UnitFields::UNIT_END + 0x148;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_2_0 = UnitFields::UNIT_END + 0x150;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_2_PROPERTIES = UnitFields::UNIT_END + 0x170;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_2_PAD = UnitFields::UNIT_END + 0x174;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_3_CREATOR = UnitFields::UNIT_END + 0x178;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_3_0 = UnitFields::UNIT_END + 0x180;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_3_PROPERTIES = UnitFields::UNIT_END + 0x1A0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_3_PAD = UnitFields::UNIT_END + 0x1A4;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_4_CREATOR = UnitFields::UNIT_END + 0x1A8;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_4_0 = UnitFields::UNIT_END + 0x1B0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_4_PROPERTIES = UnitFields::UNIT_END + 0x1D0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_4_PAD = UnitFields::UNIT_END + 0x1D4;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_5_CREATOR = UnitFields::UNIT_END + 0x1D8;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_5_0 = UnitFields::UNIT_END + 0x1E0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_5_PROPERTIES = UnitFields::UNIT_END + 0x200;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_5_PAD = UnitFields::UNIT_END + 0x204;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_6_CREATOR = UnitFields::UNIT_END + 0x208;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_6_0 = UnitFields::UNIT_END + 0x210;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_6_PROPERTIES = UnitFields::UNIT_END + 0x230;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_6_PAD = UnitFields::UNIT_END + 0x234;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_7_CREATOR = UnitFields::UNIT_END + 0x238;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_7_0 = UnitFields::UNIT_END + 0x240;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_7_PROPERTIES = UnitFields::UNIT_END + 0x260;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_7_PAD = UnitFields::UNIT_END + 0x264;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_8_CREATOR = UnitFields::UNIT_END + 0x268;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_8_0 = UnitFields::UNIT_END + 0x270;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_8_PROPERTIES = UnitFields::UNIT_END + 0x290;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_8_PAD = UnitFields::UNIT_END + 0x294;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_9_CREATOR = UnitFields::UNIT_END + 0x298;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_9_0 = UnitFields::UNIT_END + 0x2A0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_9_PROPERTIES = UnitFields::UNIT_END + 0x2C0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_9_PAD = UnitFields::UNIT_END + 0x2C4;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_10_CREATOR = UnitFields::UNIT_END + 0x2C8;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_10_0 = UnitFields::UNIT_END + 0x2D0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_10_PROPERTIES = UnitFields::UNIT_END + 0x2F0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_10_PAD = UnitFields::UNIT_END + 0x2F4;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_11_CREATOR = UnitFields::UNIT_END + 0x2F8;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_11_0 = UnitFields::UNIT_END + 0x300;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_11_PROPERTIES = UnitFields::UNIT_END + 0x320;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_11_PAD = UnitFields::UNIT_END + 0x324;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_12_CREATOR = UnitFields::UNIT_END + 0x328;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_12_0 = UnitFields::UNIT_END + 0x330;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_12_PROPERTIES = UnitFields::UNIT_END + 0x350;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_12_PAD = UnitFields::UNIT_END + 0x354;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_13_CREATOR = UnitFields::UNIT_END + 0x358;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_13_0 = UnitFields::UNIT_END + 0x360;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_13_PROPERTIES = UnitFields::UNIT_END + 0x380;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_13_PAD = UnitFields::UNIT_END + 0x384;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_14_CREATOR = UnitFields::UNIT_END + 0x388;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_14_0 = UnitFields::UNIT_END + 0x390;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_14_PROPERTIES = UnitFields::UNIT_END + 0x3B0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_14_PAD = UnitFields::UNIT_END + 0x3B4;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_15_CREATOR = UnitFields::UNIT_END + 0x3B8;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_15_0 = UnitFields::UNIT_END + 0x3C0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_15_PROPERTIES = UnitFields::UNIT_END + 0x3E0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_15_PAD = UnitFields::UNIT_END + 0x3E4;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_16_CREATOR = UnitFields::UNIT_END + 0x3E8;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_16_0 = UnitFields::UNIT_END + 0x3F0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_16_PROPERTIES = UnitFields::UNIT_END + 0x410;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_16_PAD = UnitFields::UNIT_END + 0x414;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_17_CREATOR = UnitFields::UNIT_END + 0x418;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_17_0 = UnitFields::UNIT_END + 0x420;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_17_PROPERTIES = UnitFields::UNIT_END + 0x440;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_17_PAD = UnitFields::UNIT_END + 0x444;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_18_CREATOR = UnitFields::UNIT_END + 0x448;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_18_0 = UnitFields::UNIT_END + 0x450;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_18_PROPERTIES = UnitFields::UNIT_END + 0x470;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_18_PAD = UnitFields::UNIT_END + 0x474;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_19_CREATOR = UnitFields::UNIT_END + 0x478;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_19_0 = UnitFields::UNIT_END + 0x480;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_19_PROPERTIES = UnitFields::UNIT_END + 0x4A0;
	static constexpr uintptr_t PLAYER_VISIBLE_ITEM_19_PAD = UnitFields::UNIT_END + 0x4A4;
	static constexpr uintptr_t PLAYER_FIELD_INV_SLOT_HEAD = UnitFields::UNIT_END + 0x4A8;
	static constexpr uintptr_t PLAYER_FIELD_PACK_SLOT_1 = UnitFields::UNIT_END + 0x560;
	static constexpr uintptr_t PLAYER_FIELD_BANK_SLOT_1 = UnitFields::UNIT_END + 0x5E0;
	static constexpr uintptr_t PLAYER_FIELD_BANKBAG_SLOT_1 = UnitFields::UNIT_END + 0x6A0;
	static constexpr uintptr_t PLAYER_FIELD_VENDORBUYBACK_SLOT_1 = UnitFields::UNIT_END + 0x6D0;
	static constexpr uintptr_t PLAYER_FIELD_KEYRING_SLOT_1 = UnitFields::UNIT_END + 0x730;
	static constexpr uintptr_t PLAYER_FARSIGHT = UnitFields::UNIT_END + 0x830;
	static constexpr uintptr_t PLAYER__FIELD_COMBO_TARGET = UnitFields::UNIT_END + 0x838;
	static constexpr uintptr_t PLAYER_XP = UnitFields::UNIT_END + 0x840;
	static constexpr uintptr_t PLAYER_NEXT_LEVEL_XP = UnitFields::UNIT_END + 0x844;
	static constexpr uintptr_t PLAYER_SKILL_INFO_1_1 = UnitFields::UNIT_END + 0x848;
	static constexpr uintptr_t PLAYER_CHARACTER_POINTS1 = UnitFields::UNIT_END + 0xE48;
	static constexpr uintptr_t PLAYER_CHARACTER_POINTS2 = UnitFields::UNIT_END + 0xE4C;
	static constexpr uintptr_t PLAYER_TRACK_CREATURES = UnitFields::UNIT_END + 0xE50;
	static constexpr uintptr_t PLAYER_TRACK_RESOURCES = UnitFields::UNIT_END + 0xE54;
	static constexpr uintptr_t PLAYER_BLOCK_PERCENTAGE = UnitFields::UNIT_END + 0xE58;
	static constexpr uintptr_t PLAYER_DODGE_PERCENTAGE = UnitFields::UNIT_END + 0xE5C;
	static constexpr uintptr_t PLAYER_PARRY_PERCENTAGE = UnitFields::UNIT_END + 0xE60;
	static constexpr uintptr_t PLAYER_CRIT_PERCENTAGE = UnitFields::UNIT_END + 0xE64;
	static constexpr uintptr_t PLAYER_RANGED_CRIT_PERCENTAGE = UnitFields::UNIT_END + 0xE68;
	static constexpr uintptr_t PLAYER_EXPLORED_ZONES_1 = UnitFields::UNIT_END + 0xE6C;
	static constexpr uintptr_t PLAYER_REST_STATE_EXPERIENCE = UnitFields::UNIT_END + 0xF6C;
	static constexpr uintptr_t PLAYER_FIELD_COINAGE = UnitFields::UNIT_END + 0xF70;
	static constexpr uintptr_t PLAYER_FIELD_POSSTAT0 = UnitFields::UNIT_END + 0xF74;
	static constexpr uintptr_t PLAYER_FIELD_POSSTAT1 = UnitFields::UNIT_END + 0xF78;
	static constexpr uintptr_t PLAYER_FIELD_POSSTAT2 = UnitFields::UNIT_END + 0xF7C;
	static constexpr uintptr_t PLAYER_FIELD_POSSTAT3 = UnitFields::UNIT_END + 0xF80;
	static constexpr uintptr_t PLAYER_FIELD_POSSTAT4 = UnitFields::UNIT_END + 0xF84;
	static constexpr uintptr_t PLAYER_FIELD_NEGSTAT0 = UnitFields::UNIT_END + 0xF88;
	static constexpr uintptr_t PLAYER_FIELD_NEGSTAT1 = UnitFields::UNIT_END + 0xF8C;
	static constexpr uintptr_t PLAYER_FIELD_NEGSTAT2 = UnitFields::UNIT_END + 0xF90;
	static constexpr uintptr_t PLAYER_FIELD_NEGSTAT3 = UnitFields::UNIT_END + 0xF94;
	static constexpr uintptr_t PLAYER_FIELD_NEGSTAT4 = UnitFields::UNIT_END + 0xF98;
	static constexpr uintptr_t PLAYER_FIELD_RESISTANCEBUFFMODSPOSITIVE = UnitFields::UNIT_END + 0xF9C;
	static constexpr uintptr_t PLAYER_FIELD_RESISTANCEBUFFMODSNEGATIVE = UnitFields::UNIT_END + 0xFB8;
	static constexpr uintptr_t PLAYER_FIELD_MOD_DAMAGE_DONE_POS = UnitFields::UNIT_END + 0xFD4;
	static constexpr uintptr_t PLAYER_FIELD_MOD_DAMAGE_DONE_NEG = UnitFields::UNIT_END + 0xFF0;
	static constexpr uintptr_t PLAYER_FIELD_MOD_DAMAGE_DONE_PCT = UnitFields::UNIT_END + 0x100C;
	static constexpr uintptr_t PLAYER_FIELD_BYTES = UnitFields::UNIT_END + 0x1028;
	static constexpr uintptr_t PLAYER_AMMO_ID = UnitFields::UNIT_END + 0x102C;
	static constexpr uintptr_t PLAYER_SELF_RES_SPELL = UnitFields::UNIT_END + 0x1030;
	static constexpr uintptr_t PLAYER_FIELD_PVP_MEDALS = UnitFields::UNIT_END + 0x1034;
	static constexpr uintptr_t PLAYER_FIELD_BUYBACK_PRICE_1 = UnitFields::UNIT_END + 0x1038;
	static constexpr uintptr_t PLAYER_FIELD_BUYBACK_TIMESTAMP_1 = UnitFields::UNIT_END + 0x1068;
	static constexpr uintptr_t PLAYER_FIELD_SESSION_KILLS = UnitFields::UNIT_END + 0x1098;
	static constexpr uintptr_t PLAYER_FIELD_YESTERDAY_KILLS = UnitFields::UNIT_END + 0x109C;
	static constexpr uintptr_t PLAYER_FIELD_LAST_WEEK_KILLS = UnitFields::UNIT_END + 0x10A0;
	static constexpr uintptr_t PLAYER_FIELD_THIS_WEEK_KILLS = UnitFields::UNIT_END + 0x10A4;
	static constexpr uintptr_t PLAYER_FIELD_THIS_WEEK_CONTRIBUTION = UnitFields::UNIT_END + 0x10A8;
	static constexpr uintptr_t PLAYER_FIELD_LIFETIME_HONORBALE_KILLS = UnitFields::UNIT_END + 0x10AC;
	static constexpr uintptr_t PLAYER_FIELD_LIFETIME_DISHONORBALE_KILLS = UnitFields::UNIT_END + 0x10B0;
	static constexpr uintptr_t PLAYER_FIELD_YESTERDAY_CONTRIBUTION = UnitFields::UNIT_END + 0x10B4;
	static constexpr uintptr_t PLAYER_FIELD_LAST_WEEK_CONTRIBUTION = UnitFields::UNIT_END + 0x10B8;
	static constexpr uintptr_t PLAYER_FIELD_LAST_WEEK_RANK = UnitFields::UNIT_END + 0x10BC;
	static constexpr uintptr_t PLAYER_FIELD_BYTES2 = UnitFields::UNIT_END + 0x10C0;
	static constexpr uintptr_t PLAYER_FIELD_WATCHED_FACTION_INDEX = UnitFields::UNIT_END + 0x10C4;
	static constexpr uintptr_t PLAYER_FIELD_COMBAT_RATING_1 = UnitFields::UNIT_END + 0x10C8;
	static constexpr uintptr_t PLAYER_END = UnitFields::UNIT_END + 0x1118;
};

struct ContainerFields
{
	static constexpr uintptr_t CONTAINER_FIELD_NUM_SLOTS = ItemFields::ITEM_END + 0x0;
	static constexpr uintptr_t CONTAINER_ALIGN_PAD = ItemFields::ITEM_END + 0x4;
	static constexpr uintptr_t CONTAINER_FIELD_SLOT_1 = ItemFields::ITEM_END + 0x8;
	static constexpr uintptr_t CONTAINER_END = ItemFields::ITEM_END + 0x128;
};

struct GameObjectFields
{
	static constexpr uintptr_t GAMEOBJECT_DISPLAYID = ObjectFields::OBJECT_END + 0x8;
	static constexpr uintptr_t GAMEOBJECT_FLAGS = ObjectFields::OBJECT_END + 0xC;
	static constexpr uintptr_t GAMEOBJECT_ROTATION = ObjectFields::OBJECT_END + 0x10;
	static constexpr uintptr_t GAMEOBJECT_STATE = ObjectFields::OBJECT_END + 0x20;
	static constexpr uintptr_t GAMEOBJECT_POS_X = ObjectFields::OBJECT_END + 0x24;
	static constexpr uintptr_t GAMEOBJECT_POS_Y = ObjectFields::OBJECT_END + 0x28;
	static constexpr uintptr_t GAMEOBJECT_POS_Z = ObjectFields::OBJECT_END + 0x2C;
	static constexpr uintptr_t GAMEOBJECT_FACING = ObjectFields::OBJECT_END + 0x30;
	static constexpr uintptr_t GAMEOBJECT_DYN_FLAGS = ObjectFields::OBJECT_END + 0x34;
	static constexpr uintptr_t GAMEOBJECT_FACTION = ObjectFields::OBJECT_END + 0x38;
	static constexpr uintptr_t GAMEOBJECT_TYPE_ID = ObjectFields::OBJECT_END + 0x3C;
	static constexpr uintptr_t GAMEOBJECT_LEVEL = ObjectFields::OBJECT_END + 0x40;
	static constexpr uintptr_t GAMEOBJECT_ARTKIT = ObjectFields::OBJECT_END + 0x44;
	static constexpr uintptr_t GAMEOBJECT_ANIMPROGRESS = ObjectFields::OBJECT_END + 0x48;
	static constexpr uintptr_t GAMEOBJECT_PADDING = ObjectFields::OBJECT_END + 0x4C;
	static constexpr uintptr_t GAMEOBJECT_END = ObjectFields::OBJECT_END + 0x50;
};

struct DynamicObjectFields
{
	static constexpr uintptr_t DYNAMICOBJECT_CASTER = ObjectFields::OBJECT_END + 0x0;
	static constexpr uintptr_t DYNAMICOBJECT_BYTES = ObjectFields::OBJECT_END + 0x8;
	static constexpr uintptr_t DYNAMICOBJECT_SPELLID = ObjectFields::OBJECT_END + 0xC;
	static constexpr uintptr_t DYNAMICOBJECT_RADIUS = ObjectFields::OBJECT_END + 0x10;
	static constexpr uintptr_t DYNAMICOBJECT_POS_X = ObjectFields::OBJECT_END + 0x14;
	static constexpr uintptr_t DYNAMICOBJECT_POS_Y = ObjectFields::OBJECT_END + 0x18;
	static constexpr uintptr_t DYNAMICOBJECT_POS_Z = ObjectFields::OBJECT_END + 0x1C;
	static constexpr uintptr_t DYNAMICOBJECT_FACING = ObjectFields::OBJECT_END + 0x20;
	static constexpr uintptr_t DYNAMICOBJECT_PAD = ObjectFields::OBJECT_END + 0x24;
	static constexpr uintptr_t DYNAMICOBJECT_END = ObjectFields::OBJECT_END + 0x28;
};

struct CorpseFields
{
	static constexpr uintptr_t CORPSE_FIELD_OWNER = ObjectFields::OBJECT_END + 0x0;
	static constexpr uintptr_t CORPSE_FIELD_FACING = ObjectFields::OBJECT_END + 0x8;
	static constexpr uintptr_t CORPSE_FIELD_POS_X = ObjectFields::OBJECT_END + 0xC;
	static constexpr uintptr_t CORPSE_FIELD_POS_Y = ObjectFields::OBJECT_END + 0x10;
	static constexpr uintptr_t CORPSE_FIELD_POS_Z = ObjectFields::OBJECT_END + 0x14;
	static constexpr uintptr_t CORPSE_FIELD_DISPLAY_ID = ObjectFields::OBJECT_END + 0x18;
	static constexpr uintptr_t CORPSE_FIELD_ITEM = ObjectFields::OBJECT_END + 0x1C;
	static constexpr uintptr_t CORPSE_FIELD_BYTES_1 = ObjectFields::OBJECT_END + 0x68;
	static constexpr uintptr_t CORPSE_FIELD_BYTES_2 = ObjectFields::OBJECT_END + 0x6C;
	static constexpr uintptr_t CORPSE_FIELD_GUILD = ObjectFields::OBJECT_END + 0x70;
	static constexpr uintptr_t CORPSE_FIELD_FLAGS = ObjectFields::OBJECT_END + 0x74;
	static constexpr uintptr_t CORPSE_FIELD_DYNAMIC_FLAGS = ObjectFields::OBJECT_END + 0x78;
	static constexpr uintptr_t CORPSE_FIELD_PAD = ObjectFields::OBJECT_END + 0x7C;
	static constexpr uintptr_t CORPSE_END = ObjectFields::OBJECT_END + 0x80;
};

enum EquipLoc
{
    EQUIP_LOC_HEAD = 0,
    EQUIP_LOC_NECK,
    EQUIP_LOC_SHOULDER,
    EQUIP_LOC_SHIRT,
    EQUIP_LOC_CHEST,
    EQUIP_LOC_WAIST,
    EQUIP_LOC_LEGS,
    EQUIP_LOC_FEET,
    EQUIP_LOC_WRISTS,
    EQUIP_LOC_HANDS,
    EQUIP_LOC_FINGER_1,
    EQUIP_LOC_FINGER_2,
    EQUIP_LOC_TRINKET_1,
    EQUIP_LOC_TRINKET_2,
    EQUIP_LOC_BACK,
    EQUIP_LOC_MAINHAND,
    EQUIP_LOC_OFFHAND,
    EQUIP_LOC_RANGED,
    EQUIP_LOC_TABARD,
    EQUIP_LOC_BAG_1,
    EQUIP_LOC_BAG_2,
    EQUIP_LOC_BAG_3,
    EQUIP_LOC_BAG_4,
    EQUIP_LOC_SLOTS_TOTAL
};
