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
SDName: Boss Sartharion
SD%Complete: 70%
SDComment: Flame wave, achievement and portal events need to be implemented
SDCategory: Obsidian Sanctum
EndScriptData */

#include "precompiled.h"
#include "obsidian_sanctum.h"

enum
{
    // Sartharion Yell
    SAY_SARTHARION_AGGRO                        = -1615018,
    SAY_SARTHARION_BERSERK                      = -1615019,
    SAY_SARTHARION_BREATH                       = -1615020,
    SAY_SARTHARION_CALL_SHADRON                 = -1615021,
    SAY_SARTHARION_CALL_TENEBRON                = -1615022,
    SAY_SARTHARION_CALL_VESPERON                = -1615023,
    SAY_SARTHARION_DEATH                        = -1615024,
    SAY_SARTHARION_SPECIAL_1                    = -1615025,
    SAY_SARTHARION_SPECIAL_2                    = -1615026,
    SAY_SARTHARION_SPECIAL_3                    = -1615027,
    SAY_SARTHARION_SPECIAL_4                    = -1615028,
    SAY_SARTHARION_SLAY_1                       = -1615029,
    SAY_SARTHARION_SLAY_2                       = -1615030,
    SAY_SARTHARION_SLAY_3                       = -1615031,

    WHISPER_LAVA_CHURN                          = -1615032,

    WHISPER_SHADRON_DICIPLE                     = -1615008,
    WHISPER_VESPERON_DICIPLE                    = -1615041,
    WHISPER_HATCH_EGGS                          = -1615017,
    WHISPER_OPEN_PORTAL                         = -1615042, // whisper, shared by two dragons

    // Sartharion Spells
    SPELL_BERSERK                               = 61632,    // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
    SPELL_CLEAVE                                = 56909,    // Inflicts 35% weapon damage to an enemy and its nearest allies, affecting up to 10 targets.
    SPELL_FLAME_BREATH                          = 56908,    // Inflicts 8750 to 11250 Fire damage to enemies in a cone in front of the caster.
    SPELL_FLAME_BREATH_H                        = 58956,    // Inflicts 10938 to 14062 Fire damage to enemies in a cone in front of the caster.
    SPELL_TAIL_LASH                             = 56910,    // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TAIL_LASH_H                           = 58957,    // A sweeping tail strike hits all enemies behind the caster, inflicting 4375 to 5625 damage and stunning them for 2 sec.
    SPELL_WILL_OF_SARTHARION                    = 61254,    // Sartharion's presence bolsters the resolve of the Twilight Drakes, increasing their total health by 25%. This effect also increases Sartharion's health by 25%.
    SPELL_LAVA_STRIKE                           = 57571,    // (Real spell casted should be 57578) 57571 then trigger visual missile, then summon Lava Blaze on impact(spell 57572)
    SPELL_TWILIGHT_REVENGE                      = 60639,

    SPELL_PYROBUFFET                            = 56916,    // currently used for hard enrage after 15 minutes
    SPELL_PYROBUFFET_RANGE                      = 58907,    // possibly used when player get too far away from dummy creatures (2x creature entry 30494)

    SPELL_TWILIGHT_SHIFT_ENTER                  = 57620,    // enter phase. Player get this when click GO
    SPELL_TWILIGHT_SHIFT_REMOVAL                = 61187,    // leave phase
    SPELL_TWILIGHT_SHIFT_REMOVAL_ALL            = 61190,    // leave phase (probably version to make all leave)

    // Mini bosses common spells
    SPELL_TWILIGHT_RESIDUE                      = 61885,    // makes immune to shadow damage, applied when leave phase

    // Miniboses (Vesperon, Shadron, Tenebron)
    SPELL_SHADOW_BREATH_H                       = 59126,    // Inflicts 8788 to 10212 Fire damage to enemies in a cone in front of the caster.
    SPELL_SHADOW_BREATH                         = 57570,    // Inflicts 6938 to 8062 Fire damage to enemies in a cone in front of the caster.

    SPELL_SHADOW_FISSURE_H                      = 59127,    // Deals 9488 to 13512 Shadow damage to any enemy within the Shadow fissure after 5 sec.
    SPELL_SHADOW_FISSURE                        = 57579,    // Deals 6188 to 8812 Shadow damage to any enemy within the Shadow fissure after 5 sec.

