

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
sfIntRect backdoorShotRect;
int backdoorShotFrame;
float backdoorShotTimeAnim;

// ============================================================================
// CONSTANTS
// ============================================================================

#define PI 3.14159265358979323846f
#define SHOT_FRICTION 0.00005f  // Deceleration factor

// ============================================================================
// INITIALIZATION
// ============================================================================

void initShot()
{
    dlShot = NULL;
    spellCardRect = GetRect("spellcards");
    backdoorShotRect = GetRect("backdoorShot");
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
        tempShot->damage = 1;
        tempShot->scale = 5;
        tempShot->pos.x = _pos->x + (i * 10) + 5;
        tempShot->pos.y = _pos->y;
        tempShot->velocity.x = 0;
        tempShot->velocity.y = 800;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        ajoutShot(tempShot);
    }
}

void prepareHomingShot(sfRenderWindow* _window, sfVector2f* _pos)
{
    for (int i = 0; i < 2; i++)
    {
        shot* tempShot = (shot*)calloc(1, sizeof(shot));
        tempShot->shooter = ally;
        tempShot->typeShot = HomingShot;
        tempShot->life = 1;
        tempShot->damage = 1;
        tempShot->scale = 8;
        tempShot->pos.x = _pos->x + (i * 20) + 10;
        tempShot->pos.y = _pos->y;
        tempShot->velocity.x = 0;
        tempShot->velocity.y = 800;
        tempShot->hasHit = sfFalse;
        tempShot->hitTimer = 0;

        ajoutShot(tempShot);
    }
}

void prepareSpellCardShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale, sfVector2f _centering)
{
    prepareCircleShot(_window, _pos, _nbShot, _scale, 300, _centering, ally, SpellCardShot);
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
//  TIME KNIFE PATTERNS
// ============================================================================


void createTimeKnives(sfVector2f center, int waveCount)
{
    for (int wave = 0; wave < waveCount; wave++)
    {
        int knivesPerWave = 8;
        float waveAngleOffset = (wave * 0.5f);

        for (int i = 0; i < knivesPerWave; i++)
        {
            float angle = waveAngleOffset + (i / (float)knivesPerWave) * 2.0f * PI;

            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = TimeKnifeShot;  // New type
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 12;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            tempShot->pos = center;

            // Initial fast velocity
            float speed = 300.0f;
            tempShot->velocity.x = -cos(angle) * speed;
            tempShot->velocity.y = -sin(angle) * speed;

            // Store data for time-stop behavior
            tempShot->timeStopTimer = 0.0f;
            tempShot->timeStopDuration = 1.0f + (wave * 0.3f);  // Stop for longer each wave
            tempShot->hasTimeStopped = sfFalse;

            ajoutShot(tempShot);
        }
    }
}

void createDeflationWorld(sfVector2f center, int rings)
{
    for (int ring = 0; ring < rings; ring++)
    {
        int knivesInRing = 16 + (ring * 4);
        float ringOffset = ring * 0.3f;

        for (int i = 0; i < knivesInRing; i++)
        {
            float angle = ringOffset + (i / (float)knivesInRing) * 2.0f * PI;

            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = TimeKnifeShot;
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 10;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            // Spawn slightly offset from center
            tempShot->pos.x = center.x + cos(angle) * 20.0f;
            tempShot->pos.y = center.y + sin(angle) * 20.0f;

            float speed = 200.0f - (ring * 30.0f);
            tempShot->velocity.x = -cos(angle) * speed;
            tempShot->velocity.y = -sin(angle) * speed;

            tempShot->timeStopTimer = 0.0f;
            tempShot->timeStopDuration = 0.8f;
            tempShot->hasTimeStopped = sfFalse;

            ajoutShot(tempShot);
        }
    }
}

void createPerfectSquare(sfVector2f center)
{
    int gridSize = 7;
    float spacing = 60.0f;
    float startX = center.x - (gridSize / 2) * spacing;
    float startY = center.y - 200.0f;

    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = TimeKnifeShot;
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 8;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            tempShot->pos.x = startX + (col * spacing);
            tempShot->pos.y = startY + (row * spacing * 0.3f);

            // All fall downward
            tempShot->velocity.x = 0;
            tempShot->velocity.y = -150.0f;

            // Staggered time stops
            tempShot->timeStopTimer = 0.0f;
            tempShot->timeStopDuration = 0.5f + (row * 0.1f);
            tempShot->hasTimeStopped = sfFalse;

            ajoutShot(tempShot);
        }
    }
}


