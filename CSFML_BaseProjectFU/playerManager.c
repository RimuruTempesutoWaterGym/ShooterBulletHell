
#include "playerManager.h"
#include "shotManager.h"
#include "textureManager.h"
#include "tools.h"

// derniere element de la liste des joueurs
player* dlPlayers = 0;
float timerSprite = 0;
float timerBall = 0;
int spriteFrame = 0;

sfIntRect  patchuliRect;

// cr�� un �l�ment de la liste des joueurs


void initPlayer()
{

}


// cree un element de la liste des joueurs
void preparePlayer(sfRenderWindow* _window)
{
	player* tempPlayer = (player*)calloc(1, sizeof(player));
	tempPlayer->scale = 5;
	tempPlayer->pos.y = 900;
	tempPlayer->pos.x = 900;
	tempPlayer->velocity.y = 300;
	tempPlayer->velocity.x = 300;
	ajoutPlayer(tempPlayer);
	patchuliRect = GetRect("patchuli");
}
// ajoute un element de la liste des joueurs
void ajoutPlayer(player* _player)
{
	_player->pNext = dlPlayers;
	dlPlayers = _player;
}


// retire un element de la liste des joueurs
player* retirePlayer(player* _player)
{
	if (_player == dlPlayers)
	{
		player* tempPlayer = _player->pNext;
		dlPlayers = tempPlayer;
		free(_player);
		return tempPlayer;
	}
	else
	{
		player* tempPlayer = dlPlayers;
		while (tempPlayer->pNext != _player)
		{
			tempPlayer = tempPlayer->pNext;
		}
		tempPlayer->pNext = _player->pNext;
		free(_player);
		return tempPlayer->pNext;

	}

}

//affiche toutes les particules des groupe des ennemies
void updatePlayer(sfRenderWindow* _window, sfSprite* _sprite, sfCircleShape* _hitPoint)
{

	timerBall += getDeltaTime();
	timerSprite += getDeltaTime();
	player* tempPlayer = dlPlayers;

	while (tempPlayer != NULL)
	{
		if (sfKeyboard_isKeyPressed(sfKeySpace) && timerBall > 0.05f)
		{
			preparePlayerShot(_window, &tempPlayer->pos);
			timerBall = 0.f;
	}
		
		MovementPlayer(tempPlayer);
		sfSprite_setPosition(_sprite, tempPlayer->pos);
		sfSprite_setTexture(_sprite, GetTexture("patchuli"), sfTrue);
		spriteFrame = (int){ timerSprite / GetFrameTime("patchuli") } % GetNbFrame("patchuli");
		patchuliRect.left = patchuliRect.width * spriteFrame;

		sfSprite_setTextureRect(_sprite, patchuliRect);

		sfRenderWindow_drawSprite(_window, _sprite, NULL);
		if (sfKeyboard_isKeyPressed(sfKeyC) && timerBall > 0.05f)
		{
			tempPlayer->velocity.y = 100;
			tempPlayer->velocity.x = 100;
			sfCircleShape_setFillColor(_hitPoint, sfRed);
			sfCircleShape_setOrigin(_hitPoint, (sfVector2f) { tempPlayer->scale, tempPlayer->scale  });
			sfCircleShape_setPosition(_hitPoint, (sfVector2f) { tempPlayer->pos.x + patchuliRect.width / 2, tempPlayer->pos.y + patchuliRect.height / 2 });
			sfCircleShape_setRadius(_hitPoint, tempPlayer->scale);
			sfRenderWindow_drawSprite(_window, _hitPoint, NULL);
		}
		else
		{
			tempPlayer->velocity.y = 300;
			tempPlayer->velocity.x = 300;
		}
		tempPlayer = tempPlayer->pNext;


	}


}
void MovementPlayer(player* _player)
{
	if(sfKeyboard_isKeyPressed(sfKeyD) && _player->pos.x < 1600)
	{
		_player->pos.x += _player->velocity.x * getDeltaTime();
	}
	if (sfKeyboard_isKeyPressed(sfKeyQ) && _player->pos.x >300)
	{
		_player->pos.x -= _player->velocity.x * getDeltaTime();
	}
	if (sfKeyboard_isKeyPressed(sfKeyZ) && _player->pos.y > 100)
	{
		_player->pos.y -= _player->velocity.y * getDeltaTime();
	}
	if (sfKeyboard_isKeyPressed(sfKeyS) && _player->pos.y < 900)
	{
		_player->pos.y += _player->velocity.y * getDeltaTime();
	}

}