    // Vesperon
    // In portal is a disciple, when disciple killed remove Power_of_vesperon, portal open multiple times
    NPC_DISCIPLE_OF_VESPERON                    = 30858,    // Disciple of Vesperon
    NPC_ACOLYTE_OF_VESPERON                     = 31219,    // Acolyte of Vesperon - summoned during Sartharion event
    SPELL_POWER_OF_VESPERON                     = 61251,    // Vesperon's presence decreases the maximum health of all enemies by 25%.
    SPELL_TWILIGHT_TORMENT_VESP                 = 57948,    // (Shadow only) trigger 57935 then 57988
    SPELL_TWILIGHT_TORMENT_VESP_ACO             = 58853,    // (Fire and Shadow) trigger 58835 then 57988

    // Shadron
    // In portal is a disciple, when disciple killed remove Power_of_vesperon, portal open multiple times
    NPC_DISCIPLE_OF_SHADRON                     = 30688,    // Disciple of Shadron
    NPC_ACOLYTE_OF_SHADRON                      = 31218,    // Acolyte of Shadron - summoned during Sartharion event
    SPELL_POWER_OF_SHADRON                      = 58105,    // Shadron's presence increases Fire damage taken by all enemies by 100%.
    SPELL_GIFT_OF_TWILIGTH_SHA                  = 57835,    // TARGET_SCRIPT shadron
    SPELL_GIFT_OF_TWILIGTH_SAR                  = 58766,    // TARGET_SCRIPT sartharion

    // Tenebron
    // in the portal spawns 6 eggs, if not killed in time (approx. 20s)  they will hatch,  whelps can cast 60708
    SPELL_POWER_OF_TENEBRON                     = 61248,    // Tenebron's presence increases Shadow damage taken by all enemies by 100%.
    // Tenebron, dummy spell
    SPELL_SUMMON_TWILIGHT_WHELP                 = 58035,    // doesn't work, will spawn NPC_TWILIGHT_WHELP
    SPELL_SUMMON_SARTHARION_TWILIGHT_WHELP      = 58826,    // doesn't work, will spawn NPC_SHARTHARION_TWILIGHT_WHELP

    NPC_TWILIGHT_EGG                            = 30882,    // Twilight Egg - summoned during Tenebron event
    NPC_SARTHARION_TWILIGHT_EGG                 = 31204,    // Twilight Egg - summoned during Sartharion event
    NPC_TWILIGHT_EGG_CONTROLLER                 = 31138,

    SPELL_HATCH_EGGS_H                          = 59189,
    SPELL_HATCH_EGGS                            = 58542,
    SPELL_HATCH_EGGS_EFFECT_H                   = 59190,
    SPELL_HATCH_EGGS_EFFECT                     = 58685,

    // Whelps
    NPC_TWILIGHT_WHELP                          = 30890,
    NPC_SHARTHARION_TWILIGHT_WHELP              = 31214,
    SPELL_FADE_ARMOR                            = 60708,    // Reduces the armor of an enemy by 1500 for 15s

    // flame tsunami
    SPELL_FLAME_TSUNAMI                         = 57494,    // the visual dummy
    SPELL_FLAME_TSUNAMI_LEAP                    = 60241,    // SPELL_EFFECT_138 some leap effect, causing caster to move in direction
    SPELL_FLAME_TSUNAMI_DMG_AURA                = 57492,    // periodic damage, npc has this aura

    NPC_FLAME_TSUNAMI                           = 30616,    // for the flame waves
    NPC_LAVA_BLAZE                              = 30643,    // adds spawning from flame strike

    NPC_VESPERON_CONTROLLER                     = 30878,
    NPC_VESPERON_CONTROLLER_DEBUFF_CLEAR        = 32694,
    NPC_SHADRON_PORTAL                          = 30741,
    NPC_SHADRON_PORTAL_VISUAL                   = 30650,

    // using these custom points for dragons start and end
    POINT_ID_INIT                               = 100,
    POINT_ID_LAND                               = 200
};

struct Waypoint
{
    float m_fX, m_fY, m_fZ, m_fO;
};

// each dragons special points. First where fly to before connect to connon, second where land point is.
Waypoint m_aTene[] =
{
    {3212.854f, 575.597f, 109.856f},                        // init
    {3246.425f, 565.367f, 61.249f}                          // end
};

