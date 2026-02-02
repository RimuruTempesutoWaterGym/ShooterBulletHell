
#include "waveManager.h"
#include "ennemyManager.h"
#include "tools.h"
#include "stateManager.h"
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
                printf("%d\n", event->bossType);
                switch (event->bossType)
                {
                case BOSS_TYPE_EIKI:
                    prepareEikiBoss(window);
                    break;
                case BOSS_TYPE_UTSUHO:
                    prepareUtsuhoBoss(window);
                    break;
                case BOSS_TYPE_EIKIV2:
                    prepareEikiv2Boss(window);
                    break;
                case BOSS_TYPE_UTSUHOV2:
                    prepareUtsuhov2Boss(window);
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
    case 1: 
    {
        Play_Music("stage1");
        addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 3, (sfVector2f) { 960, -100 }, 1.5f);
        addWaitEvent(manager, 4.0f);

        addMinionSpawn(manager, MINION_TYPE_FAST, 0.0f, 3, (sfVector2f) { -100, 200 }, 1.0f);
        addWaitEvent(manager, 4.0f);

        addBossSpawn(manager, BOSS_TYPE_EIKI, 2.0f, (sfVector2f) { 960, -200 });
        addCompleteEvent(manager);
     
        break;
    }

    case 2:  
    
    {
        Play_Music("stage2");
        addMinionSpawn(manager, MINION_TYPE_SHOOTER, 0.0f, 3, (sfVector2f) { 400, -100 }, 1.5f);
        addWaitEvent(manager, 3.0f);

        addMinionSpawn(manager, MINION_TYPE_TANK, 0.0f, 4, (sfVector2f) { 1520, -100 }, 1.2f);
        addWaitEvent(manager, 5.0f);

        addMinionSpawn(manager, MINION_TYPE_FAST, 0.0f, 3, (sfVector2f) { 960, -100 }, 0.8f);
        addWaitEvent(manager, 4.0f);

        addBossSpawn(manager, BOSS_TYPE_UTSUHO, 3.0f, (sfVector2f) { 960, -200 });
        addCompleteEvent(manager);
        break;
    }

    case 3:  
    {
        Play_Music("stage3");
        addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 3, (sfVector2f) { -100, 150 }, 1.0f);
      
        addMinionSpawn(manager, MINION_TYPE_SHOOTER, 0.5f, 2, (sfVector2f) { 2020, 150 }, 1.0f);
        addWaitEvent(manager, 5.0f);


        addMinionSpawn(manager, MINION_TYPE_TANK, 0.0f, 2, (sfVector2f) { -100, 300 }, 1.5f);
        addMinionSpawn(manager, MINION_TYPE_TANK, 0.5f, 2, (sfVector2f) { 2020, 300 }, 1.5f);
        addWaitEvent(manager, 5.0f);

        addBossSpawn(manager, BOSS_TYPE_EIKIV2, 2.0f, (sfVector2f) { 960, -200 });
        addCompleteEvent(manager);
        break;
    }

    case 4: 
    {
        Play_Music("stage4");
        addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 3, (sfVector2f) { 960, -100 }, 1.0f);
        addWaitEvent(manager, 3.0f);


        addMinionSpawn(manager, MINION_TYPE_FAST, 0.0f, 4, (sfVector2f) { -100, 200 }, 0.7f);
        addWaitEvent(manager, 4.0f);


        addMinionSpawn(manager, MINION_TYPE_SHOOTER, 0.0f, 4, (sfVector2f) { 2020, 200 }, 0.8f);
        addWaitEvent(manager, 4.0f);

  
        addMinionSpawn(manager, MINION_TYPE_BASIC, 0.0f, 2, (sfVector2f) { 300, -100 }, 1.0f);
        addMinionSpawn(manager, MINION_TYPE_FAST, 0.5f, 2, (sfVector2f) { 960, -100 }, 0.8f);
        addMinionSpawn(manager, MINION_TYPE_TANK, 1.0f, 2, (sfVector2f) { 1620, -100 }, 1.0f);
        addWaitEvent(manager, 6.0f);


        addBossSpawn(manager, BOSS_TYPE_UTSUHOV2, 0.0f, (sfVector2f) { 960, -200 });
   
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




