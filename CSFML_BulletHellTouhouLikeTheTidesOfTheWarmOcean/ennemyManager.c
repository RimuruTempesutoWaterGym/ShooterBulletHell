#include "ennemyManager.h"
#include "textureManager.h"
#include "particleManager.h"
#include "shotManager.h"
#include "playerManager.h"
#include "tools.h"

// ============================================================================
// GLOBALS
// ============================================================================

int ennemyNumber = 0;
ennemy* dlEnnemies = NULL;

// ============================================================================
// CONSTANTS
// ============================================================================

#define PI 3.14159265358979323846f

// ============================================================================
// INITIALIZATION
// ============================================================================

void initEnnemy()
{
    dlEnnemies = NULL;
    ennemyNumber = 0;
}

// ============================================================================
// LIST MANAGEMENT
// ============================================================================

void ajoutEnnemy(ennemy* _ennemy)
{
    _ennemy->pNext = dlEnnemies;
    dlEnnemies = _ennemy;
}

ennemy* retireEnnemy(ennemy* _ennemy)
{
    if (_ennemy == dlEnnemies)
    {
        ennemy* tempEnnemy = _ennemy->pNext;
        dlEnnemies = tempEnnemy;

        // Free boss data if it exists
        if (_ennemy->bossData != NULL)
        {
            free(_ennemy->bossData);
        }

        free(_ennemy);
        ennemyNumber--;
        return tempEnnemy;
    }
    else
    {
        ennemy* tempEnnemy = dlEnnemies;
        while (tempEnnemy->pNext != _ennemy)
        {
            tempEnnemy = tempEnnemy->pNext;
        }
        tempEnnemy->pNext = _ennemy->pNext;

        if (_ennemy->bossData != NULL)
        {
            free(_ennemy->bossData);
        }

        free(_ennemy);
        ennemyNumber--;
        return tempEnnemy->pNext;
    }
}

ennemy* GetEnnemyList()
{
    return dlEnnemies;
}

int getEnnemyNumber()
{
    return ennemyNumber;
}

int isActualEnnemyAlive()
{
    return (dlEnnemies != NULL) ? 1 : 0;
}

// ============================================================================
// MINION SPAWNING
// ============================================================================

void prepareEnnemy(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    // Basic stats
    tempEnnemy->life = 30;
    tempEnnemy->maxLife = 3;
    tempEnnemy->scale = 3.f;
    tempEnnemy->pointValue = 3000;
    tempEnnemy->cooldown = 0;
    tempEnnemy->bossData = NULL;
    tempEnnemy->MinionType = MINION_TYPE_BASIC;
    // Movement state
    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ -100, 100 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 300 };
    tempEnnemy->pos = tempEnnemy->startPos;

    // Timing
    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 2.0f;

    // Pattern behavior
    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 150.0f;
    tempEnnemy->waveFrequency = 1.5f;

    ajoutEnnemy(tempEnnemy);
}

void prepareFastEnnemy(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));
    tempEnnemy->maxLife = 15;
    tempEnnemy->life = tempEnnemy->maxLife;
    tempEnnemy->scale = 2.4f;
    tempEnnemy->cooldown = 0;
    tempEnnemy->pointValue = 2000;
    tempEnnemy->bossData = NULL;
    tempEnnemy->MinionType = MINION_TYPE_FAST;

    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ -100, 150 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 250 };
    tempEnnemy->pos = tempEnnemy->startPos;

    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 1.0f;

    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 200.0f;
    tempEnnemy->waveFrequency = 2.0f;

    ajoutEnnemy(tempEnnemy);
}

void prepareTankEnnemy(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    tempEnnemy->maxLife = 40;
    tempEnnemy->life = tempEnnemy->maxLife;
    tempEnnemy->scale = 4.5f;
    tempEnnemy->cooldown = 0;
    tempEnnemy->pointValue = 3000;
    tempEnnemy->bossData = NULL;
    tempEnnemy->MinionType = MINION_TYPE_TANK;

    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ 960, -100 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 300 };
    tempEnnemy->pos = tempEnnemy->startPos;

    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 3.0f;

    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 50.0f;
    tempEnnemy->waveFrequency = 0.3f;

    ajoutEnnemy(tempEnnemy);
}

