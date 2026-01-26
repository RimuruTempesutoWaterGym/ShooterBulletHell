#pragma once
#include "tools.h"
// liste d'ennemies
typedef enum {
	STATE_ENTER,      // Lerp to position
	STATE_PATTERN,    // Execute attack pattern
	STATE_EXIT        // Lerp off screen
} EnnemyState;

typedef enum {
    BOSS_PATTERN_SPIRAL,
    BOSS_PATTERN_BURST,
    BOSS_PATTERN_AIMED_RING,
    BOSS_PATTERN_DENSE_CIRCLE
} BossPattern;

typedef struct {
    float phaseTimer;
    int currentPhase;
    float ringTimer;
    float laneTimer;

} BossData;
typedef struct ennemy ennemy;
struct ennemy {
    sfVector2f pos;
    sfVector2f velocity;
    float life;
    float maxLife;
    float scale;
    float cooldown;

    // Lerp movement
    EnnemyState state;
    sfVector2f startPos;
    sfVector2f targetPos;
    float moveTimer;
    float moveDuration;

    // Pattern behavior
    float patternTimer;
    float waveAmplitude;
    float waveFrequency;

    // Boss-specific data
    void* bossData;  // Points to JunkoBossData for Junko

    struct ennemy* pNext;
} ;

void prepareEnnemy(sfRenderWindow* _window);
void ajoutEnnemy(ennemy* _ennemy);
void updateEnnemy(sfRenderWindow* _window, sfSprite* _sprite);
ennemy* retireEnnemy(ennemy* _ennemy);
void initEnnemy();
ennemy* GetEnnemyList();
void prepareBossEnnemy(sfRenderWindow* _window);
void prepareBoss1(sfRenderWindow* _window);
void bossAttackPurelyBulletHell(sfRenderWindow* _window, sfVector2f* _pos, BossData* bossData, float _life);