#pragma once
#include "tools.h"
typedef struct player player;
struct player
{
	player* pNext;
	sfVector2f pos;
	sfVector2f velocity;
	int scale;

};


void preparePlayer(sfRenderWindow* _window);
void ajoutPlayer(player* _player);
void updatePlayer(sfRenderWindow* _window, sfCircleShape* _circle);
player* retirePlayer(player* _player);
void initPlayer();
void MovementPlayer(player* _player);