Waypoint m_aShad[] =
{
    {3293.238f, 472.223f, 106.968f},
    {3271.669f, 526.907f, 61.931f}
};

Waypoint m_aVesp[] =
{
    {3193.310f, 472.861f, 102.697f},
    {3227.268f, 533.238f, 59.995f}
};

// points around raid "isle", counter clockwise. should probably be adjusted to be more alike
Waypoint m_aDragonCommon[] =
{
    {3214.012f, 468.932f, 98.652f},
    {3244.950f, 468.427f, 98.652f},
    {3283.520f, 496.869f, 98.652f},
    {3287.316f, 555.875f, 98.652f},
    {3250.479f, 585.827f, 98.652f},
    {3209.969f, 566.523f, 98.652f}
};

Waypoint m_aTsunamiLoc[] =
{
    // Note: this coords are guesswork, they might need to be updated.
    {3201.0f, 481.82f, 58.6f, 6.23f},                       // left to right
    {3201.0f, 524.50f, 58.6f, 6.23f},
    {3201.0f, 566.64f, 58.6f, 6.23f},
    {3287.5f, 545.50f, 58.6f, 3.19f},                       // right to left
    {3287.5f, 503.00f, 58.6f, 3.19f},
};

/*######
## Boss Sartharion
######*/

struct MANGOS_DLL_DECL boss_sartharionAI : public ScriptedAI
{
    boss_sartharionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsBerserk;
    bool m_bIsSoftEnraged;

    uint32 m_uiEnrageTimer;
    bool m_bIsHardEnraged;

    uint32 m_uiTenebronTimer;
    uint32 m_uiShadronTimer;
    uint32 m_uiVesperonTimer;

    uint32 m_uiFlameTsunamiTimer;
    uint32 m_uiFlameBreathTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiLavaStrikeTimer;

    bool m_bHasCalledTenebron;
    bool m_bHasCalledShadron;
    bool m_bHasCalledVesperon;

    void Reset() override
    {
        m_bIsBerserk            = false;
        m_bIsSoftEnraged        = false;

        m_uiEnrageTimer         = 15 * MINUTE * IN_MILLISECONDS;
        m_bIsHardEnraged        = false;

        m_uiTenebronTimer       = 30000;
        m_uiShadronTimer        = 75000;
        m_uiVesperonTimer       = 120000;

        m_uiFlameTsunamiTimer   = 30000;
        m_uiFlameBreathTimer    = 20000;
        m_uiTailSweepTimer      = 20000;
        m_uiCleaveTimer         = 7000;
        m_uiLavaStrikeTimer     = 5000;

        m_bHasCalledTenebron    = false;
        m_bHasCalledShadron     = false;
        m_bHasCalledVesperon    = false;
    }

