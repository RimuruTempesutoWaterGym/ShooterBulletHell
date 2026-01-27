
#include "waveManager.h"
#include "ennemyManager.h"
#include "tools.h"
#include <stdlib.h>

void initWaveManager(WaveManager* manager)
{
    manager->currentEvent = NULL;
    manager->eventList = NULL;
    manager->timer = 0.0f;
    manager->spawnedCount = 0;
    manager->currentWave = 0;
    manager->waveActive = sfFalse;
}

void addMinionSpawn(WaveManager* manager, MinionType minionType, float delay, int count, sfVector2f pos, float interval)
{
    SpawnEvent* newEvent = (SpawnEvent*)calloc(1, sizeof(SpawnEvent));
    newEvent->type = SPAWN_MINION;
    newEvent->minionType = minionType;
    newEvent->delay = delay;
    newEvent->count = count;
    newEvent->position = pos;
    newEvent->interval = interval;
    newEvent->pNext = NULL;

    // Add to end of list
    if (manager->eventList == NULL)
    {
        manager->eventList = newEvent;
    }
    else
    {
        SpawnEvent* temp = manager->eventList;
        while (temp->pNext != NULL)
        {
            temp = temp->pNext;
        }
        temp->pNext = newEvent;
    }
}

void addBossSpawn(WaveManager* manager, BossType bossType, float delay, sfVector2f pos)
{
    SpawnEvent* newEvent = (SpawnEvent*)calloc(1, sizeof(SpawnEvent));
    newEvent->type = SPAWN_BOSS;
    newEvent->bossType = bossType;
    newEvent->delay = delay;
    newEvent->count = 1;
    newEvent->position = pos;
    newEvent->interval = 0.0f;
    newEvent->pNext = NULL;

    if (manager->eventList == NULL)
    {
        manager->eventList = newEvent;
    }
    else
    {
        SpawnEvent* temp = manager->eventList;
        while (temp->pNext != NULL)
        {
            temp = temp->pNext;
        }
        temp->pNext = newEvent;
    }
}

void addWaitEvent(WaveManager* manager, float delay)
{
    SpawnEvent* newEvent = (SpawnEvent*)calloc(1, sizeof(SpawnEvent));
    newEvent->type = SPAWN_WAIT;
    newEvent->delay = delay;
    newEvent->count = 0;
    newEvent->pNext = NULL;

    if (manager->eventList == NULL)
    {
        manager->eventList = newEvent;
    }
    else
    {
        SpawnEvent* temp = manager->eventList;
        while (temp->pNext != NULL)
        {
            temp = temp->pNext;
        }
        temp->pNext = newEvent;
    }
}

void addCompleteEvent(WaveManager* manager)
{
    SpawnEvent* newEvent = (SpawnEvent*)calloc(1, sizeof(SpawnEvent));
    newEvent->type = SPAWN_COMPLETE;
    newEvent->delay = 0.0f;
    newEvent->count = 0;
    newEvent->pNext = NULL;

    if (manager->eventList == NULL)
    {
        manager->eventList = newEvent;
    }
    else
    {
        SpawnEvent* temp = manager->eventList;
        while (temp->pNext != NULL)
        {
            temp = temp->pNext;
        }
        temp->pNext = newEvent;
    }
}