void prepareShooterEnnemy(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    tempEnnemy->maxLife = 25;
    tempEnnemy->life = tempEnnemy->maxLife;
    tempEnnemy->scale = 3.f;
    tempEnnemy->cooldown = 0;
    tempEnnemy->pointValue = 1500;
    tempEnnemy->bossData = NULL;
    tempEnnemy->MinionType = MINION_TYPE_SHOOTER;

    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ 2020, 200 };
    tempEnnemy->targetPos = (sfVector2f){ 1600, 250 };
    tempEnnemy->pos = tempEnnemy->startPos;

    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 2.0f;

    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 100.0f;
    tempEnnemy->waveFrequency = 1.0f;

    ajoutEnnemy(tempEnnemy);
}

// ============================================================================
// BOSS SPAWNING
// ============================================================================

void prepareJunkoBoss(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    // Junko's stats
    tempEnnemy->maxLife = 1500;
    tempEnnemy->life = tempEnnemy->maxLife;
    tempEnnemy->scale = 2.5f;
    tempEnnemy->cooldown = 0;
    tempEnnemy->pointValue = 1500000;

    // Movement
    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ 960, -200 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 250 };
    tempEnnemy->pos = tempEnnemy->startPos;

    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 3.0f;

    // Gentle floating movement
    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 100.0f;
    tempEnnemy->waveFrequency = 0.3f;

    // Allocate boss-specific data
    BossData* bossData = (BossData*)calloc(1, sizeof(BossData));
    bossData->phaseTimer = 0.0f;
    bossData->currentPhase = 0;
    bossData->attack1Timer = 0.0f;
    bossData->laneTimer = 0.0f;
    bossData->entityBoss = BOSS_TYPE_REMILIA;

    tempEnnemy->bossData = bossData;

    ajoutEnnemy(tempEnnemy);
}

void prepareRemiliaBoss(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    // Remilia's stats
    tempEnnemy->maxLife = 2500;
    tempEnnemy->life = tempEnnemy->maxLife;
    tempEnnemy->scale = 2.5f;
    tempEnnemy->cooldown = 0;
    tempEnnemy->pointValue =3050000;

    // Movement
    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ 960, -200 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 200 };
    tempEnnemy->pos = tempEnnemy->startPos;

    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 2.0f;

    // Faster movement than Junko
    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 200.0f;
    tempEnnemy->waveFrequency = 0.7f;

    // Allocate boss-specific data
    BossData* bossData = (BossData*)calloc(1, sizeof(BossData));
    bossData->phaseTimer = 0.0f;
    bossData->currentPhase = 1; // Different phase ID for Remilia
    bossData->attack1Timer = 0.0f;
    bossData->laneTimer = 0.0f;
    bossData->entityBoss = BOSS_TYPE_REMILIA;

    tempEnnemy->bossData = bossData;

    ajoutEnnemy(tempEnnemy);
}

void prepareFlandreBoss(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    tempEnnemy->maxLife = 2000;
    tempEnnemy->life = tempEnnemy->maxLife;
    tempEnnemy->scale = 2.3f;
    tempEnnemy->cooldown = 0;
    tempEnnemy->pointValue = 2300000;

    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ 960, -200 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 220 };
    tempEnnemy->pos = tempEnnemy->startPos;

    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 2.5f;

    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 150.0f;
    tempEnnemy->waveFrequency = 0.5f;

    BossData* bossData = (BossData*)calloc(1, sizeof(BossData));
    bossData->phaseTimer = 0.0f;
    bossData->currentPhase = 2;
    bossData->attack1Timer = 0.0f;
    bossData->laneTimer = 0.0f;
    bossData->entityBoss = BOSS_TYPE_REMILIAV2;

    tempEnnemy->bossData = bossData;

    ajoutEnnemy(tempEnnemy);
}

