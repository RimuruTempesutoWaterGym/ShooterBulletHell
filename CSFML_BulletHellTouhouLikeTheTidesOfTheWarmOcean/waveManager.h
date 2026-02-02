
#pragma once
#include "tools.h"
typedef enum {
    SPAWN_MINION,
    SPAWN_BOSS,
    SPAWN_WAIT,
    SPAWN_COMPLETE
} SpawnType;
typedef enum {

    BOSS_TYPE_EIKI = 1,
    BOSS_TYPE_UTSUHO,
    BOSS_TYPE_EIKIV2,
    BOSS_TYPE_UTSUHOV2,
} BossType;
typedef enum {
    MINION_TYPE_BASIC = 1,
    MINION_TYPE_FAST,
    MINION_TYPE_TANK,
    MINION_TYPE_SHOOTER
} MinionType;

typedef struct SpawnEvent SpawnEvent;
struct SpawnEvent {
    SpawnType type;
    union {
        MinionType minionType;
        BossType bossType;
    };
    float delay;
    int count;
    sfVector2f position;
    float interval;
    struct SpawnEvent* pNext;
} ;

typedef struct {
    SpawnEvent* currentEvent;
    SpawnEvent* eventList;
    float timer;
    int spawnedCount;
    int currentWave;
    sfBool waveActive;
} WaveManager;


void initWaveManager(WaveManager* manager);
void addSpawnEvent(WaveManager* manager, SpawnType type, float delay, int count, sfVector2f pos, float interval);
void updateWaveManager(WaveManager* manager, sfRenderWindow* window);
void startWave(WaveManager* manager, int waveNumber);
void clearWaveManager(WaveManager* manager);
void addSpawnEvent(WaveManager* manager, SpawnType type, BossType bossType, float delay, int count, sfVector2f pos, float interval);
