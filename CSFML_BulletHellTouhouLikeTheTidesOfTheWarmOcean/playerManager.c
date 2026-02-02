
#include "playerManager.h"
#include "shotManager.h"
#include "textureManager.h"
#include "scoreManager.h"

// derniere element de la liste des joueurs

float timerSprite = 0;
float timerBall = 0;
float timerImmortality = 0;
float timerinvu= 0;
float timerUlt = 0;
int spriteFrame = 0;
player* dlPlayers = 0;

int  standMode;

// cr�� un �l�ment de la liste des joueurs


void initPlayer()
{
	timerUlt = 2.f;
	patchuliRect = GetRect("patchuli");
}


// cree un element de la liste des joueurs
void preparePlayer(sfRenderWindow* _window)
{
	player* tempPlayer = (player*)calloc(1, sizeof(player));
	tempPlayer->scale = 5;
	tempPlayer->pos.y = 900;
	tempPlayer->score = 0;
	tempPlayer->pos.x = 900;
	tempPlayer->velocity.y = 500;
	tempPlayer->velocity.x = 500;
	tempPlayer->lifeForce = 3;
	tempPlayer->spellCard = 3;
	tempPlayer->isHitSinceLastBoss = 0;
	tempPlayer->noHitMultiplier = 1;
	tempPlayer->isInvu = 0;
	tempPlayer->godmode = sfFalse;
	ajoutPlayer(tempPlayer);

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
void updatePlayer(sfRenderWindow* _window)
{

	timerBall += getDeltaTime();
	timerImmortality += getDeltaTime();
	timerUlt += getDeltaTime();
	timerSprite += getDeltaTime();
	player* tempPlayer = dlPlayers;

	while (tempPlayer != NULL)
	{
	
		if (tempPlayer->isInvu)
		{
			timerinvu += getDeltaTime();
			if (timerinvu > 1.f)
			{
				tempPlayer->isInvu = 0;
				timerinvu = 0.f;
			}

		}
		if (sfKeyboard_isKeyPressed(sfKeyM) && timerImmortality > 1.f)
		{
			tempPlayer->godmode = !tempPlayer->godmode;
			timerImmortality = 0.f;
		}
		if (sfKeyboard_isKeyPressed(sfKeySpace) && timerBall > 0.02f)
		{
		
			if (standMode == 1)
			{
				if (timerBall > 0.05f)
				{

					prepareHomingShot(_window, &tempPlayer->pos);
					timerBall = 0.f;
				}
			}
			else
			{
				preparePlayerShot(_window, &tempPlayer->pos);
				timerBall = 0.f;
			}
			Play_SFX("laser");
		}
		if (tempPlayer->lifeForce < 1)
		{
	
			changeHighScore(tempPlayer->score);
			toggleDefeated();
			Play_SFX("death");
		}
		
		MovementPlayer(tempPlayer);

		spriteFrame = (int){ timerSprite / GetFrameTime("patchuli") } % GetNbFrame("patchuli");
		patchuliRect.left = patchuliRect.width * spriteFrame;



	
		if ( sfKeyboard_isKeyPressed(sfKeyF) && tempPlayer->spellCard > 0 && timerUlt > 2.f )
		{

			prepareSpellCardShot(_window, &tempPlayer->pos, 50, 50, (sfVector2f) { 25, 25 });
			timerUlt = 0.f;
			tempPlayer->spellCard--;
			Play_SFX("spellCard");
		}
		if (sfKeyboard_isKeyPressed(sfKeyC) )
		{
			standMode = 1;
			tempPlayer->velocity.y = 200;
			tempPlayer->velocity.x = 200;


		}
		else
		{
			standMode = 0;
			tempPlayer->velocity.y = 500;
			tempPlayer->velocity.x = 500;
		}
		tempPlayer = tempPlayer->pNext;


	}


}
void DisplayPlayer(sfRenderWindow* _window, sfSprite* _sprite, sfCircleShape* _hitPoint)
{
	player* tempPlayer = dlPlayers;

	while (tempPlayer != NULL)
	{
		sfSprite_setPosition(_sprite, tempPlayer->pos);
		sfSprite_setTexture(_sprite, GetTexture("patchuli"), sfTrue);
		sfSprite_setTextureRect(_sprite, patchuliRect);
		sfCircleShape_setTexture(_hitPoint, GetTexture("hitbox"), NULL);
		sfCircleShape_setOrigin(_hitPoint, (sfVector2f) { tempPlayer->scale, tempPlayer->scale });
		sfCircleShape_setPosition(_hitPoint, (sfVector2f) { tempPlayer->pos.x + patchuliRect.width / 2, tempPlayer->pos.y + patchuliRect.height / 2 });
		sfCircleShape_setRadius(_hitPoint, tempPlayer->scale);
		sfRenderWindow_drawSprite(_window, _sprite, NULL);
		if (standMode == 1)
		{
			sfRenderWindow_drawSprite(_window, _hitPoint, NULL);
		}
			tempPlayer = tempPlayer->pNext;
		}
	}
void DisplayPlayerLife(sfRenderWindow* _window, sfSprite* _sprite)
{
	player* tempPlayer = dlPlayers;

	while (tempPlayer != NULL)
	{
		for(int i = 0; i < tempPlayer->lifeForce; i++)
		{
			sfSprite_setPosition(_sprite, (sfVector2f) {700 + i * (20), 1000  });
			sfSprite_setTexture(_sprite, GetTexture("life"), sfTrue);
			sfRenderWindow_drawSprite(_window, _sprite, NULL);
		}
		tempPlayer = tempPlayer->pNext;
	}
}
void DisplayPlayerSpellcard(sfRenderWindow* _window, sfSprite* _sprite)
{
	player* tempPlayer = dlPlayers;

	while (tempPlayer != NULL)
	{
		for (int i = 0; i < tempPlayer->spellCard; i++)
		{
			sfSprite_setPosition(_sprite, (sfVector2f) { 1300 + i * (20), 1000 });
			sfSprite_setTexture(_sprite, GetTexture("spellcard"), sfTrue);
			sfRenderWindow_drawSprite(_window, _sprite, NULL);
		}
		tempPlayer = tempPlayer->pNext;
	}
}
void DisplayPlayerScore(sfRenderWindow* _window, sfText* _text)
{
	char buffer[50];
	player* tempPlayer = dlPlayers;
	while (tempPlayer != NULL)
	{
		sfText_setPosition(_text, (sfVector2f) { 100  , 1000 });
		sprintf(buffer, "score: %d							Multiplicateur Evasion: %d", tempPlayer->score, tempPlayer->noHitMultiplier);
		sfText_setString(_text, buffer);
		sfRenderWindow_drawText(_window, _text, NULL);
		tempPlayer = tempPlayer->pNext;
	}
}
void MovementPlayer(player* _player)
{
	if(sfKeyboard_isKeyPressed(sfKeyD) && _player->pos.x < 1870)
	{
		_player->pos.x += _player->velocity.x * getDeltaTime();
	}
	if (sfKeyboard_isKeyPressed(sfKeyQ) && _player->pos.x >0)
	{
		_player->pos.x -= _player->velocity.x * getDeltaTime();
	}
	if (sfKeyboard_isKeyPressed(sfKeyZ) && _player->pos.y > 0)
	{
		_player->pos.y -= _player->velocity.y * getDeltaTime();
	}
	if (sfKeyboard_isKeyPressed(sfKeyS) && _player->pos.y < 930)
	{
		_player->pos.y += _player->velocity.y * getDeltaTime();
	}

}
player* GetPlayerList()
{
	return dlPlayers;
}
void RemoveAllPlayers()
{
	player* tempPlayer = dlPlayers;
	while (tempPlayer != NULL)
	{
		player* toDelete = tempPlayer;
		tempPlayer = tempPlayer->pNext;
		free(toDelete);
	}
	dlPlayers = 0;
}

