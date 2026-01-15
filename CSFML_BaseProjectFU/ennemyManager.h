#pragma once
#include "tools.h"
// liste d'ennemies
typedef struct ennemy ennemy;
struct ennemy
{
	ennemy* pNext;

	sfVector2f pos;
	sfVector2f velocity;
	int life;
	int scale;
	float cooldown;

};

ennemy* dlEnnemies ;
void prepareEnnemy(sfRenderWindow* _window);
void ajoutEnnemy(ennemy* _ennemy);
void updateEnnemy(sfRenderWindow* _window, sfSprite* _sprite);
ennemy* retireEnnemy(ennemy* _ennemy);
void initEnnemy();