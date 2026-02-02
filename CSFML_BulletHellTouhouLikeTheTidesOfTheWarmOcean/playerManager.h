#pragma once
#include "tools.h"
typedef struct player player;
struct player
{
	player* pNext;
	sfVector2f pos;
	sfVector2f velocity;
	int scale;
	int lifeForce;
	int spellCard;
	int score;
	int isHitSinceLastBoss;
	int noHitMultiplier;
	int isInvu;
	sfBool godmode;

};


sfIntRect  patchuliRect;

void preparePlayer(sfRenderWindow* _window);
void ajoutPlayer(player* _player);
void updatePlayer(sfRenderWindow* _window);
void DisplayPlayer(sfRenderWindow* _window, sfSprite* _sprite, sfCircleShape* _hitPoint);
player* retirePlayer(player* _player);
void initPlayer();
void MovementPlayer(player* _player);
player* GetPlayerList();
void RemoveAllPlayers();
void DisplayPlayerLife(sfRenderWindow* _window, sfSprite* _sprite);
void DisplayPlayerSpellcard(sfRenderWindow* _window, sfSprite* _sprite);
void DisplayPlayerScore(sfRenderWindow* _window, sfText* _text);