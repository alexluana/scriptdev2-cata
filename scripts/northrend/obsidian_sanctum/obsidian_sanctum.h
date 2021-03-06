/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_OBSIDIAN_SANCTUM_H
#define DEF_OBSIDIAN_SANCTUM_H

enum
{
    MAX_ENCOUNTER               = 1,

    TYPE_SARTHARION_EVENT       = 1,
    // internal used types for achievement
    TYPE_ALIVE_DRAGONS          = 2,

    NPC_SARTHARION              = 28860,
    NPC_TENEBRON                = 30452,
    NPC_SHADRON                 = 30451,
    NPC_VESPERON                = 30449,

    GO_TWILIGHT_PORTAL          = 193988,

    // Achievement related
    ACHIEV_CRIT_VOLCANO_BLOW_N  = 7326,                     // achievs 2047, 2048 (Go When the Volcano Blows) -- This is individual achievement!
    ACHIEV_CRIT_VOLCANO_BLOW_H  = 7327,
    ACHIEV_DRAGONS_ALIVE_1_N    = 7328,                     // achievs 2049, 2052 (Twilight Assist)
    ACHIEV_DRAGONS_ALIVE_1_H    = 7331,
    ACHIEV_DRAGONS_ALIVE_2_N    = 7329,                     // achievs 2050, 2053 (Twilight Duo)
    ACHIEV_DRAGONS_ALIVE_2_H    = 7332,
    ACHIEV_DRAGONS_ALIVE_3_N    = 7330,                     // achievs 2051, 2054 (The Twilight Zone)
    ACHIEV_DRAGONS_ALIVE_3_H    = 7333,
};

class MANGOS_DLL_DECL instance_obsidian_sanctum : public ScriptedInstance
{
    public:
        instance_obsidian_sanctum(Map* pMap);

        void Initialize() override;

        void OnCreatureCreate(Creature* pCreature) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiType) const override;

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/) const override;
        bool CheckConditionCriteriaMeet(Player const* pPlayer, uint32 uiInstanceConditionId, WorldObject const* pConditionSource, uint32 conditionSourceType) const override;

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];

        uint8 m_uiAliveDragons;
};

#endif
