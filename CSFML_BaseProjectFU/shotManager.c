#include "shotManager.h"
#include "playerManager.h"
#include "textureManager.h"
#include "ennemyManager.h"
#include "tools.h"

// derniere element de la liste des joueurs
shot* dlShot = 0;


// cr�� un �l�ment de la liste des joueurs


void initShot()
{

}


// cree un element de la liste des joueurs
void preparePlayerShot(sfRenderWindow* _window , sfVector2f* _pos)
{
	shot* tempShot = (shot*)calloc(1, sizeof(shot));
	tempShot->shooter = ally;
	tempShot->typeShot = normalShot;
	tempShot->life = 1;
	tempShot->scale = 5;
	tempShot->pos.y = _pos->y;
	tempShot->pos.x = _pos->x +30;
	tempShot->velocity.y = 300;
	tempShot->velocity.x = 0;
	ajoutShot(tempShot);
}
// ajoute un element de la liste des joueurs
void ajoutShot(shot* _shot)
{
	_shot->pNext = dlShot;
	dlShot = _shot;
}

void prepareEnnemyShot(sfRenderWindow* _window, sfVector2f* _pos,int _nbShot)
{
	//prepareCircleShot(_window, _pos, _nbShot);
	prepareCrossShot(_window, _pos, _nbShot);

//	prepareLosangeShot(_window, _pos, _nbShot);
}
void prepareCircleShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
	float up = -3.14159265358979323846f;
	for (int i = 0; i < _nbShot; i++)
	{
		if (i < _nbShot / 2)
		{
			up += 6.2f / _nbShot;
		}
		else
		{
			up -= 6.2f / _nbShot;
		}
		float left = cos(up) * (20 / 1.2);
		float	top = Clamp(sin(up)) * (20 / 1.2);
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->scale = 10;
		tempShot->pos.y = (_pos->y + 42) + top;
		tempShot->pos.x = (_pos->x + 42) + left;
		//tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 10;
		//tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.y < 2000 && tempShot->pos.y < 1100)
		{

			ajoutShot(tempShot);
		}
	}
}
void prepareCrossShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
	float up = -3.14159265358979323846f;
	for (int i = 0; i < _nbShot; i++)
	{
		up += 6.2f / _nbShot;
		float left = tan(sin(up)) * (100 / 1.2);
		float	top = sin(up) * (100 / 1.2);
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->scale = 10;
		tempShot->pos.y = (_pos->y + 42) + top;
		tempShot->pos.x = (_pos->x + 42) + left;
		//tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 10;
		//tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.y < 2000 && tempShot->pos.y < 1100)
		{

			ajoutShot(tempShot);
		}
	}
}
void prepareRandomShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
	float up = -3.14159265358979323846f;
	for (int i = 0; i < _nbShot; i++)
	{
		up += 6.2f / _nbShot;
		float left = log(up) * (100 / 1.2);
		float	top = sqrt(up) * (100 / 1.2);
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->scale = 10;
		tempShot->pos.y = (_pos->y + 42) + top;
		tempShot->pos.x = (_pos->x + 42) + left;
		tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 10;
		tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.y < 2000 && tempShot->pos.y < 1100)
		{

			ajoutShot(tempShot);
		}
	}
}
void prepareLosangeShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
	float up = -3.14159265358979323846f;
	for (int i = 0; i < _nbShot; i++)
	{
		up += 6.2f / _nbShot;
		float left = tan(cos(up)) * (100 / 1.2);
		float	top = tan(sin(up)) * (100 / 1.2);
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->scale = 10;
		tempShot->pos.y = (_pos->y + 42) + top;
		tempShot->pos.x = (_pos->x + 42) + left;
		tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 10;
		tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.y < 2000 && tempShot->pos.y < 1100)
		{

			ajoutShot(tempShot);
		}
	}
}
void preparePrismShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
	float up = -3.14159265358979323846f;
	for (int i = 0; i < _nbShot; i++)
	{
		up += 6.2f / _nbShot;
		float left = acos(cos(up)) * (100 / 1.2);
		float	top = acos(sin(up)) * (100 / 1.2);
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->scale = 10;
		tempShot->pos.y = (_pos->y + 42) + top;
		tempShot->pos.x = (_pos->x + 42) + left;
		tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 10;
		tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.y < 2000 && tempShot->pos.y < 1100)
		{

			ajoutShot(tempShot);
		}
	}
}
void prepareLassoShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot)
{
	float up = -3.14159265358979323846f;
	for (int i = 0; i < _nbShot; i++)
	{
		up += 6.2f / _nbShot;
		float left = atan(sin(up)) * (100 / 1.2);
		float	top = cos(atan(up)) * (100 / 1.8);
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->scale = 10;
		tempShot->pos.y = (_pos->y + 42) + top;
		tempShot->pos.x = (_pos->x + 42) + left;
		tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 10;
		tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.y < 2000 && tempShot->pos.y < 1100)
		{

			ajoutShot(tempShot);
		}
	}
}
// retire un element de la liste des joueurs
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

//affiche toutes les particules des groupe des ennemies
void updateShot(sfRenderWindow* _window, sfCircleShape* _shot, sfSprite* _ennemies)
{
	shot* tempShot = dlShot;

	while (tempShot != NULL)
	{
		ennemy* tempEnnemy = dlEnnemies;
		sfCircleShape_setPosition(_shot, tempShot->pos);
		if (tempShot->shooter != opponent)
		{
			while (tempEnnemy != NULL)
			{
				if (Rectangle_Collision(sfCircleShape_getGlobalBounds(_shot), (sfFloatRect) { tempEnnemy->pos.x, tempEnnemy->pos.y, 93, 84 }))
				{
					tempShot->life--;
					tempEnnemy->life--;
				}
				tempEnnemy = tempEnnemy->pNext;
			}
		}
		if (tempShot->shooter != ally)
		{
		}
 		if (tempShot->life < 1 || tempShot->pos.y < 0 || tempShot->pos.y > 1100 || tempShot->pos.x < 0 || tempShot->pos.y > 2000)
		{
			tempShot = retireShot(tempShot);
		}
		else
		{
			sfCircleShape_setFillColor(_shot, sfBlue);
			sfCircleShape_setRadius(_shot, tempShot->scale);
	
			tempShot->pos.y -= tempShot->velocity.y * getDeltaTime();
			tempShot->pos.x -= tempShot->velocity.x * getDeltaTime();
				sfRenderWindow_drawCircleShape(_window, _shot, NULL);
			tempShot = tempShot->pNext;
		}
	}
}

