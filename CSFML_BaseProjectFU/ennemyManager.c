#include "ennemyManager.h"
#include "textureManager.h"
#include "particleManager.h"
#include "shotManager.h"
#include "tools.h"

// derniere element de la liste des ennemies
int ennemyNumber = 0;
ennemy* dlEnnemies;

// cr�� un �l�ment de la liste des ennemies


void initEnnemy()
{

}


// cree un element de la liste des ennemies
void prepareEnnemy(sfRenderWindow* _window)
{
	ennemyNumber++;
	ennemy* tempEnnemy= (ennemy*)calloc(1, sizeof(ennemy));
	tempEnnemy->life = 3;
	tempEnnemy->scale = 1;
	tempEnnemy->cooldown = 0;
	tempEnnemy->pos.y = 100;
	tempEnnemy->pos.x = -100;
	tempEnnemy->velocity.y = 0;
	tempEnnemy->velocity.x = 200;
	ajoutEnnemy(tempEnnemy);
}
// ajoute un element de la liste des ennemies
void ajoutEnnemy( ennemy* _ennemy)
{
	_ennemy->pNext = dlEnnemies;
	dlEnnemies = _ennemy;
}


// retire un element de la liste des ennemies
ennemy* retireEnnemy(ennemy* _ennemy)
{
	if (_ennemy == dlEnnemies)
	{
		ennemy* tempEnnemy = _ennemy->pNext;
		dlEnnemies = tempEnnemy;
		free(_ennemy);
		return tempEnnemy;
	}
	else
	{
		ennemy* tempEnnemy = dlEnnemies;
		while (tempEnnemy->pNext != _ennemy)
		{
			tempEnnemy = tempEnnemy->pNext;
		}
		tempEnnemy->pNext = _ennemy->pNext;
		free(_ennemy);
		return tempEnnemy->pNext;

	}

}
int getEnnemyNumber()
{

	return ennemyNumber;

}
int isActualEnnemyAlive()
{

	ennemy* tempEnnemy = dlEnnemies;
	if (tempEnnemy != NULL)
	{
		return 1;
	}
	return 0;

}
//affiche toutes les particules des groupe des ennemies
void updateEnnemy(sfRenderWindow* _window, sfSprite* _sprite)
{

	ennemy* tempEnnemy = dlEnnemies;
	while (tempEnnemy != NULL)
			{
	tempEnnemy->cooldown += getDeltaTime();
	if (tempEnnemy->cooldown >= 1.f)
	{
		prepareEnnemyShot(_window, &tempEnnemy->pos, 60,10,(sfVector2f){42,42});
		tempEnnemy->cooldown = 0.f;
	}
	if (tempEnnemy->pos.x >= 2100 || tempEnnemy->pos.y > 1200 || tempEnnemy->pos.y < -200 || tempEnnemy->pos.x < -200)
	{
		tempEnnemy = retireEnnemy(tempEnnemy);
	}
	else
	{
		sfSprite_setPosition(_sprite, tempEnnemy->pos);
		sfSprite_setTexture(_sprite, GetTexture("enemy"), sfTrue);
		sfSprite_setScale(_sprite, (sfVector2f) { tempEnnemy->scale, tempEnnemy->scale });
		sfRenderWindow_drawSprite(_window, _sprite, NULL);
		tempEnnemy->pos.x += tempEnnemy->velocity.x * getDeltaTime();
		tempEnnemy->pos.y += tempEnnemy->velocity.y * getDeltaTime();
		tempEnnemy = tempEnnemy->pNext;
	}

}
		
		
	
}
ennemy* GetEnnemyList()
{
	return dlEnnemies;
}