void prepareYukariBoss(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    tempEnnemy->maxLife = 6666;
    tempEnnemy->life = tempEnnemy->maxLife;
    tempEnnemy->scale = 2.7f;
    tempEnnemy->cooldown = 0;
    tempEnnemy->pointValue = 5600000;

    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ 960, -200 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 230 };
    tempEnnemy->pos = tempEnnemy->startPos;

    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 3.5f;

    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 120.0f;
    tempEnnemy->waveFrequency = 0.4f;

    BossData* bossData = (BossData*)calloc(1, sizeof(BossData));
    bossData->phaseTimer = 0.0f;
    bossData->currentPhase = 3;
    bossData->attack1Timer = 0.0f;
    bossData->laneTimer = 0.0f;
    bossData->entityBoss = BOSS_TYPE_JUNKOV2;

    tempEnnemy->bossData = bossData;

    ajoutEnnemy(tempEnnemy);
}

// ============================================================================
// BOSS ATTACK PATTERNS
// ============================================================================


void bossAttackEiki(sfRenderWindow* _window, sfVector2f* _pos, BossData* bossData, float _life, float _maxHealth)
{
    bossData->phaseTimer += getDeltaTime();
    bossData->attack1Timer += getDeltaTime();

    static int waveDirection = 1;
    sfVector2f center = { _pos->x + 47, _pos->y + 42 };

    // Phase 1: Rotating dense rings (HP > 50%)
    if (_life > _maxHealth / 2)
    {
        if (bossData->attack1Timer >= 0.3f)
        {
            waveDirection = -waveDirection;
            float angleOffset = (waveDirection == -1) ? 3.5f : 0.0f;

            createBulletRing(center, 40, angleOffset, 200.0f, 15, 1);

            bossData->attack1Timer = 0.0f;
        }
    }
    // Phase 2: Faster random rings (50% > HP > 25%)
    else if (_life > _maxHealth / 4)
    {
        if (bossData->attack1Timer >= 0.8f)
        {
            float randomOffset = (rand() % 628) / 100.0f;
            createBulletRing(center, 40, randomOffset, 300.0f, 10, 1);

            bossData->attack1Timer = 0.0f;
        }
    }
    // Phase 3: CHAOS - Dense rings + lanes (HP < 25%)
    else
    {
        bossData->laneTimer += getDeltaTime();

        // Super dense rings
        if (bossData->attack1Timer >= 0.5f)
        {
            float randomOffset = (rand() % 628) / 100.0f;
            float randomSpeed = 150.0f + (rand() % 60);
            createBulletRing(center, 48, randomOffset, randomSpeed, 8, 1);

            bossData->attack1Timer = 0.0f;
        }

        // Confining lanes
        if (bossData->laneTimer >= 2.0f)
        {
            createRadialLanes(center, 24, 8, 30.0f, 100.0f, 12, 1);
            bossData->laneTimer = 0.0f;
        }
    }
}



void bossAttackUtsuho(sfRenderWindow* _window, sfVector2f* _pos, BossData* bossData, float _life, float _maxHealth)
{
    bossData->phaseTimer += getDeltaTime();
    bossData->attack1Timer += getDeltaTime();

    sfVector2f center = { _pos->x + 47, _pos->y + 42 };

    // Get player position
    player* tempPlayer = GetPlayerList();
    sfVector2f playerPos = { 960, 800 };

    if (tempPlayer != NULL)
    {
        playerPos.x = tempPlayer->pos.x + 25;
        playerPos.y = tempPlayer->pos.y + 25;
    }

    // Phase 1: Aimed shots + cross pattern (HP > 50%)
    if (_life > _maxHealth / 2)
    {
        // Continuous aimed 3-way spread
        if (bossData->attack1Timer >= 0.3f)
        {
            createAimedSpread(center, playerPos, 3, 0.4f, 180.0f, 8, 1);
            bossData->attack1Timer = 0.0f;
        }

        // Red magic cross bursts every 2 seconds
        bossData->laneTimer += getDeltaTime();
        if (bossData->laneTimer >= 2.0f)
        {
            createCrossPattern(center, 8, 15.0f, 140.0f, 10, 1);
            bossData->laneTimer = 0.0f;
        }
    }
    // Phase 2: Dense aimed + expanding rings (HP < 50%)
    else
    {
        // Faster aimed shots - 5-way spread
        if (bossData->attack1Timer >= 0.2f)
        {
            createAimedSpread(center, playerPos, 5, 0.6f, 220.0f, 7, 1);
            bossData->attack1Timer = 0.0f;
        }

        // Dense expanding rings every 1.5 seconds
        bossData->laneTimer += getDeltaTime();
        if (bossData->laneTimer >= 1.5f)
        {
            createBulletRing(center, 36, 0.0f, 150.0f, 9, 1);
            bossData->laneTimer = 0.0f;
        }
    }
}

