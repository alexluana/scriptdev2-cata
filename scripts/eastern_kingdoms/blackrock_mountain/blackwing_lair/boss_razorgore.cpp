/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Razorgore
SD%Complete: 50
SDComment: Needs additional review. Phase 1 NYI (Grethok the Controller), Conflagration needs core support
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"

enum
{
    SAY_EGGS_BROKEN_1           = -1469022,
    SAY_EGGS_BROKEN_2           = -1469023,
    SAY_EGGS_BROKEN_3           = -1469024,
    SAY_DEATH                   = -1469025,

    EMOTE_TROOPS_FLEE           = -1469033,                 // emote by Nefarian's Troops npc

    // phase I event spells
    SPELL_POSSESS               = 23014,                    // visual effect and increase the damage taken
    SPELL_EXPLODE_ORB           = 20037,                    // used if attacked without destroying the eggs - related to 20038

    SPELL_CLEAVE                = 19632,
    SPELL_WARSTOMP              = 24375,
    SPELL_FIREBALL_VOLLEY       = 22425,
    SPELL_CONFLAGRATION         = 23023,

    // npcs used in phase I
    NPC_BLACKWING_LEGIONNAIRE   = 12416,
    NPC_BLACKWING_MAGE          = 12420,
    NPC_DRAGONSPAWN             = 12422,
};

struct MANGOS_DLL_DECL boss_razorgoreAI : public ScriptedAI
{
    boss_razorgoreAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_blackwing_lair*)pCreature->GetInstanceData();
        Reset();
    }

    instance_blackwing_lair* m_pInstance;

    uint32 m_uiIntroVisualTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiWarStompTimer;
    uint32 m_uiFireballVolleyTimer;
    uint32 m_uiConflagrationTimer;

    void Reset() override
    {
        m_uiIntroVisualTimer    = 5000;

        m_uiCleaveTimer         = 15000;                    // These times are probably wrong
        m_uiWarStompTimer       = 35000;
        m_uiConflagrationTimer  = 12000;
        m_uiFireballVolleyTimer = 7000;
    }

    void Aggro(Unit* pWho) override
    {
        // TODO Temporarily add this InstData setting, must be started with Phase 1 which is not yet implemented
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORGORE, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORGORE, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORGORE, FAIL);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        // Set visual on OOC timer
        if (m_uiIntroVisualTimer)
        {
            if (m_uiIntroVisualTimer <= uiDiff)
            {
                if (Creature* pOrbTrigger = m_pInstance->GetSingleCreatureFromStorage(NPC_BLACKWING_ORB_TRIGGER))
                    pOrbTrigger->CastSpell(m_creature, SPELL_POSSESS, false);
                m_uiIntroVisualTimer = 0;
            }
            else
                m_uiIntroVisualTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = urand(7000, 10000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // War Stomp
        if (m_uiWarStompTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WARSTOMP) == CAST_OK)
                m_uiWarStompTimer = urand(15000, 25000);
        }
        else
            m_uiWarStompTimer -= uiDiff;

        // Fireball Volley
        if (m_uiFireballVolleyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FIREBALL_VOLLEY) == CAST_OK)
                m_uiFireballVolleyTimer = urand(12000, 15000);
        }
        else
            m_uiFireballVolleyTimer -= uiDiff;

        // Conflagration
        if (m_uiConflagrationTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_CONFLAGRATION) == CAST_OK)
                m_uiConflagrationTimer = 12000;
        }
        else
            m_uiConflagrationTimer -= uiDiff;

        /* This is obsolete code, not working anymore, keep as reference, should be handled in core though
        * // Aura Check. If the gamer is affected by confliguration we attack a random gamer.
        * if (m_creature->getVictim()->HasAura(SPELL_CONFLAGRATION, EFFECT_INDEX_0))
        * {
        *     if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
        *         m_creature->TauntApply(pTarget);
        * }
        */

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_razorgore(Creature* pCreature)
{
    return new boss_razorgoreAI(pCreature);
}

void AddSC_boss_razorgore()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_razorgore";
    pNewScript->GetAI = &GetAI_boss_razorgore;
    pNewScript->RegisterSelf();
}