    void Aggro(Unit* /*pWho*/) override
    {
        DoScriptText(SAY_SARTHARION_AGGRO, m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, IN_PROGRESS);
            FetchDragons();
        }
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_SARTHARION_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, DONE);
    }

    void KilledUnit(Unit* /*pVictim*/) override
    {
        switch (urand(0, 2))
        {
            case 0: DoScriptText(SAY_SARTHARION_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SARTHARION_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SARTHARION_SLAY_3, m_creature); break;
        }
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, FAIL);

        // Despawn portal
        if (GameObject* pPortal = GetClosestGameObjectWithEntry(m_creature, GO_TWILIGHT_PORTAL, 50.0f))
            pPortal->SetLootState(GO_JUST_DEACTIVATED);
    }

    void FetchDragons()
    {
        Creature* pTene = m_pInstance->GetSingleCreatureFromStorage(NPC_TENEBRON);
        Creature* pShad = m_pInstance->GetSingleCreatureFromStorage(NPC_SHADRON);
        Creature* pVesp = m_pInstance->GetSingleCreatureFromStorage(NPC_VESPERON);

        // if at least one of the dragons are alive and are being called
        uint8 uiCountFetchableDragons = 0;

        if (pTene && pTene->isAlive() && !pTene->getVictim())
        {
            ++uiCountFetchableDragons;
            pTene->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aTene[0].m_fX, m_aTene[0].m_fY, m_aTene[0].m_fZ);

            if (!pTene->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pTene->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (pShad && pShad->isAlive() && !pShad->getVictim())
        {
            ++uiCountFetchableDragons;
            pShad->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aShad[0].m_fX, m_aShad[0].m_fY, m_aShad[0].m_fZ);

            if (!pShad->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pShad->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (pVesp && pVesp->isAlive() && !pVesp->getVictim())
        {
            ++uiCountFetchableDragons;
            pVesp->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aVesp[0].m_fX, m_aVesp[0].m_fY, m_aVesp[0].m_fZ);

            if (!pVesp->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pVesp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (uiCountFetchableDragons)
            DoCastSpellIfCan(m_creature, SPELL_WILL_OF_SARTHARION);

        m_pInstance->SetData(TYPE_ALIVE_DRAGONS, uiCountFetchableDragons);
    }

    void CallDragon(uint32 uiEntry)
    {
        if (m_pInstance)
        {
            Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(uiEntry);
            if (pTemp && pTemp->isAlive())
            {
                if (pTemp->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                    pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                if (pTemp->getVictim())
                    return;

                pTemp->SetWalk(false);

                int32 iTextId = 0;

                switch (uiEntry)
                {
                    case NPC_TENEBRON:
                        iTextId = SAY_SARTHARION_CALL_TENEBRON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aTene[1].m_fX, m_aTene[1].m_fY, m_aTene[1].m_fZ);
                        break;
                    case NPC_SHADRON:
                        iTextId = SAY_SARTHARION_CALL_SHADRON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aShad[1].m_fX, m_aShad[1].m_fY, m_aShad[1].m_fZ);
                        break;
                    case NPC_VESPERON:
                        iTextId = SAY_SARTHARION_CALL_VESPERON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aVesp[1].m_fX, m_aVesp[1].m_fY, m_aVesp[1].m_fZ);
                        break;
                }

                DoScriptText(iTextId, m_creature);
            }
        }
    }

    void SendFlameTsunami()
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const& PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive())
                        DoScriptText(WHISPER_LAVA_CHURN, m_creature, i->getSource());
                }
            }
        }

        uint8 uiTsunamiStartLoc = 0;
        uint8 uiTsunamiEndLoc = 3;
        if (urand(0, 1))
        {
            uiTsunamiStartLoc = 3;
            uiTsunamiEndLoc = 5;
        }

        for (uint8 i = uiTsunamiStartLoc; i < uiTsunamiEndLoc; ++i)
            m_creature->SummonCreature(NPC_FLAME_TSUNAMI, m_aTsunamiLoc[i].m_fX, m_aTsunamiLoc[i].m_fY, m_aTsunamiLoc[i].m_fZ, m_aTsunamiLoc[i].m_fO, TEMPSUMMON_TIMED_DESPAWN, 15000);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // spell will target dragons, if they are still alive at 35%
        if (!m_bIsBerserk && m_creature->GetHealthPercent() < 35.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_SARTHARION_BERSERK, m_creature);
                m_bIsBerserk = true;
            }
        }

        // soft enrage
        if (!m_bIsSoftEnraged && m_creature->GetHealthPercent() <= 10.0f)
        {
            // TODO
            m_bIsSoftEnraged = true;
        }

        // hard enrage
        if (!m_bIsHardEnraged)
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_PYROBUFFET, CAST_TRIGGERED) == CAST_OK)
                    m_bIsHardEnraged = true;
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }

        // flame tsunami
        if (m_uiFlameTsunamiTimer < uiDiff)
        {
            SendFlameTsunami();
            m_uiFlameTsunamiTimer = 30000;
        }
        else
            m_uiFlameTsunamiTimer -= uiDiff;

        // flame breath
        if (m_uiFlameBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FLAME_BREATH : SPELL_FLAME_BREATH_H) == CAST_OK)
            {
                DoScriptText(SAY_SARTHARION_BREATH, m_creature);
                m_uiFlameBreathTimer = urand(25000, 35000);
            }
        }
        else
            m_uiFlameBreathTimer -= uiDiff;

        // Tail Sweep
        if (m_uiTailSweepTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TAIL_LASH : SPELL_TAIL_LASH_H) == CAST_OK)
                m_uiTailSweepTimer = urand(15000, 20000);
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = urand(7000, 10000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Lavas Strike
        if (m_uiLavaStrikeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, SPELL_LAVA_STRIKE);

                switch (urand(0, 15))
                {
                    case 0: DoScriptText(SAY_SARTHARION_SPECIAL_1, m_creature); break;
                    case 1: DoScriptText(SAY_SARTHARION_SPECIAL_2, m_creature); break;
                    case 2: DoScriptText(SAY_SARTHARION_SPECIAL_3, m_creature); break;
                }
            }
            m_uiLavaStrikeTimer = urand(5000, 20000);
        }
        else
            m_uiLavaStrikeTimer -= uiDiff;

        // call tenebron
        if (!m_bHasCalledTenebron && m_uiTenebronTimer < uiDiff)
        {
            CallDragon(NPC_TENEBRON);
            m_bHasCalledTenebron = true;
        }
        else
            m_uiTenebronTimer -= uiDiff;

        // call shadron
        if (!m_bHasCalledShadron && m_uiShadronTimer < uiDiff)
        {
            CallDragon(NPC_SHADRON);
            m_bHasCalledShadron = true;
        }
        else
            m_uiShadronTimer -= uiDiff;

        // call vesperon
        if (!m_bHasCalledVesperon && m_uiVesperonTimer < uiDiff)
        {
            CallDragon(NPC_VESPERON);
            m_bHasCalledVesperon = true;
        }
        else
            m_uiVesperonTimer -= uiDiff;

        DoMeleeAttackIfReady();

        EnterEvadeIfOutOfCombatArea(uiDiff);
    }
};