void createEternalMeek(sfVector2f center, sfVector2f playerPos)
{
    int burstCount = 5;

    for (int burst = 0; burst < burstCount; burst++)
    {
        float dx = playerPos.x - center.x;
        float dy = playerPos.y - center.y;
        float baseAngle = atan2(dy, dx);

        int knivesPerBurst = 7;
        float spread = 0.6f;

        for (int i = 0; i < knivesPerBurst; i++)
        {
            float offset = ((i / (float)(knivesPerBurst - 1)) - 0.5f) * spread;
            float angle = baseAngle + offset;

            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = TimeKnifeShot;
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 10;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            tempShot->pos = center;

            float speed = 350.0f - (burst * 50.0f);
            tempShot->velocity.x = -cos(angle) * speed;
            tempShot->velocity.y = -sin(angle) * speed;

            // Each burst stops at different time
            tempShot->timeStopTimer = 0.0f;
            tempShot->timeStopDuration = 0.4f + (burst * 0.2f);
            tempShot->hasTimeStopped = sfFalse;

            ajoutShot(tempShot);
        }
    }
}
// ============================================================================
//  BACKDOOR PATTERNS
// ============================================================================


void createBackdoorSpiral(sfVector2f center)
{
    int portalCount = 6;

    for (int portal = 0; portal < portalCount; portal++)
    {
        float portalAngle = (portal / (float)portalCount) * 2.0f * PI;
        float portalDistance = 250.0f;

        sfVector2f portalPos;
        portalPos.x = center.x + cos(portalAngle) * portalDistance;
        portalPos.y = center.y + sin(portalAngle) * portalDistance;

        // Each portal shoots bullets inward and outward
        for (int direction = 0; direction < 2; direction++)
        {
            for (int i = 0; i < 8; i++)
            {
                float shootAngle = portalAngle + PI + (i * 0.2f);
                if (direction == 1) shootAngle += PI; // Reverse direction

                shot* tempShot = (shot*)calloc(1, sizeof(shot));
                tempShot->shooter = opponent;
                tempShot->typeShot = BackdoorShot;  // New type
                tempShot->life = 1;
                tempShot->damage = 1;
                tempShot->scale = 10;
                tempShot->hasHit = sfFalse;
                tempShot->hitTimer = 0;

                tempShot->pos = portalPos;

                float speed = 200.0f;
                tempShot->velocity.x = -cos(shootAngle) * speed;
                tempShot->velocity.y = -sin(shootAngle) * speed;

                // Backdoor data
                tempShot->backdoorTimer = 0.0f;
                tempShot->backdoorTeleportTime = 1.5f + (i * 0.1f);
                tempShot->hasTeleported = sfFalse;
                tempShot->portalIndex = portal;

                ajoutShot(tempShot);
            }
        }
    }
}

//  Cross-portal teleportation
void createSummerBackdoor(sfVector2f center)
{
    int doorCount = 8;

    for (int door = 0; door < doorCount; door++)
    {
        float doorAngle = (door / (float)doorCount) * 2.0f * PI;

        // Doors at screen edges
        sfVector2f doorPos;
        doorPos.x = center.x + cos(doorAngle) * 400.0f;
        doorPos.y = center.y + sin(doorAngle) * 300.0f;

        // Each door shoots toward opposite door
        float oppositeAngle = doorAngle + PI;

        for (int i = 0; i < 5; i++)
        {
            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = BackdoorShot;
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 12;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            tempShot->pos = doorPos;

            float speed = 250.0f;
            tempShot->velocity.x = -cos(oppositeAngle) * speed;
            tempShot->velocity.y = -sin(oppositeAngle) * speed;

            tempShot->backdoorTimer = 0.0f;
            tempShot->backdoorTeleportTime = 0.8f;
            tempShot->hasTeleported = sfFalse;
            tempShot->portalIndex = door;
            tempShot->targetPortalIndex = (door + 4) % doorCount; // Opposite portal

            // Store teleport destination
            tempShot->teleportPos.x = center.x + cos(oppositeAngle) * 400.0f;
            tempShot->teleportPos.y = center.y + sin(oppositeAngle) * 300.0f;

            ajoutShot(tempShot);
        }
    }
}