void updateWaveManager(WaveManager* manager, sfRenderWindow* window)
{
    if (!manager->waveActive || manager->currentEvent == NULL)
        return;

    manager->timer += getDeltaTime();
    SpawnEvent* event = manager->currentEvent;

    switch (event->type)
    {
    case SPAWN_MINION:
    {
        if (manager->timer >= event->delay)
        {
            float spawnTime = event->delay + (manager->spawnedCount * event->interval);

            if (manager->timer >= spawnTime && manager->spawnedCount < event->count)
            {
                // Spawn different minion types
                switch (event->minionType)
                {
                case MINION_TYPE_BASIC:
                    prepareEnnemy(window);
                    break;
                case MINION_TYPE_FAST:
                    prepareFastEnnemy(window);
                    break;
                case MINION_TYPE_TANK:
                    prepareTankEnnemy(window);
                    break;
                case MINION_TYPE_SHOOTER:
                    prepareShooterEnnemy(window);
                    break;
                }

                manager->spawnedCount++;
            }

            if (manager->spawnedCount >= event->count)
            {
                manager->currentEvent = event->pNext;
                manager->timer = 0.0f;
                manager->spawnedCount = 0;
            }
        }
        break;
    }

    case SPAWN_BOSS:
    {
        if (manager->timer >= event->delay)
        {
            if (manager->spawnedCount == 0)
            {
                // Spawn different boss types
                switch (event->bossType)
                {
                case BOSS_TYPE_JUNKO:
                    prepareJunkoBoss(window);
                    break;
                case BOSS_TYPE_REMILIA:
                    prepareRemiliaBoss(window);
                    break;
                case BOSS_TYPE_FLANDRE:
                    prepareFlandreBoss(window);
                    break;
                case BOSS_TYPE_YUKARI:
                    prepareYukariBoss(window);
                    break;
                }
                manager->spawnedCount++;
            }

            manager->currentEvent = event->pNext;
            manager->timer = 0.0f;
            manager->spawnedCount = 0;
        }
        break;
    }

    case SPAWN_WAIT:
    {
        if (manager->timer >= event->delay)
        {
            manager->currentEvent = event->pNext;
            manager->timer = 0.0f;
        }
        break;
    }

    case SPAWN_COMPLETE:
    {
        manager->waveActive = sfFalse;
        break;
    }
    }
}

void startWave(WaveManager* manager, int waveNumber)
{
    clearWaveManager(manager);
    manager->currentWave = waveNumber;
    manager->waveActive = sfTrue;
    manager->timer = 0.0f;
    manager->spawnedCount = 0;

    switch (waveNumber)
    {
    case 1:  // 3 basic minions → 3 fast minions → Junko
    {
        addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 3, (sfVector2f) { 960, -100 }, 1.5f);
        addWaitEvent(manager, 8.0f);

        addMinionSpawn(manager, MINION_TYPE_FAST, 0.0f, 3, (sfVector2f) { -100, 200 }, 1.0f);
        addWaitEvent(manager, 8.0f);

        addBossSpawn(manager, BOSS_TYPE_JUNKO, 2.0f, (sfVector2f) { 960, -200 });
        addCompleteEvent(manager);
        break;
    }

    case 2:  // 3 shooters → 4 tanks → 3 fast → Remilia
    {
        addMinionSpawn(manager, MINION_TYPE_SHOOTER, 0.0f, 3, (sfVector2f) { 400, -100 }, 1.5f);
        addWaitEvent(manager, 9.0f);

        addMinionSpawn(manager, MINION_TYPE_TANK, 0.0f, 4, (sfVector2f) { 1520, -100 }, 1.2f);
        addWaitEvent(manager, 10.0f);

        addMinionSpawn(manager, MINION_TYPE_FAST, 0.0f, 3, (sfVector2f) { 960, -100 }, 0.8f);
        addWaitEvent(manager, 8.0f);

        addBossSpawn(manager, BOSS_TYPE_REMILIA, 3.0f, (sfVector2f) { 960, -200 });
        addCompleteEvent(manager);
        break;
    }

    case 3:  // Mixed assault → Flandre
    {
        // Left side: 3 basic
        addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 3, (sfVector2f) { -100, 150 }, 1.0f);
        // Right side: 2 shooters (simultaneous!)
        addMinionSpawn(manager, MINION_TYPE_SHOOTER, 0.5f, 2, (sfVector2f) { 2020, 150 }, 1.0f);
        addWaitEvent(manager, 10.0f);

        // Second wave: tanks from both sides
        addMinionSpawn(manager, MINION_TYPE_TANK, 0.0f, 2, (sfVector2f) { -100, 300 }, 1.5f);
        addMinionSpawn(manager, MINION_TYPE_TANK, 0.5f, 2, (sfVector2f) { 2020, 300 }, 1.5f);
        addWaitEvent(manager, 10.0f);

        addBossSpawn(manager, BOSS_TYPE_FLANDRE, 2.0f, (sfVector2f) { 960, -200 });
        addCompleteEvent(manager);
        break;
    }

    case 4:  // Final stage chaos → Yukari
    {
        // Wave 1: Basic enemies
        addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 3, (sfVector2f) { 960, -100 }, 1.0f);
        addWaitEvent(manager, 6.0f);

        // Wave 2: Fast enemies
        addMinionSpawn(manager, MINION_TYPE_FAST, 0.0f, 4, (sfVector2f) { -100, 200 }, 0.7f);
        addWaitEvent(manager, 7.0f);

        // Wave 3: Shooters
        addMinionSpawn(manager, MINION_TYPE_SHOOTER, 0.0f, 4, (sfVector2f) { 2020, 200 }, 0.8f);
        addWaitEvent(manager, 7.0f);

        // Wave 4: Everything at once!
        addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 2, (sfVector2f) { 300, -100 }, 1.0f);
        addMinionSpawn(manager, MINION_TYPE_FAST, 0.5f, 2, (sfVector2f) { 960, -100 }, 0.8f);
        addMinionSpawn(manager, MINION_TYPE_TANK, 1.0f, 2, (sfVector2f) { 1620, -100 }, 1.0f);
        addWaitEvent(manager, 12.0f);

        // Final boss warning
        addWaitEvent(manager, 3.0f);

        addBossSpawn(manager, BOSS_TYPE_YUKARI, 0.0f, (sfVector2f) { 960, -200 });
        addCompleteEvent(manager);
        break;
    }
    }

    manager->currentEvent = manager->eventList;
}

