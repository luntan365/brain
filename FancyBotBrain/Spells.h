#pragma once

#include <cstdint>
#include <string>

#define MAX_SPELL_TOTEMS 2
#define MAX_SPELL_REAGENTS 8
#define MAX_EFFECT_INDEX 3

struct SpellEntry
{
        uint32_t id;
        uint32_t school;
        uint32_t category;
        uint32_t castUI;
        uint32_t dispel;
        uint32_t mechanic;
        uint32_t attributes;
        uint32_t attributesEx;
        uint32_t attributesEx2;
        uint32_t attributesEx3;
        uint32_t attributesEx4;
        uint32_t stances;
        uint32_t stancesNot;
        uint32_t targets;
        uint32_t targetCreatureType;
        uint32_t requiresSpellFocus;
        uint32_t casterAuraState;
        uint32_t targetAuraState;
        uint32_t castingTimeIndex;
        uint32_t recoveryTime;
        uint32_t categoryRecoveryTime;
        uint32_t interruptFlags;
        uint32_t auraInterruptFlags;
        uint32_t channelInterruptFlags;
        uint32_t procFlags;
        uint32_t procChance;
        uint32_t procCharges;
        uint32_t maxLevel;
        uint32_t baseLevel;
        uint32_t spellLevel;
        uint32_t durationIndex;
        uint32_t powerType;
        uint32_t manaCost;
        uint32_t manaCostPerlevel;
        uint32_t manaPerSecond;
        uint32_t manaPerSecondPerLevel;
        uint32_t rangeIndex;
        float speed;
        uint32_t modalNextSpell;
        uint32_t stackAmount;
        uint32_t totem[MAX_SPELL_TOTEMS];
        int32_t reagent[MAX_SPELL_REAGENTS];
        uint32_t reagentCount[MAX_SPELL_REAGENTS];
        int32_t equippedItemClass;
        int32_t equippedItemSubClassMask;
        int32_t equippedItemInventoryTypeMask;
        uint32_t effect[MAX_EFFECT_INDEX];
        int32_t effectDieSides[MAX_EFFECT_INDEX];
        uint32_t effectBaseDice[MAX_EFFECT_INDEX];
        float effectDicePerLevel[MAX_EFFECT_INDEX];
        float effectRealPointsPerLevel[MAX_EFFECT_INDEX];
        int32_t effectBasePoints[MAX_EFFECT_INDEX];
        uint32_t effectMechanic[MAX_EFFECT_INDEX];
        uint32_t effectImplicitTargetA[MAX_EFFECT_INDEX];
        uint32_t effectImplicitTargetB[MAX_EFFECT_INDEX];
        uint32_t effectRadiusIndex[MAX_EFFECT_INDEX];
        uint32_t effectApplyAuraName[MAX_EFFECT_INDEX];
        uint32_t effectAmplitude[MAX_EFFECT_INDEX];
        float effectMultipleValue[MAX_EFFECT_INDEX];
        uint32_t effectChainTarget[MAX_EFFECT_INDEX];
        uint32_t effectItemType[MAX_EFFECT_INDEX];
        int32_t effectMiscValue[MAX_EFFECT_INDEX];
        uint32_t effectTriggerSpell[MAX_EFFECT_INDEX];
        float effectPointsPerComboPoint[MAX_EFFECT_INDEX];
        uint32_t spellVisual;
        uint32_t spellVisual2;
        uint32_t spellIconID;
        uint32_t activeIconID;
        uint32_t spellPriority;
        char* spellName[8];
        uint32_t spellNameFlag;
        char* rank[8];
        uint32_t rankFlags;
        char* description[8];
        uint32_t descriptionFlags;
        char* toolTip[8];
        uint32_t toolTipFlags;
        uint32_t manaCostPercentage;
        uint32_t startRecoveryCategory;
        uint32_t startRecoveryTime;
        uint32_t maxTargetLevel;
        uint32_t spellFamilyName;
        uint64_t spellFamilyMask; // might be wrong size?
        uint32_t maxAffectedTargets;
        uint32_t dmgClass;
        uint32_t preventionType;
        uint32_t stanceBarOrder;
        float dmgMultiplier[MAX_EFFECT_INDEX];
        uint32_t minFactionId;
        uint32_t minReputation;
        uint32_t requiredAuraVision;
};

const SpellEntry& GetSpellEntry(uint32_t id);

std::string GetSpellName(uint32_t id);
