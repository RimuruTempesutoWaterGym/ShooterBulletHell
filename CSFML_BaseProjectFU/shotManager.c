Here's a cleaned up and reorganized shotManager.c:
c

// shotManager.c
#include "playerManager.h"
#include "shotManager.h"
#include "textureManager.h"
#include "ennemyManager.h"
#include "particleManager.h"
#include "tools.h"

// ============================================================================
// GLOBALS
// ============================================================================

shot * dlShot = NULL;
sfIntRect spellCardRect;

// ============================================================================
// CONSTANTS
// ============================================================================

#define PI 3.14159265358979323846f
#define SHOT_FRICTION 0.05f  // Deceleration factor

// ============================================================================
// INITIALIZATION
// ============================================================================

void initShot()
{
    dlShot = NULL;
    spellCardRect = GetRect("spellcards");
}

// ============================================================================
// LIST MANAGEMENT
// ============================================================================

void ajoutShot(shot* _shot)
{
    _shot->pNext = dlShot;
    dlShot = _shot;
}

shot* retireShot(shot* _shot)
{
    if (_shot == dlShot)
    {
        shot* tempShot = _shot->pNext;
        dlShot = tempShot;
        free(_shot);
        return tempShot;
    }
    else
    {
        shot* tempShot = dlShot;
        while (tempShot->pNext != _shot)
        {
            tempShot = tempShot->pNext;
        }
        tempShot->pNext = _shot->pNext;
        free(_shot);
        return tempShot->pNext;
    }
}

// ============================================================================
// PLAYER SHOTS
// ============================================================================

void preparePlayerShot(sfRenderWindow* _window, sfVector2f* _pos)
{
    for (int i = 0; i < 4; i++)
    {
        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = ally;
        tempShot->typeShot = normalShot;
        tempShot->life = 1;
        tempShot->scale = 5;
        tempShot->pos.x = _pos->x + (i * 10) + 5;
        tempShot->pos.y = _pos->y;
        tempShot->velocity.x = 0;
        tempShot->velocity.y = 300;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        ajoutShot(tempShot);
    }
}

void prepareSpellCardShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale, sfVector2f _centering)
{
    prepareCircleShot(_window, _pos, _nbShot, _scale, 3, _centering, ally, SpellCardShot);
}

// ============================================================================
// ENEMY SHOTS - BASIC PATTERNS
// ============================================================================

void prepareEnnemyShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale, sfVector2f _centering)
{
    prepareCircleShot(_window, _pos, _nbShot, _scale, 1, _centering, opponent, normalShot);
}

void prepareCircleShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale, int _damage, sfVector2f _centering, int _origin, int _typeOfShot)
{
    float angle = -PI;
    float angleStep = (2.0f * PI) / _nbShot;

    for (int i = 0; i <= _nbShot; i++)
    {
        angle += angleStep;

        // Calculate spawn offset from center
        float offsetX = cos(angle) * (80.0f / 1.2f);
        float offsetY = sin(angle) * (40.0f / 1.2f);

        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = _origin;
        tempShot->typeShot = _typeOfShot;
        tempShot->life = 1;
        tempShot->damage = _damage;
        tempShot->scale = _scale;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        // Random color for spell cards
        if (_typeOfShot == SpellCardShot)
        {
            tempShot->color = rand_int(0, 14);
        }

        // Position
        sfVector2f center = { _pos->x + _centering.x, _pos->y + _centering.y };
        tempShot->pos.x = center.x + offsetX;
        tempShot->pos.y = center.y + offsetY;

        // Velocity (outward from center)
        float speed = 10.0f;
        tempShot->velocity.x = -(tempShot->pos.x - center.x) * speed;
        tempShot->velocity.y = -(tempShot->pos.y - center.y) * speed;

        // Only add if on screen
        if (tempShot->pos.x > 0 && tempShot->pos.y > 0 &&
            tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
        {
            ajoutShot(tempShot);
        }
        else
        {
            free(tempShot);
        }
    }
}

// ============================================================================
// ENEMY SHOTS - ADVANCED PATTERNS
// ============================================================================

// Create a single shot with full control
shot* createShot(sfVector2f position, float angle, float speed, int scale, int damage)
{
    shot* tempShot = (shot*)calloc(1, sizeof(shot));
    tempShot->shooter = opponent;
    tempShot->typeShot = normalShot;
    tempShot->life = 1;
    tempShot->damage = damage;
    tempShot->scale = scale;
    tempShot->hasHit = sfFalse;
    tempShot->hitTimer = 0;

    tempShot->pos = position;

    tempShot->velocity.x = -cos(angle) * speed;
    tempShot->velocity.y = -sin(angle) * speed;

    ajoutShot(tempShot);
    return tempShot;
}

// Create a ring of bullets
void createBulletRing(sfVector2f center, int bulletCount, float angleOffset, float speed, int scale, int damage)
{
    for (int i = 0; i < bulletCount; i++)
    {
        float angle = angleOffset + (i / (float)bulletCount) * 2.0f * PI;
        createShot(center, angle, speed, scale, damage);
    }
}

// Create aimed shot toward a target
void createAimedShot(sfVector2f fromPos, sfVector2f toPos, float speed, int scale, int damage)
{
    float dx = toPos.x - fromPos.x;
    float dy = toPos.y - fromPos.y;
    float angle = atan2(dy, dx);

    createShot(fromPos, angle, speed, scale, damage);
}

// Create aimed spread (multiple shots in a cone toward target)
void createAimedSpread(sfVector2f fromPos, sfVector2f toPos, int bulletCount, float spreadAngle, float speed, int scale, int damage)
{
    float dx = toPos.x - fromPos.x;
    float dy = toPos.y - fromPos.y;
    float baseAngle = atan2(dy, dx);

    for (int i = 0; i < bulletCount; i++)
    {
        float offset = ((i / (float)(bulletCount - 1)) - 0.5f) * spreadAngle;
        float angle = baseAngle + offset;

        createShot(fromPos, angle, speed, scale, damage);
    }
}

// Create a lane of bullets (bullets spawned along a line)
void createBulletLane(sfVector2f center, float angle, int depth, float spacing, float speed, int scale, int damage)
{
    for (int i = 0; i < depth; i++)
    {
        float distance = i * spacing;
        sfVector2f spawnPos;
        spawnPos.x = center.x + cos(angle) * distance;
        spawnPos.y = center.y + sin(angle) * distance;

        createShot(spawnPos, angle, speed, scale, damage);
    }
}

// Create multiple lanes in a radial pattern
void createRadialLanes(sfVector2f center, int laneCount, int bulletsPerLane, float spacing, float speed, int scale, int damage)
{
    for (int i = 0; i < laneCount; i++)
    {
        float angle = (i / (float)laneCount) * 2.0f * PI;
        createBulletLane(center, angle, bulletsPerLane, spacing, speed, scale, damage);
    }
}

// Create a spiral of bullets
void createBulletSpiral(sfVector2f center, int streamCount, int bulletsPerStream, float baseAngle, float bulletSpacing, float speedStart, float speedDecay, int scale, int damage)
{
    for (int stream = 0; stream < streamCount; stream++)
    {
        float streamAngle = baseAngle + (stream * (2.0f * PI / streamCount));

        for (int i = 0; i < bulletsPerStream; i++)
        {
            float bulletAngle = streamAngle + (i * bulletSpacing);
            float speed = speedStart - (i * speedDecay);

            createShot(center, bulletAngle, speed, scale, damage);
        }
    }
}

// Create bullets in a cross pattern
void createCrossPattern(sfVector2f center, int bulletsPerArm, float armSpacing, float speed, int scale, int damage)
{
    for (int direction = 0; direction < 4; direction++)
    {
        float baseAngle = direction * (PI / 2.0f); // 90 degrees apart
        createBulletLane(center, baseAngle, bulletsPerArm, armSpacing, speed, scale, damage);
    }
}

// Create random scattered bullets
void createRandomScatter(sfVector2f center, int bulletCount, float minSpeed, float maxSpeed, int scale, int damage)
{
    for (int i = 0; i < bulletCount; i++)
    {
        float angle = ((rand() % 628) / 100.0f);
        float speed = minSpeed + (rand() % (int)(maxSpeed - minSpeed));

        createShot(center, angle, speed, scale, damage);
    }
}

// Create overlapping rings (multiple rings with different speeds)
void createOverlappingRings(sfVector2f center, int ringCount, int bulletsPerRing, float ringOffset, float speedStart, float speedDecay, int scaleStart, int scaleGrowth, int damage)
{
    for (int ring = 0; ring < ringCount; ring++)
    {
        int bulletCount = bulletsPerRing + (ring * 8);
        float angleOffset = ring * ringOffset;
        float speed = speedStart - (ring * speedDecay);
        int scale = scaleStart + (ring * scaleGrowth);

        createBulletRing(center, bulletCount, angleOffset, speed, scale, damage);
    }
}
void prepareCrossShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
    float angle = -PI;
    sfVector2f center = { _pos->x + 42, _pos->y + 42 };

    for (int i = 0; i < _nbShot; i++)
    {
        angle += (2.0f * PI) / _nbShot;

        float offsetX = cos(tan(angle)) * (100.0f / 1.2f);
        float offsetY = sin(angle) * (100.0f / 1.2f);

        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = opponent;
        tempShot->typeShot = normalShot;
        tempShot->life = 1;
        tempShot->damage = 1;
        tempShot->scale = 10;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        tempShot->pos.x = center.x + offsetX;
        tempShot->pos.y = center.y + offsetY;

        tempShot->velocity.x = -(tempShot->pos.x - center.x) * 10.0f;
        tempShot->velocity.y = -(tempShot->pos.y - center.y) * 10.0f;

        if (tempShot->pos.x > 0 && tempShot->pos.y > 0 &&
            tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
        {
            ajoutShot(tempShot);
        }
        else
        {
            free(tempShot);
        }
    }
}

void prepareRandomShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
    float angle = -PI;
    sfVector2f center = { _pos->x + 42, _pos->y + 42 };

    for (int i = 0; i < _nbShot; i++)
    {
        angle += (2.0f * PI) / _nbShot;

        float offsetX = log(angle + PI) * (100.0f / 1.2f);
        float offsetY = sqrt(angle + PI) * (100.0f / 1.2f);

        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = opponent;
        tempShot->typeShot = normalShot;
        tempShot->life = 1;
        tempShot->damage = 1;
        tempShot->scale = 10;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        tempShot->pos.x = center.x + offsetX;
        tempShot->pos.y = center.y + offsetY;

        tempShot->velocity.x = -(tempShot->pos.x - center.x) * 10.0f;
        tempShot->velocity.y = -(tempShot->pos.y - center.y) * 10.0f;

        if (tempShot->pos.x > 0 && tempShot->pos.y > 0 &&
            tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
        {
            ajoutShot(tempShot);
        }
        else
        {
            free(tempShot);
        }
    }
}

void prepareLosangeShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
    float angle = -PI;
    sfVector2f center = { _pos->x + 42, _pos->y + 42 };

    for (int i = 0; i < _nbShot; i++)
    {
        angle += (2.0f * PI) / _nbShot;

        float offsetX = tan(cos(angle)) * (100.0f / 1.2f);
        float offsetY = tan(sin(angle)) * (100.0f / 1.2f);

        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = opponent;
        tempShot->typeShot = normalShot;
        tempShot->life = 1;
        tempShot->damage = 1;
        tempShot->scale = 10;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        tempShot->pos.x = center.x + offsetX;
        tempShot->pos.y = center.y + offsetY;

        tempShot->velocity.x = -(tempShot->pos.x - center.x) * 10.0f;
        tempShot->velocity.y = -(tempShot->pos.y - center.y) * 10.0f;

        if (tempShot->pos.x > 0 && tempShot->pos.y > 0 &&
            tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
        {
            ajoutShot(tempShot);
        }
        else
        {
            free(tempShot);
        }
    }
}

void preparePrismShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
    float angle = -PI;
    sfVector2f center = { _pos->x + 42, _pos->y + 42 };

    for (int i = 0; i < _nbShot; i++)
    {
        angle += (2.0f * PI) / _nbShot;

        float offsetX = acos(cos(angle)) * (100.0f / 1.2f);
        float offsetY = acos(sin(angle)) * (100.0f / 1.2f);

        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = opponent;
        tempShot->typeShot = normalShot;
        tempShot->life = 1;
        tempShot->damage = 1;
        tempShot->scale = 10;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        tempShot->pos.x = center.x + offsetX;
        tempShot->pos.y = center.y + offsetY;

        tempShot->velocity.x = -(tempShot->pos.x - center.x) * 5.0f;
        tempShot->velocity.y = -(tempShot->pos.y - center.y) * 5.0f;

        if (tempShot->pos.x > 0 && tempShot->pos.y > 0 &&
            tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
        {
            ajoutShot(tempShot);
        }
        else
        {
            free(tempShot);
        }
    }
}

void prepareLassoShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
    float angle = -PI;
    sfVector2f center = { _pos->x + 42, _pos->y + 42 };

    for (int i = 0; i < _nbShot; i++)
    {
        angle += (2.0f * PI) / _nbShot;

        float offsetX = atan(sin(angle)) * (100.0f / 1.2f);
        float offsetY = cos(atan(angle)) * (100.0f / 1.8f);

        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = opponent;
        tempShot->typeShot = normalShot;
        tempShot->life = 1;
        tempShot->damage = 1;
        tempShot->scale = 10;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        tempShot->pos.x = center.x + offsetX;
        tempShot->pos.y = center.y + offsetY;

        tempShot->velocity.x = -(tempShot->pos.x - center.x) * 10.0f;
        tempShot->velocity.y = -(tempShot->pos.y - center.y) * 10.0f;

        if (tempShot->pos.x > 0 && tempShot->pos.y > 0 &&
            tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
        {
            ajoutShot(tempShot);
        }
        else
        {
            free(tempShot);
        }
    }
}

// ============================================================================
// BOSS ATTACK PATTERNS
// ============================================================================

void bossAttackSpiral(sfRenderWindow* _window, sfVector2f* _pos, float patternTimer)
{
    static float spiralAngle = 0.0f;
    spiralAngle += getDeltaTime() * 2.0f;

    // Fire 3 streams that rotate (120 degrees apart)
    for (int stream = 0; stream < 3; stream++)
    {
        float streamAngle = spiralAngle + (stream * 2.094f);

        // Each stream has 5 bullets
        for (int i = 0; i < 5; i++)
        {
            float bulletAngle = streamAngle + (i * 0.3f);

            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = normalShot;
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 8;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            tempShot->pos.x = _pos->x + 47;
            tempShot->pos.y = _pos->y + 42;

            // Bullets slow down as they go out
            float speed = 150.0f - (i * 20.0f);
            tempShot->velocity.x = -cos(bulletAngle) * speed;
            tempShot->velocity.y = -sin(bulletAngle) * speed;

            ajoutShot(tempShot);
        }
    }
}

void bossAttackExpandingRing(sfRenderWindow* _window, sfVector2f* _pos)
{
    int bulletCount = 48;

    for (int i = 0; i < bulletCount; i++)
    {
        float angle = (i / (float)bulletCount) * 2.0f * PI;

        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = opponent;
        tempShot->typeShot = normalShot;
        tempShot->life = 1;
        tempShot->damage = 1;
        tempShot->scale = 6;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        tempShot->pos.x = _pos->x + 47;
        tempShot->pos.y = _pos->y + 42;

        float speed = 120.0f;
        tempShot->velocity.x = -cos(angle) * speed;
        tempShot->velocity.y = -sin(angle) * speed;

        ajoutShot(tempShot);
    }
}

void bossAttackAimedBurst(sfRenderWindow* _window, sfVector2f* _bossPos, sfVector2f* _playerPos)
{
    // Calculate angle to player
    float dx = _playerPos->x - (_bossPos->x + 47);
    float dy = _playerPos->y - (_bossPos->y + 42);
    float angleToPlayer = atan2(dy, dx);

    int bulletCount = 7;
    float spreadAngle = 0.5f;

    for (int i = 0; i < bulletCount; i++)
    {
        float offset = ((i / (float)(bulletCount - 1)) - 0.5f) * spreadAngle;
        float angle = angleToPlayer + offset;

        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = opponent;
        tempShot->typeShot = normalShot;
        tempShot->life = 1;
        tempShot->damage = 1;
        tempShot->scale = 10;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        tempShot->pos.x = _bossPos->x + 47;
        tempShot->pos.y = _bossPos->y + 42;

        float speed = 200.0f;
        tempShot->velocity.x = -cos(angle) * speed;
        tempShot->velocity.y = -sin(angle) * speed;

        ajoutShot(tempShot);
    }
}

// ============================================================================
// UPDATE & COLLISION DETECTION
// ============================================================================

void updateShot(sfRenderWindow* _window, sfCircleShape* _shot)
{
    shot* tempShot = dlShot;

    while (tempShot != NULL)
    {
        // Update hit timer
        if (tempShot->hasHit == sfTrue)
        {
            tempShot->hitTimer += getDeltaTime();
            if (tempShot->hitTimer >= 3.0f)
            {
                tempShot->hasHit = sfFalse;
            }
        }

        // === PLAYER SHOTS vs ENEMIES ===
        if (tempShot->shooter != opponent)
        {
            ennemy* tempEnnemy = GetEnnemyList();
            sfCircleShape_setPosition(_shot, tempShot->pos);

            while (tempEnnemy != NULL)
            {
                sfFloatRect enemyRect = { tempEnnemy->pos.x, tempEnnemy->pos.y, 93, 84 };

                if (Rectangle_Collision(sfCircleShape_getGlobalBounds(_shot), enemyRect))
                {
                    if (tempShot->typeShot != SpellCardShot)
                    {
                        tempShot->life--;
                    }

                    if (tempShot->hasHit == sfFalse)
                    {
                        tempEnnemy->life--;
                        tempShot->hasHit = sfTrue;
                    }
                }

                if (tempEnnemy->life < 1)
                {
                    prepareParticleGroup(_window, 50,
                        (sfVector2f) {
                        tempEnnemy->pos.x + 47, tempEnnemy->pos.y + 42
                    },
                        40, 40);
                    tempEnnemy = retireEnnemy(tempEnnemy);
                }
                else
                {
                    tempEnnemy = tempEnnemy->pNext;
                }
            }
        }

        // === SPELL CARD COLLISION (destroys enemy bullets) ===
        if (tempShot->typeShot == SpellCardShot)
        {
            shot* tempShot2 = dlShot;

            while (tempShot2 != NULL)
            {
                if (tempShot->shooter != tempShot2->shooter &&
                    Circle_Collision(tempShot->pos, tempShot2->pos, tempShot->scale, tempShot2->scale))
                {
                    tempShot2->life--;
                }
                tempShot2 = tempShot2->pNext;
            }
        }

        // === ENEMY SHOTS vs PLAYER ===
        if (tempShot->shooter != ally)
        {
            player* tempPlayer = GetPlayerList();

            while (tempPlayer != NULL)
            {
                sfVector2f playerCenter = { tempPlayer->pos.x + 25, tempPlayer->pos.y + 25 };

                if (Circle_Collision(tempShot->pos, playerCenter, tempShot->scale, tempPlayer->scale))
                {
                    if (tempShot->hasHit == sfFalse)
                    {
                        tempPlayer->lifeForce -= tempShot->damage;
                        tempShot->hasHit = sfTrue;
                    }
                }

                tempPlayer = tempPlayer->pNext;
            }
        }

        // === SET TEXTURE ===
        if (tempShot->shooter != ally)
        {
            sfCircleShape_setTexture(_shot, GetTexture("ennemyShot"), NULL);
            sfCircleShape_setTextureRect(_shot, (sfIntRect) { 0, 0, 16, 16 });
        }
        else if (tempShot->typeShot == SpellCardShot)
        {
            sfCircleShape_setTexture(_shot, GetTexture("spellcards"), NULL);
            spellCardRect.left = spellCardRect.width * tempShot->color;
            sfCircleShape_setTextureRect(_shot, spellCardRect);
        }
        else
        {
            sfCircleShape_setTexture(_shot, GetTexture("shot"), NULL);
            sfCircleShape_setTextureRect(_shot, (sfIntRect) { 0, 0, 16, 16 });
        }

        // === CHECK IF SHOT SHOULD BE REMOVED ===
        if (tempShot->life < 1 ||
            tempShot->pos.y < -100 || tempShot->pos.y > 1200 ||
            tempShot->pos.x < -100 || tempShot->pos.x > 2100)
        {
            tempShot = retireShot(tempShot);
        }
        else
        {
            // Update position
            tempShot->pos.x -= tempShot->velocity.x * getDeltaTime();
            tempShot->pos.y -= tempShot->velocity.y * getDeltaTime();

            // Apply friction (gradual slowdown)
            tempShot->velocity.x -= tempShot->velocity.x * SHOT_FRICTION;
            tempShot->velocity.y -= tempShot->velocity.y * SHOT_FRICTION;

            // Draw
            sfCircleShape_setRadius(_shot, tempShot->scale);
            sfCircleShape_setPosition(_shot, tempShot->pos);
            sfRenderWindow_drawCircleShape(_window, _shot, NULL);

            tempShot = tempShot->pNext;
        }
    }
}