void clearWaveManager(WaveManager* manager)
{
    SpawnEvent* current = manager->eventList;
    while (current != NULL)
    {
        SpawnEvent* next = current->pNext;
        free(current);
        current = next;
    }

    manager->eventList = NULL;
    manager->currentEvent = NULL;
    manager->timer = 0.0f;
    manager->spawnedCount = 0;
}

// Alternative: Complete Touhou-style stage progression
void startTouhouStage(WaveManager* manager)
{
    clearWaveManager(manager);
    manager->waveActive = sfTrue;
    manager->timer = 0.0f;
    manager->spawnedCount = 0;

    // === PHASE 1: Opening Wave ===
    addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 3, (sfVector2f) { 960, -100 }, 1.5f);
    addWaitEvent(manager, 10.0f);

    // === MID-BOSS: JUNKO ===
    addBossSpawn(manager, BOSS_TYPE_JUNKO, 2.0f, (sfVector2f) { 960, -200 });
    addWaitEvent(manager, 45.0f);

    // === PHASE 2: Intermission ===
    addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 3, (sfVector2f) { -100, 200 }, 1.0f);
    addMinionSpawn(manager, MINION_TYPE_BASIC, 2.0f, 2, (sfVector2f) { 2020, 200 }, 1.0f);
    addWaitEvent(manager, 12.0f);

    // === FINAL BOSS: REMILIA ===
    addBossSpawn(manager, BOSS_TYPE_REMILIA, 3.0f, (sfVector2f) { 960, -200 });

    // Stage complete
    addCompleteEvent(manager);

    manager->currentEvent = manager->eventList;
}

// Helper: Create a standard "waves then boss" pattern
void createWavesThenBoss(WaveManager* manager, int numWaves, BossType boss)
{
    clearWaveManager(manager);
    manager->waveActive = sfTrue;

    for (int wave = 0; wave < numWaves; wave++)
    {
        // Vary spawn positions each wave
        sfVector2f spawnPos;
        MinionType minionType;

        if (wave % 3 == 0)
        {
            spawnPos = (sfVector2f){ 960, -100 };    // Center
            minionType = MINION_TYPE_BASIC;
        }
        else if (wave % 3 == 1)
        {
            spawnPos = (sfVector2f){ -100, 200 };    // Left
            minionType = MINION_TYPE_FAST;
        }
        else
        {
            spawnPos = (sfVector2f){ 2020, 200 };    // Right
            minionType = MINION_TYPE_SHOOTER;
        }

        // Spawn 3 minions
        addMinionSpawn(manager, minionType, 0.0f, 3, spawnPos, 1.0f);

        // Wait between waves
        addWaitEvent(manager, 7.0f);
    }

    // Boss after all waves
    addBossSpawn(manager, boss, 2.0f, (sfVector2f) { 960, -200 });
    addCompleteEvent(manager);

    manager->currentEvent = manager->eventList;
}