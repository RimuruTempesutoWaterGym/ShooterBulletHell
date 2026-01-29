 #pragma once
#include "tools.h"


typedef enum
{
	opponent,
	ally,
}shooter;
typedef enum 
{
normalShot,
FlowerShot,
SpellCardShot,
}typeShot;
typedef struct shot shot;
struct shot
{
	shot* pNext;
	sfVector2f pos;
	sfVector2f velocity;
	shooter shooter;
	typeShot typeShot;
	int life;
	int damage;
	int scale;
	float hitTimer;
	sfBool hasHit;
	int color;

};


void preparePlayerShot(sfRenderWindow* _window, sfVector2f* _pos);
void prepareEnnemyShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale, sfVector2f _centering);
void prepareSpellCardShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale, sfVector2f _centering);
void prepareCircleShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale,int damage, sfVector2f _centering, int _origin, int _typeOfShot);
void prepareRandomShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot);
void prepareLosangeShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot);
void preparePrismShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot);
void prepareLassoShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot);
void prepareCrossShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot);
void ajoutShot(shot* _shot);
shot* createShot(sfVector2f position, float angle, float speed, int scale, int damage);
void createBulletRing(sfVector2f center, int bulletCount, float angleOffset, float speed, int scale, int damage);
void createAimedShot(sfVector2f fromPos, sfVector2f toPos, float speed, int scale, int damage);
void createAimedSpread(sfVector2f fromPos, sfVector2f toPos, int bulletCount, float spreadAngle, float speed, int scale, int damage);
void createBulletLane(sfVector2f center, float angle, int depth, float spacing, float speed, int scale, int damage);
void createRadialLanes(sfVector2f center, int laneCount, int bulletsPerLane, float spacing, float speed, int scale, int damage);
void createBulletSpiral(sfVector2f center, int streamCount, int bulletsPerStream, float baseAngle, float bulletSpacing, float speedStart, float speedDecay, int scale, int damage);
void createCrossPattern(sfVector2f center, int bulletsPerArm, float armSpacing, float speed, int scale, int damage);
void createRandomScatter(sfVector2f center, int bulletCount, float minSpeed, float maxSpeed, int scale, int damage);
void createOverlappingRings(sfVector2f center, int ringCount, int bulletsPerRing, float ringOffset, float speedStart, float speedDecay, int scaleStart, int scaleGrowth, int damage);
void updateShot(sfRenderWindow* _window);
void DisplayShot(sfRenderWindow* _window, sfCircleShape* _circle);
shot* retireShot(shot* _shot);
void initShot();