CreatureAI* GetAI_boss_sartharion(Creature* pCreature)
{
    return new boss_sartharionAI(pCreature);
}

enum TeneText
{
    SAY_TENEBRON_AGGRO                      = -1615009,
    SAY_TENEBRON_SLAY_1                     = -1615010,
    SAY_TENEBRON_SLAY_2                     = -1615011,
    SAY_TENEBRON_DEATH                      = -1615012,
    SAY_TENEBRON_BREATH                     = -1615013,
    SAY_TENEBRON_RESPOND                    = -1615014,
    SAY_TENEBRON_SPECIAL_1                  = -1615015,
    SAY_TENEBRON_SPECIAL_2                  = -1615016
};

enum ShadText
{
    SAY_SHADRON_AGGRO                       = -1615000,
    SAY_SHADRON_SLAY_1                      = -1615001,
    SAY_SHADRON_SLAY_2                      = -1615002,
    SAY_SHADRON_DEATH                       = -1615003,
    SAY_SHADRON_BREATH                      = -1615004,
    SAY_SHADRON_RESPOND                     = -1615005,
    SAY_SHADRON_SPECIAL_1                   = -1615006,
    SAY_SHADRON_SPECIAL_2                   = -1615007
};

enum VespText
{
    SAY_VESPERON_AGGRO                      = -1615033,
    SAY_VESPERON_SLAY_1                     = -1615034,
    SAY_VESPERON_SLAY_2                     = -1615035,
    SAY_VESPERON_DEATH                      = -1615036,
    SAY_VESPERON_BREATH                     = -1615037,
    SAY_VESPERON_RESPOND                    = -1615038,
    SAY_VESPERON_SPECIAL_1                  = -1615039,
    SAY_VESPERON_SPECIAL_2                  = -1615040
};

// to control each dragons common abilities
struct MANGOS_DLL_DECL dummy_dragonAI : public ScriptedAI
{
    dummy_dragonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWaypointId;
    uint32 m_uiMoveNextTimer;
    bool m_bCanMoveFree;

    uint32 m_iPortalRespawnTime;
    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;

    void Reset() override
    {
        m_uiWaypointId          = 0;
        m_uiMoveNextTimer       = 500;
        m_bCanMoveFree          = false;

        m_iPortalRespawnTime    = 30000;
        m_uiShadowBreathTimer   = 20000;
        m_uiShadowFissureTimer  = 5000;
    }

