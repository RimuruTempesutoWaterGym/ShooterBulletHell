 #pragma once
#include "tools.h"
#include "playerManager.h"

typedef enum
{
	opponent,
	ally,
}shooter;
typedef enum 
{
normalShot,
FlowerShot,
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
	int scale;

};


void preparePlayerShot(sfRenderWindow* _window, sfVector2f* _pos);
void prepareEnnemyShot(sfRenderWindow* _window, sfVector2f* _pos,int _nbShot);
void ajoutShot(shot* _shot);
void updateShot(sfRenderWindow* _window, sfCircleShape* _circle);
shot* retireShot(shot* _shot);
void initShot();