// Bullets emerge from center portal
void createCrystallizedRelease(sfVector2f center)
{
    int waveCount = 5;

    for (int wave = 0; wave < waveCount; wave++)
    {
        int bulletsPerWave = 24 + (wave * 8);
        float waveOffset = wave * 0.3f;

        for (int i = 0; i < bulletsPerWave; i++)
        {
            float angle = waveOffset + (i / (float)bulletsPerWave) * 2.0f * PI;

            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = BackdoorShot;
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 8;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            // All start at center (portal)
            tempShot->pos = center;

            // But don't appear until teleport time
            float speed = 180.0f - (wave * 20.0f);
            tempShot->velocity.x = -cos(angle) * speed;
            tempShot->velocity.y = -sin(angle) * speed;

            tempShot->backdoorTimer = 0.0f;
            tempShot->backdoorTeleportTime = wave * 0.4f; // Staggered appearance
            tempShot->hasTeleported = sfFalse;
            tempShot->isInvisible = sfTrue; // Hidden until teleport

            ajoutShot(tempShot);
        }
    }
}

//  Curved portal paths
void createButterflyDream(sfVector2f center, sfVector2f playerPos)
{
    int portalPairs = 4;

    for (int pair = 0; pair < portalPairs; pair++)
    {
        float pairAngle = (pair / (float)portalPairs) * 2.0f * PI;

        // Portal 1
        sfVector2f portal1;
        portal1.x = center.x + cos(pairAngle) * 300.0f;
        portal1.y = center.y + sin(pairAngle) * 200.0f;

        // Portal 2 (opposite side)
        sfVector2f portal2;
        portal2.x = center.x + cos(pairAngle + PI) * 300.0f;
        portal2.y = center.y + sin(pairAngle + PI) * 200.0f;

        // Bullets curve from portal1 to portal2 toward player
        for (int i = 0; i < 12; i++)
        {
            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = BackdoorShot;
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 10;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            tempShot->pos = portal1;

            // Initial velocity toward player
            float dx = playerPos.x - portal1.x;
            float dy = playerPos.y - portal1.y;
            float angleToPlayer = atan2(dy, dx);

            float spread = ((i / 12.0f) - 0.5f) * 1.0f;
            angleToPlayer += spread;

            float speed = 220.0f;
            tempShot->velocity.x = -cos(angleToPlayer) * speed;
            tempShot->velocity.y = -sin(angleToPlayer) * speed;

            tempShot->backdoorTimer = 0.0f;
            tempShot->backdoorTeleportTime = 1.0f;
            tempShot->hasTeleported = sfFalse;
            tempShot->teleportPos = portal2;

            ajoutShot(tempShot);
        }
    }
}
void createSevenStars(sfVector2f center)
{
    int starPoints = 7;

    for (int point = 0; point < starPoints; point++)
    {
        float pointAngle = (point / (float)starPoints) * 2.0f * PI;

        // Star point position
        sfVector2f starPos;
        starPos.x = center.x + cos(pointAngle) * 280.0f;
        starPos.y = center.y + sin(pointAngle) * 280.0f;

        // Each star point shoots inward
        for (int i = 0; i < 6; i++)
        {
            shot* tempShot = (shot*)calloc(1, sizeof(shot));
            tempShot->shooter = opponent;
            tempShot->typeShot = BackdoorShot;
            tempShot->life = 1;
            tempShot->damage = 1;
            tempShot->scale = 11;
            tempShot->hasHit = sfFalse;
            tempShot->hitTimer = 0;

            tempShot->pos = starPos;

            // Shoot toward center with slight spread
            float shootAngle = pointAngle + PI + ((i - 3) * 0.15f);

            float speed = 190.0f;
            tempShot->velocity.x = -cos(shootAngle) * speed;
            tempShot->velocity.y = -sin(shootAngle) * speed;

            // After reaching center, teleport to next star point
            tempShot->backdoorTimer = 0.0f;
            tempShot->backdoorTeleportTime = 1.2f;
            tempShot->hasTeleported = sfFalse;

            // Teleport to next star point
            int nextPoint = (point + 3) % starPoints; // Skip 3 points
            tempShot->teleportPos.x = center.x + cos((nextPoint / (float)starPoints) * 2.0f * PI) * 280.0f;
            tempShot->teleportPos.y = center.y + sin((nextPoint / (float)starPoints) * 2.0f * PI) * 280.0f;

            ajoutShot(tempShot);
        }
    }
}
// ============================================================================
// UPDATE & COLLISION DETECTION
// ============================================================================