    void JustReachedHome() override
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        // Despawn portal
        if (GameObject* pPortal = GetClosestGameObjectWithEntry(m_creature, GO_TWILIGHT_PORTAL, 50.0f))
            pPortal->SetLootState(GO_JUST_DEACTIVATED);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId) override
    {
        if (!m_pInstance || uiType != POINT_MOTION_TYPE)
            return;

        debug_log("dummy_dragonAI: %s reached point %u", m_creature->GetName(), uiPointId);

        // if healers messed up the raid and we was already initialized
        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            EnterEvadeMode();
            return;
        }

        // this is the end (!)
        if (uiPointId == POINT_ID_LAND)
        {
            m_creature->GetMotionMaster()->Clear();
            m_bCanMoveFree = false;
            m_creature->SetInCombatWithZone();
            return;
        }

        // increase
        m_uiWaypointId = uiPointId + 1;

        // if we have reached a point bigger or equal to count, it mean we must reset to point 0
        if (m_uiWaypointId >= countof(m_aDragonCommon))
        {
            if (!m_bCanMoveFree)
                m_bCanMoveFree = true;

            m_uiWaypointId = 0;
        }

        m_uiMoveNextTimer = 500;
    }

    // used when open portal and spawn mobs in phase
    void DoRaidWhisper(int32 iTextId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const& PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    DoScriptText(iTextId, m_creature, i->getSource());
            }
        }
    }

    //"opens" the portal and does the "opening" whisper
    void DoOpenPortal()
    {
        int32 iTextId = 0;

        // there are 4 portal spawn locations, each are expected to be spawned with negative spawntimesecs in database

        // using a grid search here seem to be more efficient than caching all four guids
        // in instance script and calculate range to each.
        GameObject* pPortal = GetClosestGameObjectWithEntry(m_creature, GO_TWILIGHT_PORTAL, 50.0f);

        switch (m_creature->GetEntry())
        {
            case NPC_TENEBRON:
                iTextId = WHISPER_HATCH_EGGS;
                break;
            case NPC_SHADRON:
            case NPC_VESPERON:
                iTextId = WHISPER_OPEN_PORTAL;
                break;
        }

        DoRaidWhisper(iTextId);

        // By using SetRespawnTime() we will actually "spawn" the object with our defined time.
        // Once time is up, portal will disappear again.
        if (pPortal && !pPortal->isSpawned())
        {
            pPortal->SetRespawnTime(m_iPortalRespawnTime);
            pPortal->Refresh();
        }

        // Unclear what are expected to happen if one drake has a portal open already
        // Refresh respawnTime so time again are set to 30secs?
    }

    // Removes each drakes unique debuff from players
    void RemoveDebuff(uint32 uiSpellId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const& PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && i->getSource()->HasAura(uiSpellId))
                    i->getSource()->RemoveAurasDueToSpell(uiSpellId);
            }
        }
    }

    // Handle breath yell
    virtual void DoHandleBreathYell() { }

    // Handle special events for each dragon
    virtual void UpdateDragonAI(const uint32 /*uiDiff*/) { }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_bCanMoveFree && m_uiMoveNextTimer)
        {
            if (m_uiMoveNextTimer <= uiDiff)
            {
                m_creature->GetMotionMaster()->MovePoint(m_uiWaypointId,
                        m_aDragonCommon[m_uiWaypointId].m_fX, m_aDragonCommon[m_uiWaypointId].m_fY, m_aDragonCommon[m_uiWaypointId].m_fZ);

                debug_log("dummy_dragonAI: %s moving to point %u", m_creature->GetName(), m_uiWaypointId);
                m_uiMoveNextTimer = 0;
            }
            else
                m_uiMoveNextTimer -= uiDiff;
        }

        // if no target return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Call dragon specific virtual function
        UpdateDragonAI(uiDiff);

        // respawn portal
        if (m_iPortalRespawnTime)
        {
            if (m_iPortalRespawnTime <= uiDiff)
            {
                DoOpenPortal();
                m_iPortalRespawnTime = 0;
            }
            else
                m_iPortalRespawnTime -= uiDiff;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H) == CAST_OK)
                    m_uiShadowFissureTimer = urand(15000, 20000);
            }
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H) == CAST_OK)
            {
                DoHandleBreathYell();
                m_uiShadowBreathTimer = urand(20000, 25000);
            }
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## Mob Tenebron
######*/

struct MANGOS_DLL_DECL mob_tenebronAI : public dummy_dragonAI
{
    mob_tenebronAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    uint32 m_uiHatchEggTimer;

    void Reset() override
    {
        m_uiHatchEggTimer = 30000;

        dummy_dragonAI::Reset();
    }

    void Aggro(Unit* /*pWho*/) override
    {
        DoScriptText(SAY_TENEBRON_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_POWER_OF_TENEBRON);
    }

