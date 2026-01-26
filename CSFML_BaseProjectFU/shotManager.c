#include "playerManager.h"
#include "shotManager.h"
#include "textureManager.h"
#include "ennemyManager.h"
#include "particleManager.h"
#include "tools.h"

// derniere element de la liste des joueurs
shot* dlShot = 0;


// cr�� un �l�ment de la liste des joueurs

sfIntRect spellCardRect;

void initShot()
{
	spellCardRect = GetRect("spellcards");
}


// cree un element de la liste des joueurs
void preparePlayerShot(sfRenderWindow* _window , sfVector2f* _pos)
{
	for (int i = 0; i < 4; i++)
	{
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = ally;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->scale = 5;
		tempShot->pos.y = _pos->y ;
		tempShot->pos.x = _pos->x + (i * 10)+5;
		tempShot->velocity.y = 300;
		tempShot->velocity.x = 0;
		ajoutShot(tempShot);
	}
}
// ajoute un element de la liste des joueurs
void ajoutShot(shot* _shot)
{
	_shot->pNext = dlShot;
	dlShot = _shot;
}

void prepareEnnemyShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale, sfVector2f _centering)
{
	prepareCircleShot(_window, _pos, _nbShot, _scale,1, _centering, opponent, normalShot);


}
void prepareSpellCardShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot, int _scale, sfVector2f _centering)
{
	
	prepareCircleShot(_window, _pos, _nbShot, _scale,3, _centering, ally, SpellCardShot);


}
void prepareCircleShot(sfRenderWindow* _window, sfVector2f* _pos, int _nbShot,int _scale,int _damage,sfVector2f _centering,int _origin,int _typeOfShot)
{
	int color = NULL;

	float up = -3.14159265358979323846f;
	for (int i = 0; i <= _nbShot; i++)
	{
		if (_typeOfShot == SpellCardShot)
		{
			color = rand_int(0, 14);
		}
	
		up += (3.14159265358979323846f*2) / _nbShot;
	
		float left = cos(up) * (80 / 1.2);
		float	top = sin(up) * (40 / 1.2);
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = _origin;
		tempShot->typeShot = _typeOfShot;
		tempShot->life = 1;
		tempShot->damage = _damage;
		tempShot->scale = _scale;
		tempShot->color = color;
		tempShot->hasHit = sfFalse;
		tempShot->hitTimer = 0;
		tempShot->pos.y = (_pos->y + _centering.y) + top;
		tempShot->pos.x = (_pos->x + _centering.x) + left;
		tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + _centering.y)) * 10;
		tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + _centering.x)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
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
		float left = cos(tan(up)) * (100 / 1.2);
		float	top = sin(up) * (100 / 1.2);
		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->scale = 10;
		tempShot->pos.y = (_pos->y + 42) + top;
		tempShot->pos.x = (_pos->x + 42) + left;
		tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 10;
		tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
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


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
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
		tempShot->damage = 1;
		tempShot->scale = 10;
		tempShot->pos.y = (_pos->y + 42) + top;
		tempShot->pos.x = (_pos->x + 42) + left;
		tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 10;
		tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 10;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
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
		tempShot->velocity.y = -(tempShot->pos.y - (_pos->y + 42)) * 5;
		tempShot->velocity.x = -(tempShot->pos.x - (_pos->x + 42)) * 5;


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
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


		if (tempShot->pos.x > 0 && tempShot->pos.y > 0 && tempShot->pos.x < 2000 && tempShot->pos.y < 1100)
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
// Boss attack pattern: Rotating Spiral
void bossAttackSpiral(sfRenderWindow* _window, sfVector2f* _pos, float patternTimer)
{
	// Creates a beautiful rotating spiral pattern
	static float spiralAngle = 0.0f;

	spiralAngle += getDeltaTime() * 2.0f; // Rotation speed

	// Fire 3 streams that rotate
	for (int stream = 0; stream < 3; stream++)
	{
		float streamAngle = spiralAngle + (stream * 2.094f); // 120 degrees apart

		// Each stream has 5 bullets
		for (int i = 0; i < 5; i++)
		{
			float bulletAngle = streamAngle + (i * 0.3f);

			shot* tempShot = (shot*)calloc(1, sizeof(shot));
			tempShot->shooter = opponent;
			tempShot->typeShot = normalShot;
			tempShot->life = 1;
			tempShot->damage = 1;
			tempShot->scale = 8;
			tempShot->hasHit = sfFalse;
			tempShot->hitTimer = 0;

			// Start at boss center
			tempShot->pos.x = _pos->x + 47;
			tempShot->pos.y = _pos->y + 42;

			// Velocity in spiral direction
			float speed = 150.0f - (i * 20.0f); // Bullets slow down as they go out
			tempShot->velocity.x = -cos(bulletAngle) * speed;
			tempShot->velocity.y = -sin(bulletAngle) * speed;

			ajoutShot(tempShot);
		}
	}
}
void bossAttackExpandingRing(sfRenderWindow* _window, sfVector2f* _pos)
{
	// Creates a dense ring that expands outward
	int bulletCount = 48; // Dense pattern

	for (int i = 0; i < bulletCount; i++)
	{
		float angle = (i / (float)bulletCount) * 2.0f * 3.14159265359f;

		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->damage = 1;
		tempShot->scale = 6;
		tempShot->hasHit = sfFalse;
		tempShot->hitTimer = 0;

		// Start at boss center
		tempShot->pos.x = _pos->x + 47;
		tempShot->pos.y = _pos->y + 42;

		// All bullets move outward at same speed
		float speed = 120.0f;
		tempShot->velocity.x = -cos(angle) * speed;
		tempShot->velocity.y = -sin(angle) * speed;

		ajoutShot(tempShot);
	}
}
void bossAttackAimedBurst(sfRenderWindow* _window, sfVector2f* _bossPos, sfVector2f* _playerPos)
{
	// Calculate angle to player
	float dx = _playerPos->x - (_bossPos->x + 47);
	float dy = _playerPos->y - (_bossPos->y + 42);
	float angleToPlayer = atan2(dy, dx);

	// Fire spread of bullets around player direction
	int bulletCount = 7;
	float spreadAngle = 0.5f; // Spread in radians

	for (int i = 0; i < bulletCount; i++)
	{
		float offset = ((i / (float)(bulletCount - 1)) - 0.5f) * spreadAngle;
		float angle = angleToPlayer + offset;

		shot* tempShot = (shot*)calloc(1, sizeof(shot));
		tempShot->shooter = opponent;
		tempShot->typeShot = normalShot;
		tempShot->life = 1;
		tempShot->damage = 1;
		tempShot->scale = 10;
		tempShot->hasHit = sfFalse;
		tempShot->hitTimer = 0;

		tempShot->pos.x = _bossPos->x + 47;
		tempShot->pos.y = _bossPos->y + 42;

		float speed = 200.0f;
		tempShot->velocity.x = -cos(angle) * speed;
		tempShot->velocity.y = -sin(angle) * speed;

		ajoutShot(tempShot);
	}
}
//affiche toutes les particules des groupe des ennemies
void updateShot(sfRenderWindow* _window, sfCircleShape* _shot)
{
	shot* tempShot = dlShot;

	while (tempShot != NULL)
	{
		if (tempShot->hasHit == sfTrue)
		{
			tempShot->hitTimer += getDeltaTime();
		}
		if (tempShot->hitTimer >= 3.f)
		{
			tempShot->hasHit = sfFalse;
		}
		ennemy* tempEnnemy = GetEnnemyList();
		sfCircleShape_setPosition(_shot, tempShot->pos);
		if (tempShot->shooter != opponent)
		{
			while (tempEnnemy != NULL)
			{
				if (Rectangle_Collision(sfCircleShape_getGlobalBounds(_shot), (sfFloatRect) { tempEnnemy->pos.x, tempEnnemy->pos.y, 93, 84 }))
				{
					if (tempShot->typeShot != SpellCardShot)
					{
						tempShot->life--;
					}
					if (tempShot->hasHit == sfFalse)
					{
						tempEnnemy->life--;
					tempShot->hasHit = sfTrue;
					}
				
				}
				if (tempEnnemy->life < 1 )
				{

					prepareParticleGroup(_window, 50, (sfVector2f) { tempEnnemy->pos.x + 47, tempEnnemy->pos.y + 42 }, 40, 40);

					tempEnnemy = retireEnnemy(tempEnnemy);
				}
				else {
					tempEnnemy = tempEnnemy->pNext;
				}
			
			}
		}
		shot* tempShot2 = dlShot;
		if (tempShot->typeShot == SpellCardShot)
		{
		
			while (tempShot2 != NULL)
			{
				if (tempShot->shooter != tempShot2->shooter && Circle_Collision(tempShot->pos, tempShot2->pos, tempShot->scale, tempShot2->scale))
				{

					tempShot2->life--;
				}
				tempShot2 = tempShot2->pNext;
			}

		}
		player* tempPlayer = GetPlayerList();
		if (tempShot->shooter != ally)
		{
			while (tempPlayer != NULL)
			{
				if (Circle_Collision(tempShot->pos, (sfVector2f) { tempPlayer->pos.x + 25,tempPlayer->pos.y + 25 }, tempShot->scale, tempPlayer->scale))
				{
				if(tempShot->hasHit == sfFalse)
					tempPlayer->lifeForce-= tempShot->damage;
				tempShot->hasHit = sfTrue;
				}

					tempPlayer = tempPlayer->pNext;
				

			}
			sfCircleShape_setTexture(_shot, GetTexture("ennemyShot"),NULL);
		}
		sfCircleShape_setTextureRect(_shot, (sfIntRect){0,0,16,16});
		if(tempShot->shooter != ally)
		{
		sfCircleShape_setTexture(_shot, GetTexture("ennemyShot"), NULL);
		}
		else if( tempShot->typeShot == SpellCardShot)
		{
			sfCircleShape_setTexture(_shot, GetTexture("spellcards"), NULL);
			spellCardRect.left = spellCardRect.width * tempShot->color;
			sfCircleShape_setTextureRect(_shot, spellCardRect);
		}
		else
		{
				sfCircleShape_setTexture(_shot, GetTexture("shot"), NULL);
				
		}
 		if (tempShot->life < 1 || tempShot->pos.y < -100 || tempShot->pos.y > 1200 || tempShot->pos.x < -100 || tempShot->pos.x > 2100)
		{
			tempShot = retireShot(tempShot);
		}
		else
		{
		
			sfCircleShape_setRadius(_shot, tempShot->scale);
			tempShot->pos.y -= tempShot->velocity.y * getDeltaTime();
			tempShot->pos.x -= tempShot->velocity.x * getDeltaTime();
				tempShot->velocity.x -= tempShot->velocity.x * getDeltaTime() / 20;
				tempShot->velocity.y -= tempShot->velocity.y * getDeltaTime() / 20;
			sfRenderWindow_drawCircleShape(_window, _shot, NULL);
			tempShot = tempShot->pNext;
		}
	}
}

