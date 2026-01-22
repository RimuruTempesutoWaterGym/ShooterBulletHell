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
void updateShot(sfRenderWindow* _window, sfCircleShape* _circle);
shot* retireShot(shot* _shot);
void initShot();