    void KilledUnit(Unit* /*pVictim*/) override
    {
        DoScriptText(urand(0, 1) ? SAY_TENEBRON_SLAY_1 : SAY_TENEBRON_SLAY_2, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_TENEBRON_DEATH, m_creature);

        // Cast Twilight Revent - script target on Sartharion
        if (m_pInstance && m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            DoCastSpellIfCan(m_creature, SPELL_TWILIGHT_REVENGE, CAST_TRIGGERED);
    }

    void DoHandleBreathYell()
    {
        DoScriptText(SAY_TENEBRON_BREATH, m_creature);
    }

    void UpdateDragonAI(const uint32 uiDiff)
    {
        if (m_uiHatchEggTimer < uiDiff)
        {
            // ToDo:
            m_uiHatchEggTimer = 30000;
        }
        else
            m_uiHatchEggTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_tenebron(Creature* pCreature)
{
    return new mob_tenebronAI(pCreature);
}

/*######
## Mob Shadron
######*/

struct MANGOS_DLL_DECL mob_shadronAI : public dummy_dragonAI
{
    mob_shadronAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    uint32 m_uiAcolyteShadronTimer;

    void Reset() override
    {
        m_uiAcolyteShadronTimer = 60000;

        if (m_creature->HasAura(SPELL_TWILIGHT_TORMENT_VESP))
            m_creature->RemoveAurasDueToSpell(SPELL_TWILIGHT_TORMENT_VESP);

        if (m_creature->HasAura(SPELL_GIFT_OF_TWILIGTH_SHA))
            m_creature->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SHA);

        dummy_dragonAI::Reset();
    }

    void Aggro(Unit* /*pWho*/) override
    {
        DoScriptText(SAY_SHADRON_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_POWER_OF_SHADRON);
    }

    void KilledUnit(Unit* /*pVictim*/) override
    {
        DoScriptText(urand(0, 1) ? SAY_SHADRON_SLAY_1 : SAY_SHADRON_SLAY_2, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_SHADRON_DEATH, m_creature);

        // Cast Twilight Revent - script target on Sartharion
        if (m_pInstance && m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            DoCastSpellIfCan(m_creature, SPELL_TWILIGHT_REVENGE, CAST_TRIGGERED);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_DISCIPLE_OF_SHADRON)
            pSummoned->CastSpell(pSummoned, SPELL_GIFT_OF_TWILIGTH_SHA, true);
        else if (pSummoned->GetEntry() == NPC_ACOLYTE_OF_SHADRON)
            pSummoned->CastSpell(pSummoned, SPELL_GIFT_OF_TWILIGTH_SAR, true);
    }

    void SummonedCreatureJustDied(Creature* pSummoned) override
    {
        pSummoned->CastSpell(pSummoned, SPELL_TWILIGHT_SHIFT_REMOVAL_ALL, true);
    }

    void DoHandleBreathYell()
    {
        DoScriptText(SAY_SHADRON_BREATH, m_creature);
    }

    void UpdateDragonAI(const uint32 uiDiff) override
    {
        if (m_uiAcolyteShadronTimer < uiDiff)
        {
            // ToDo:
            m_uiAcolyteShadronTimer = 60000;
        }
        else
            m_uiAcolyteShadronTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_shadron(Creature* pCreature)
{
    return new mob_shadronAI(pCreature);
}

/*######
## Mob Vesperon
######*/

struct MANGOS_DLL_DECL mob_vesperonAI : public dummy_dragonAI
{
    mob_vesperonAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    uint32 m_uiAcolyteVesperonTimer;

    void Reset() override
    {
        m_uiAcolyteVesperonTimer = 60000;

        dummy_dragonAI::Reset();
    }

    void Aggro(Unit* /*pWho*/) override
    {
        DoScriptText(SAY_VESPERON_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_POWER_OF_VESPERON);
    }

    void KilledUnit(Unit* /*pVictim*/) override
    {
        DoScriptText(urand(0, 1) ? SAY_VESPERON_SLAY_1 : SAY_VESPERON_SLAY_2, m_creature);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        DoScriptText(SAY_VESPERON_DEATH, m_creature);

        // Cast Twilight Revent - script target on Sartharion
        if (m_pInstance && m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            DoCastSpellIfCan(m_creature, SPELL_TWILIGHT_REVENGE, CAST_TRIGGERED);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_DISCIPLE_OF_VESPERON)
            pSummoned->CastSpell(pSummoned, SPELL_TWILIGHT_TORMENT_VESP, true);
        else if (pSummoned->GetEntry() == NPC_ACOLYTE_OF_VESPERON)
            pSummoned->CastSpell(pSummoned, SPELL_TWILIGHT_TORMENT_VESP_ACO, true);
    }

    void SummonedCreatureJustDied(Creature* pSummoned) override
    {
        // ToDo: remove Twilight Torment debuff
        pSummoned->CastSpell(pSummoned, SPELL_TWILIGHT_SHIFT_REMOVAL_ALL, true);
    }

    void DoHandleBreathYell()
    {
        DoScriptText(SAY_VESPERON_BREATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiAcolyteVesperonTimer < uiDiff)
        {
            // ToDo:
            m_uiAcolyteVesperonTimer = 60000;
        }
        else
            m_uiAcolyteVesperonTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_vesperon(Creature* pCreature)
{
    return new mob_vesperonAI(pCreature);
}

/*######
## Mob Twilight Eggs
######*/

struct MANGOS_DLL_DECL mob_twilight_eggsAI : public ScriptedAI
{
    mob_twilight_eggsAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() override
    {
    }

    void AttackStart(Unit* /*pWho*/) override { }
    void MoveInLineOfSight(Unit* /*pWho*/) override { }
};

CreatureAI* GetAI_mob_twilight_eggs(Creature* pCreature)
{
    return new mob_twilight_eggsAI(pCreature);
}

/*######
## Mob Twilight Whelps
######*/

struct MANGOS_DLL_DECL mob_twilight_whelpAI : public ScriptedAI
{
    mob_twilight_whelpAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiFadeArmorTimer;

    void Reset() override
    {
        m_uiFadeArmorTimer = 1000;
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // twilight torment
        if (m_uiFadeArmorTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FADE_ARMOR) == CAST_OK)
                m_uiFadeArmorTimer = urand(5000, 10000);
        }
        else
            m_uiFadeArmorTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_twilight_whelp(Creature* pCreature)
{
    return new mob_twilight_whelpAI(pCreature);
}

/*######
## npc_flame_tsunami
######*/

struct MANGOS_DLL_DECL npc_flame_tsunamiAI : public ScriptedAI
{
    npc_flame_tsunamiAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiTsunamiTimer;

    void Reset()
    {
        m_uiTsunamiTimer = 2000;

        DoCastSpellIfCan(m_creature, SPELL_FLAME_TSUNAMI, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_FLAME_TSUNAMI_DMG_AURA, CAST_TRIGGERED);
    }

    void AttackStart(Unit* /*pWho*/) override { }
    void MoveInLineOfSight(Unit* /*pWho*/) override { }

    void MovementInform(uint32 uiType, uint32 uiPointId) override
    {
        if (uiType != POINT_MOTION_TYPE || !uiPointId)
            return;

        m_creature->RemoveAllAurasOnEvade();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiTsunamiTimer)
        {
            if (m_uiTsunamiTimer <= uiDiff)
            {
                // Note: currently the way in which spell 60241 works is unk, so for the moment we'll use simple movement
                m_creature->SetWalk(false);
                m_creature->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX() < 3250.0f ? m_creature->GetPositionX() + 86.5f : m_creature->GetPositionX() - 86.5f,
                    m_creature->GetPositionY(), m_creature->GetPositionZ());

                m_uiTsunamiTimer = 0;
            }
            else
                m_uiTsunamiTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_flame_tsunami(Creature* pCreature)
{
    return new npc_flame_tsunamiAI(pCreature);
}

void AddSC_boss_sartharion()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_sartharion";
    pNewScript->GetAI = &GetAI_boss_sartharion;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_vesperon";
    pNewScript->GetAI = &GetAI_mob_vesperon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_shadron";
    pNewScript->GetAI = &GetAI_mob_shadron;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_tenebron";
    pNewScript->GetAI = &GetAI_mob_tenebron;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_twilight_eggs";
    pNewScript->GetAI = &GetAI_mob_twilight_eggs;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_twilight_whelp";
    pNewScript->GetAI = &GetAI_mob_twilight_whelp;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_flame_tsunami";
    pNewScript->GetAI = &GetAI_npc_flame_tsunami;
    pNewScript->RegisterSelf();
}