void bossAttackEikiV2(sfRenderWindow* _window, sfVector2f* _pos, BossData* bossData, float _life, float _maxHealth)
{
    bossData->phaseTimer += getDeltaTime();
    bossData->attack1Timer += getDeltaTime();

    sfVector2f center = { _pos->x + 47, _pos->y + 42 };

    // Get player position
    player* tempPlayer = GetPlayerList();
    sfVector2f playerPos = { 960, 800 };

    if (tempPlayer != NULL)
    {
        playerPos.x = tempPlayer->pos.x + 25;
        playerPos.y = tempPlayer->pos.y + 25;
    }

    // Phase 1: Time Knives + Deflation World (HP > 50%)
    if (_life > _maxHealth / 2)
    {
        // Time knives every 2 seconds
        if (bossData->attack1Timer >= 2.0f)
        {
            createTimeKnives(center, 3);
            bossData->attack1Timer = 0.0f;
        }

        // Deflation World every 4 seconds
        bossData->laneTimer += getDeltaTime();
        if (bossData->laneTimer >= 4.0f)
        {
            createDeflationWorld(center, 4);
            bossData->laneTimer = 0.0f;
        }
    }
    // Phase 2: Perfect Square + Eternal Meek (HP < 50%)
    else
    {
        // Perfect Square every 3 seconds
        if (bossData->attack1Timer >= 3.0f)
        {
            createPerfectSquare(center);
            bossData->attack1Timer = 0.0f;
        }

        // Eternal Meek aimed knives every 1.5 seconds
        bossData->laneTimer += getDeltaTime();
        if (bossData->laneTimer >= 1.5f)
        {
            createEternalMeek(center, playerPos);
            bossData->laneTimer = 0.0f;
        }
    }
}
void bossAttackUtsuhoV2(sfRenderWindow* _window, sfVector2f* _pos, BossData* bossData, float _life, float _maxHealth)
{
    bossData->phaseTimer += getDeltaTime();
    bossData->attack1Timer += getDeltaTime();

    sfVector2f center = { _pos->x + 47, _pos->y + 42 };

    // Get player position
    player* tempPlayer = GetPlayerList();
    sfVector2f playerPos = { 960, 800 };

    if (tempPlayer != NULL)
    {
        playerPos.x = tempPlayer->pos.x + 25;
        playerPos.y = tempPlayer->pos.y + 25;
    }

    // Phase 1: Backdoor Spiral + Summer Backdoor (HP > 66%)
    if (_life > _maxHealth * 0.66f)
    {
        // Backdoor spiral every 3 seconds
        if (bossData->attack1Timer >= 3.0f)
        {
            createBackdoorSpiral(center);
            bossData->attack1Timer = 0.0f;
        }
    }
    // Phase 2: Crystallized Release + Butterfly Dream (66% > HP > 33%)
    else if (_life > _maxHealth * 0.33f)
    {
        // Crystallized release every 4 seconds
        if (bossData->attack1Timer >= 4.0f)
        {
            createCrystallizedRelease(center);
            bossData->attack1Timer = 0.0f;
        }

        // Butterfly dream every 2.5 seconds
        bossData->laneTimer += getDeltaTime();
        if (bossData->laneTimer >= 2.5f)
        {
            createButterflyDream(center, playerPos);
            bossData->laneTimer = 0.0f;
        }
    }
    // Phase 3: Seven Stars + Summer Backdoor (HP < 33%)
    else
    {
        // Seven stars every 3.5 seconds
        if (bossData->attack1Timer >= 3.5f)
        {
            createSevenStars(center);
            bossData->attack1Timer = 0.0f;
        }

        // Summer backdoor every 2 seconds
        bossData->laneTimer += getDeltaTime();
        if (bossData->laneTimer >= 2.0f)
        {
            createSummerBackdoor(center);
            bossData->laneTimer = 0.0f;
        }
    }
}