void updateShot(sfRenderWindow* _window)
{

	backdoorShotTimeAnim += getDeltaTime() ;
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
                tempShot->hitTimer = 0.f;
            }
        }

        // === PLAYER SHOTS vs ENEMIES ===
        if (tempShot->shooter == ally)
        {
            ennemy* tempEnnemy = GetEnnemyList();


            while (tempEnnemy != NULL)
            {
                sfFloatRect ballRect = { tempShot->pos.x, tempShot->pos.y,tempShot->scale,tempShot->scale };
                sfFloatRect enemyRect = { tempEnnemy->pos.x, tempEnnemy->pos.y };
                if (tempEnnemy->bossData != NULL)
                {
                    enemyRect.height = 100 * tempEnnemy->scale;
                    enemyRect.width = 50 * tempEnnemy->scale;
                }
                else
                {
                    enemyRect.height = 16 * tempEnnemy->scale;
                    enemyRect.width = 16 * tempEnnemy->scale;

                }
                if (Rectangle_Collision(ballRect, enemyRect))
                {
                    if (tempShot->typeShot != SpellCardShot)
                    {
                        tempShot->life--;
                    }

                    if (tempShot->hasHit == sfFalse)
                    {
                         tempEnnemy->life-= tempShot->damage;
                        tempShot->hasHit = sfTrue;
                    }
                }

                if (tempEnnemy->life < 1)
                {
                    player* tempPlayer = GetPlayerList();

                    while (tempPlayer != NULL)
                    {
                        if (tempEnnemy->bossData != NULL)
                        {
                            if (tempPlayer->isHitSinceLastBoss == 0)
                            {
 
                                prepareParticleNewLootGroup(_window, 0, 1, 1, tempEnnemy->pointValue,
                                    (sfVector2f) {
                                    tempEnnemy->pos.x + 8 * tempEnnemy->scale, tempEnnemy->pos.y + 8 * tempEnnemy->scale
                                },
                                    8 * tempEnnemy->scale, 8 * tempEnnemy->scale);
                            }
                            else
                            {
                                prepareParticleNewLootGroup(_window, 0, 0, 0, tempEnnemy->pointValue,
                                    (sfVector2f) {
                                    tempEnnemy->pos.x + 8 * tempEnnemy->scale, tempEnnemy->pos.y + 8 * tempEnnemy->scale
                                },
                                    8 * tempEnnemy->scale, 8 * tempEnnemy->scale);
                                tempPlayer->isHitSinceLastBoss = 0;
                            }

                        }

           
                        tempPlayer->noHitMultiplier++;
                        tempPlayer = tempPlayer->pNext;
                 
                        Play_SFX("defeated");
                    }

                    prepareParticleNewLootGroup(_window, 50, 0, 0, tempEnnemy->pointValue,
                        (sfVector2f) 
                        {
                            tempEnnemy->pos.x + 8 * tempEnnemy->scale, tempEnnemy->pos.y + 8 * tempEnnemy->scale
                        },
                        8 * tempEnnemy->scale, 8 * tempEnnemy->scale);

                    prepareParticleGroup(_window, 50,
                        (sfVector2f) {
                        tempEnnemy->pos.x + 8 * tempEnnemy->scale, tempEnnemy->pos.y + 8 * tempEnnemy->scale
                        },
                        8* tempEnnemy->scale, 8 * tempEnnemy->scale);
                    tempEnnemy = retireEnnemy(tempEnnemy);
                }
                else
                {
                    tempEnnemy = tempEnnemy->pNext;
                }
            }
        }
        if (tempShot->shooter == ally && tempShot->typeShot == HomingShot)
        {
  
            ennemy* nearestEnemy = NULL;
            float nearestDistance = 99999.0f;

            ennemy* tempEnnemy = GetEnnemyList();
            while (tempEnnemy != NULL)
            {
                float dx = tempEnnemy->pos.x - tempShot->pos.x;
                float dy = tempEnnemy->pos.y - tempShot->pos.y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < nearestDistance)
                {
                    nearestDistance = distance;
                    nearestEnemy = tempEnnemy;
                }

                tempEnnemy = tempEnnemy->pNext;
            }

 
            if (nearestEnemy != NULL)
            {
                float dx = (nearestEnemy->pos.x + 25) - tempShot->pos.x;
                float dy = (nearestEnemy->pos.y + 25) - tempShot->pos.y;
                float angle = atan2(dy, dx);


                float homingStrength = 200.0f;
                float targetVelX = -cos(angle) * 500.0f;
                float targetVelY = -sin(angle) * 500.0f;


                tempShot->velocity.x += (targetVelX - tempShot->velocity.x) * homingStrength * getDeltaTime();
                tempShot->velocity.y += (targetVelY - tempShot->velocity.y) * homingStrength * getDeltaTime();
            }
        }
        // === SPELL CARD COLLISION (destroys enemy bullets) ===
        if (tempShot->typeShot == SpellCardShot)
        {
            shot* tempShot2 = dlShot;

            while (tempShot2 != NULL)
            {
                sfVector2f shotCenter = { tempShot->pos.x + tempShot->scale , tempShot->pos.y + tempShot->scale };
                sfVector2f shotCenter2 = { tempShot2->pos.x + tempShot2->scale , tempShot2->pos.y + tempShot2->scale };
                if (tempShot->shooter != tempShot2->shooter &&
                    Circle_Collision(shotCenter, shotCenter2, tempShot->scale, tempShot2->scale))
                {
                    tempShot2->life--;
                }
                tempShot2 = tempShot2->pNext;
            }
        }

        // === ENEMY SHOTS vs PLAYER ===
        if (tempShot->shooter == opponent)
        {
            // === TIME KNIFE BEHAVIOR ===
            if (tempShot->typeShot == TimeKnifeShot)
            {
                tempShot->timeStopTimer += getDeltaTime();

                // Time to stop?
                if (!tempShot->hasTimeStopped &&
                    tempShot->timeStopTimer >= tempShot->timeStopDuration)
                {
                    // FREEZE!
                    tempShot->hasTimeStopped = sfTrue;
                    tempShot->storedVelocity = tempShot->velocity;
                    tempShot->velocity.x = 0;
                    tempShot->velocity.y = 0;
                    tempShot->timeStopTimer = 0.0f;  // Reset for resume
                }

                // Time to resume?
                if (tempShot->hasTimeStopped &&
                    tempShot->timeStopTimer >= 1.5f)  // Frozen for 1.5 seconds
                {
                    // RESUME!
                    tempShot->velocity = tempShot->storedVelocity;
                    // Add acceleration on resume
                    tempShot->velocity.x *= 1.5f;
                    tempShot->velocity.y *= 1.5f;
                }
            }
            // === BACKDOOR PORTAL BEHAVIOR ===
            if (tempShot->typeShot == BackdoorShot)
            {
                tempShot->backdoorTimer += getDeltaTime();

                // Time to teleport/appear?
                if (!tempShot->hasTeleported &&
                    tempShot->backdoorTimer >= tempShot->backdoorTeleportTime)
                {
                    tempShot->hasTeleported = sfTrue;

                    // If invisible (spawn from portal)
                    if (tempShot->isInvisible)
                    {
                        tempShot->isInvisible = sfFalse; // Now visible
                    }
                    // If has teleport position (portal warp)
                    else if (tempShot->teleportPos.x != 0 || tempShot->teleportPos.y != 0)
                    {
                        tempShot->pos = tempShot->teleportPos;

                        // Reverse or randomize direction after teleport
                        tempShot->velocity.x *= -1.2f;
                        tempShot->velocity.y *= -1.2f;
                    }
                }
            }
  

            player* tempPlayer = GetPlayerList();

            while (tempPlayer != NULL)
            {
                sfVector2f playerCenter = { tempPlayer->pos.x + patchuliRect.width / 2 , tempPlayer->pos.y + patchuliRect.height / 2  };
                sfVector2f shotCenter = { tempShot->pos.x + tempShot->scale , tempShot->pos.y + tempShot->scale };

                if (Circle_Collision(shotCenter, playerCenter, tempShot->scale, tempPlayer->scale))
                {
                    if (tempShot->hasHit == sfFalse && tempPlayer->isInvu == 0 && tempPlayer->godmode == sfFalse)
                    {
                        tempPlayer->isInvu = 1;
						tempPlayer->isHitSinceLastBoss = 1;
						tempPlayer->noHitMultiplier = 1;
                        tempPlayer->lifeForce -= tempShot->damage;
                        tempShot->hasHit = sfTrue;
                        Play_SFX("hit");
                    }
                }

                tempPlayer = tempPlayer->pNext;
            }
        }
     
        if (tempShot->life < 1|| tempShot->pos.y < -100 || tempShot->pos.y > 1200 || tempShot->pos.x < -100 || tempShot->pos.x > 2100)
        { 
         
            tempShot = retireShot(tempShot);
        }   
  
        else
        {
            if (tempShot->typeShot != BackdoorShot || !tempShot->isInvisible)
            {
                // Update position
                tempShot->pos.x -= tempShot->velocity.x * getDeltaTime();
                tempShot->pos.y -= tempShot->velocity.y * getDeltaTime();

                // Apply friction ONLY to non-time-knife shots when not frozen
                if (tempShot->typeShot != TimeKnifeShot || !tempShot->hasTimeStopped)
                {
                    tempShot->velocity.x -= tempShot->velocity.x * SHOT_FRICTION;
                    tempShot->velocity.y -= tempShot->velocity.y * SHOT_FRICTION;
                }
            }

            tempShot = tempShot->pNext;
        }
    }
}
void DisplayShot(sfRenderWindow* _window, sfCircleShape* _shot)
{
    shot* tempShot = dlShot;

    while (tempShot != NULL)
    {
        sfCircleShape_setPosition(_shot, tempShot->pos);
        // === SET TEXTURE ===
        if (tempShot->shooter == opponent)
        {
            sfCircleShape_setTexture(_shot, GetTexture("ennemyShot"), NULL);
            sfCircleShape_setTextureRect(_shot, (sfIntRect) { 0, 0, 16, 16 });
        }
        if (tempShot->shooter == ally)
        {
            sfCircleShape_setTexture(_shot, GetTexture("shot"), NULL);
            sfCircleShape_setTextureRect(_shot, (sfIntRect) { 0, 0, 16, 16 });
        }
        if (tempShot->typeShot == SpellCardShot)
        {
            sfCircleShape_setTexture(_shot, GetTexture("spellcards"), NULL);
            spellCardRect.left = spellCardRect.width * tempShot->color;
            sfCircleShape_setTextureRect(_shot, spellCardRect);
        }
        else if (tempShot->typeShot == BackdoorShot)
        {
            printf("dd");
            // Portal bullet - glowing effect
            sfCircleShape_setTexture(_shot, GetTexture("backdoorShot"), NULL);
            backdoorShotFrame = (int){ backdoorShotTimeAnim / GetFrameTime("backdoorShot") } % GetNbFrame("backdoorShot");
            backdoorShotRect.left = backdoorShotRect.width * backdoorShotFrame;
            sfCircleShape_setTextureRect(_shot,backdoorShotRect);
            // Pulse before teleport
            if (!tempShot->hasTeleported &&
                tempShot->backdoorTimer > tempShot->backdoorTeleportTime - 0.3f)
            {
                float pulse = (sin(tempShot->backdoorTimer * 20.0f) + 1.0f) / 2.0f;
                sfUint8 alpha = (sfUint8)(150 + pulse * 105);
                sfCircleShape_setFillColor(_shot, (sfColor) { 255, 100, 255, alpha });
            }
            else
            {
                sfCircleShape_setFillColor(_shot, (sfColor) { 200, 50, 200, 255 });
            }
        }
        else if (tempShot->typeShot == TimeKnifeShot)
        {
            // iceShard
            sfCircleShape_setTexture(_shot, GetTexture("iceShard"), NULL);

            // Flash white when frozen
            if (tempShot->hasTimeStopped)
            {
                float pulse = (sin(tempShot->timeStopTimer * 10.0f) + 1.0f) / 2.0f;
                sfUint8 brightness = (sfUint8)(200 + pulse * 55);
                sfCircleShape_setFillColor(_shot, (sfColor) { brightness, brightness, 255, 255 });
            }
            else
            {
                sfCircleShape_setFillColor(_shot, sfWhite);
            }
        }
     


        // === CHECK IF SHOT SHOULD BE REMOVED ===

            // Draw
            sfCircleShape_setRadius(_shot, tempShot->scale);
            sfCircleShape_setPosition(_shot, tempShot->pos);
            sfRenderWindow_drawCircleShape(_window, _shot, NULL);

            tempShot = tempShot->pNext;
        
    }
}
void RemoveAllShots()
{
    shot* tempShot = dlShot;
    while (tempShot != NULL)
    {
        shot* toDelete = tempShot;
        tempShot = tempShot->pNext;
        free(toDelete);
    }
    dlShot = 0;
}