// ============================================================================
// UPDATE & MOVEMENT
// ============================================================================

void updateEnnemy(sfRenderWindow* _window)
{
    ennemy* tempEnnemy = dlEnnemies;

    while (tempEnnemy != NULL)
    {
        tempEnnemy->moveTimer += getDeltaTime();

        switch (tempEnnemy->state)
        {
        case STATE_ENTER:
        {
            float t = tempEnnemy->moveTimer / tempEnnemy->moveDuration;
            if (t >= 1.0f)
            {
                t = 1.0f;
                tempEnnemy->state = STATE_PATTERN;
                tempEnnemy->moveTimer = 0.0f;
                tempEnnemy->patternTimer = 0.0f;
            }

            t = easeInOutQuad(t);
            tempEnnemy->pos = vec2f_lerp(tempEnnemy->startPos, tempEnnemy->targetPos, t);
            break;
        }

        case STATE_PATTERN:
        {
            tempEnnemy->patternTimer += getDeltaTime();

            // Wave movement
            float wave = sin(tempEnnemy->patternTimer * tempEnnemy->waveFrequency) * tempEnnemy->waveAmplitude;
            tempEnnemy->pos.x = tempEnnemy->targetPos.x + wave;

            // Boss attacks
            if (tempEnnemy->bossData != NULL)
            {
                BossData* bossData = (BossData*)tempEnnemy->bossData;

                // Determine which boss based on currentPhase
                if (bossData->currentPhase == 1) // Eiki
                {
                    bossAttackEiki(_window, &tempEnnemy->pos, bossData, tempEnnemy->life, tempEnnemy->maxLife);
                }
                else if (bossData->currentPhase == 1) // Utsuho
                {
                    bossAttackUtsuho(_window, &tempEnnemy->pos, bossData, tempEnnemy->life, tempEnnemy->maxLife);
                }
                else if (bossData->currentPhase == 0) // EikiV2
                {
                    bossAttackEikiV2(_window, &tempEnnemy->pos, bossData, tempEnnemy->life, tempEnnemy->maxLife);
                }
                else if (bossData->currentPhase == 3) // UtsuhoV2
                {
                    bossAttackUtsuhoV2(_window, &tempEnnemy->pos, bossData, tempEnnemy->life, tempEnnemy->maxLife);
                }

            }
            else
            {
                // Regular enemy attacks
                tempEnnemy->cooldown += getDeltaTime();

                if (tempEnnemy->cooldown >= 1.0f)
                {
                    prepareEnnemyShot(_window, &tempEnnemy->pos, 12, 8, (sfVector2f) { 47, 42 });
                    tempEnnemy->cooldown = 0.0f;
                }
            }

            break;
        }

        case STATE_EXIT:
        {
            // Move off screen
            float t = tempEnnemy->moveTimer / tempEnnemy->moveDuration;
            if (t >= 1.0f)
            {
                tempEnnemy = retireEnnemy(tempEnnemy);
                continue;
            }

            tempEnnemy->pos = vec2f_lerp(tempEnnemy->startPos, tempEnnemy->targetPos, t);
            break;
        }
        }



      

        tempEnnemy = tempEnnemy->pNext;
    }
}
void DisplayEnnemy(sfRenderWindow* _window, sfSprite* _sprite)
{
    ennemy* tempEnnemy = dlEnnemies;

    while (tempEnnemy != NULL)
    {
        // Draw
        sfSprite_setPosition(_sprite, tempEnnemy->pos);
        if (tempEnnemy->bossData != NULL)
        {
 
            switch (tempEnnemy->bossData->entityBoss)
            {
            case 1:
                sfSprite_setTexture(_sprite, GetTexture("utsuho"), sfTrue);
                break;
            case 2:
                sfSprite_setTexture(_sprite, GetTexture("eiki"), sfTrue);
                break;
            case 3:
                sfSprite_setTexture(_sprite, GetTexture("utsuho"), sfTrue);
                break;
            case 4:
                sfSprite_setTexture(_sprite, GetTexture("eiki"), sfTrue);
                break;
            }
        }
        else if (tempEnnemy->MinionType != NULL)
        {
     
            switch (tempEnnemy->MinionType)
            {
            case 1:
                sfSprite_setTexture(_sprite, GetTexture("clownpiece"), sfTrue);
                break;
            case 2:
                sfSprite_setTexture(_sprite, GetTexture("cirno"), sfTrue);
                break;
            case 3:
                sfSprite_setTexture(_sprite, GetTexture("suikalbuki"), sfTrue);
                break;
            case 4:
                sfSprite_setTexture(_sprite, GetTexture("flandreScarlet"), sfTrue);
                break;
            }
        }
        else
        {
            sfSprite_setTexture(_sprite, GetTexture("enemy"), sfTrue);
        }
        sfSprite_setScale(_sprite, (sfVector2f) { tempEnnemy->scale, tempEnnemy->scale });
        sfRenderWindow_drawSprite(_window, _sprite, NULL);
        tempEnnemy = tempEnnemy->pNext;
    }
}
void RemoveAllEnnemies()
{
    ennemy* tempEnnemy = dlEnnemies;
    while (tempEnnemy != NULL)
    {
        ennemy* toDelete = tempEnnemy;
        tempEnnemy = tempEnnemy->pNext;
        free(toDelete);
    }
    dlEnnemies = 0;
}
void drawBossHealthArc(sfRenderWindow* window)
{
    ennemy* tempEnnemy = dlEnnemies;
    while (tempEnnemy != NULL)
    {
        if (tempEnnemy->bossData != NULL)
        {
            sfVector2f center = { tempEnnemy->pos.x + 25 * tempEnnemy->scale, tempEnnemy->pos.y + 50 * tempEnnemy->scale };
            float healthPercent = tempEnnemy->life / tempEnnemy->maxLife;

            float radius = 120.0f;
            int segments = 300;
            for (int thickness = 0; thickness < 6; thickness++)  
            {
                sfVertexArray* arc = sfVertexArray_create();
                sfVertexArray_setPrimitiveType(arc, sfLinesStrip);

                float currentRadius = radius + thickness;
                int segmentsToDraw = (int)(segments * healthPercent);

                sfColor color;
           
             
                    if (healthPercent > 0.5f)
                        color = (sfColor){ 0, 255, 0, 255 };
                    else if (healthPercent > 0.25f)
                        color = (sfColor){ 255, 255, 0, 255 };
                    else
                        color = (sfColor){ 255, 0, 0, 255 };
                
                for (int i = 0; i <= segmentsToDraw; i++)
                {
                    float angle = (i / (float)segments) * 2.0f * 3.14159265359f - 1.5708f; 
                    float x = center.x + cos(angle) * currentRadius;
                    float y = center.y + sin(angle) * currentRadius;

                    sfVertex vertex = { {x, y}, color, {0, 0} };
                    sfVertexArray_append(arc, vertex);
                }

                sfRenderWindow_drawVertexArray(window, arc, NULL);
                sfVertexArray_destroy(arc);
            }
        }
        tempEnnemy = tempEnnemy->pNext;
    }
}