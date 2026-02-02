#include "textureManager.h"
#include "particleManager.h"
#include "playerManager.h"
#include "tools.h"

// derniere element de la liste de groupe de particules
particleGroup* dlParticleGroup = 0;
	float lifeTime = 0;


// cr�� un �l�ment de la liste de groupe de particules
	sfIntRect particlesRect;
	sfIntRect lootpartRect;

	void initParticle()
{
	 particlesRect = GetRect("particles");
	 lootpartRect = GetRect("lootpart");

	
}

// cree un element de la liste de groupe de particules
void prepareParticleGroup(sfRenderWindow * _window, int nbParticles, sfVector2f _pos,int _rangeY,int _rangeX)

{

	particleGroup * tempParticleGroup = (particleGroup*)calloc(1, sizeof(particleGroup));
	tempParticleGroup->particlesLife = 10;
	tempParticleGroup->time = 0;
	tempParticleGroup->numberOfParticles = nbParticles;

	for (int i = 0; i < tempParticleGroup->numberOfParticles; i++)
	{
		prepareParticle(_window, tempParticleGroup,_pos,_rangeY,_rangeX);
	}
	ajoutParticleGroup(tempParticleGroup);
}
// cree un element de la liste de particules
void prepareParticle(sfRenderWindow* _window, particleGroup* _particleGroup,sfVector2f _pos, int _rangeY, int _rangeX)
{
	particle* tempParticle = (particle*)calloc(1, sizeof(particle));
	tempParticle->color = rand_int(0,14);
	tempParticle->fadeValue =255;
	tempParticle->spawnTime = rand_int(0, 2);
	tempParticle->lifeTime = rand_int(2, 4) + tempParticle->spawnTime;
	tempParticle->scale = rand_int(5, 15);
	tempParticle->pos.x = _pos.x + rand_int(-_rangeX, _rangeX);
	tempParticle->pos.y = _pos.y + rand_int(-_rangeY, _rangeY);
	tempParticle->velocity.x = rand_int(-_rangeX, _rangeX);
	tempParticle->velocity.y = rand_int(-_rangeY, _rangeY);
	ajoutParticle(_particleGroup, tempParticle);
	
}
void prepareParticleNewLootGroup(sfRenderWindow* _window, int nbParticles,int nbSpellTag, int nbLifeForce,int _points,  sfVector2f _pos, int _rangeY, int _rangeX)
{

	particleGroup* tempParticleGroup = (particleGroup*)calloc(1, sizeof(particleGroup));
	tempParticleGroup->particlesLife = 200;
	tempParticleGroup->time = 0;
	tempParticleGroup->numberOfParticles = nbParticles;

	for (int i = 0; i < tempParticleGroup->numberOfParticles; i++)
	{
		prepareParticleNewLoot(_window, tempParticleGroup, _points / nbParticles, _pos, _rangeY, _rangeX);
	}
	for (int i = 0; i < nbSpellTag; i++)
	{
		prepareParticleSpellCard(_window, tempParticleGroup, _pos, _rangeY, _rangeX);
	}
	for (int i = 0; i < nbLifeForce; i++)
	{
		prepareParticleLifeForce(_window, tempParticleGroup, _pos, _rangeY, _rangeX);
	}
	ajoutParticleGroup(tempParticleGroup);
}
void prepareParticleNewLoot(sfRenderWindow* _window, particleGroup* _particleGroup, int _points, sfVector2f _pos, int _rangeY, int _rangeX)
{
	particle* tempParticle = (particle*)calloc(1, sizeof(particle));
	tempParticle->color = rand_int(0, 14);
	tempParticle->fadeValue = 255;
	tempParticle->spawnTime = 0;
	tempParticle->lifeTime = 200;
	tempParticle->scale = rand_int(5, 15);
	tempParticle->pos.x = _pos.x + rand_int(-_rangeX, _rangeX);
	tempParticle->pos.y = _pos.y + rand_int(-_rangeY, _rangeY);
	tempParticle->velocity.x = rand_int(200, 400);
	tempParticle->velocity.y = rand_int(200, 400);
	tempParticle->pointValue = _points;
	tempParticle->type= pointPart;
	ajoutParticle(_particleGroup, tempParticle);

}
void prepareParticleLifeForce(sfRenderWindow* _window, particleGroup* _particleGroup, sfVector2f _pos, int _rangeY, int _rangeX)
{
	particle* tempParticle = (particle*)calloc(1, sizeof(particle));
	tempParticle->color = rand_int(0, 14);
	tempParticle->fadeValue = 255;
	tempParticle->spawnTime = 0;
	tempParticle->lifeTime = 200;
	tempParticle->scale = rand_int(5, 15);
	tempParticle->pos.x = _pos.x + rand_int(-_rangeX, _rangeX);
	tempParticle->pos.y = _pos.y + rand_int(-_rangeY, _rangeY);
	tempParticle->velocity.x = 300;
	tempParticle->velocity.y = 300;
	tempParticle->type = lifeforcePart;
	ajoutParticle(_particleGroup, tempParticle);

}
void prepareParticleSpellCard(sfRenderWindow* _window, particleGroup* _particleGroup, sfVector2f _pos, int _rangeY, int _rangeX)
{
	particle* tempParticle = (particle*)calloc(1, sizeof(particle));
	tempParticle->color = rand_int(0, 14);
	tempParticle->fadeValue = 255;
	tempParticle->spawnTime = 0;
	tempParticle->lifeTime = 200;
	tempParticle->scale = rand_int(5, 15);
	tempParticle->pos.x = _pos.x + rand_int(-_rangeX, _rangeX);
	tempParticle->pos.y = _pos.y + rand_int(-_rangeY, _rangeY);
	tempParticle->velocity.x = 300;
	tempParticle->velocity.y = 300;
	tempParticle->type = spellcardPart;
	ajoutParticle(_particleGroup, tempParticle);

}
// ajoute un element de la liste de particules
void ajoutParticle(particleGroup* _particleGroup, particle* _particle)
{
	_particle->pNext = _particleGroup->particlesLast;
	_particleGroup->particlesLast = _particle;
}
// ajoute un element de la liste de groupe de particules
void ajoutParticleGroup(particleGroup* _particleGroup)
{
	_particleGroup->pNext = dlParticleGroup;
	dlParticleGroup = _particleGroup;
}

particle* retireParticle(particle* _particle, particleGroup* _particleGroup)
{
	if (_particle == NULL || _particleGroup == NULL)
	{
		return NULL;
	}

	if (_particle == _particleGroup->particlesLast)
	{
		particle* tempParticle = _particle->pNext;
		_particleGroup->particlesLast = tempParticle;
		free(_particle);
		return tempParticle;
	}
	else
	{
		particle* tempParticle = _particleGroup->particlesLast;
		while (tempParticle != NULL && tempParticle->pNext != _particle)
		{
			tempParticle = tempParticle->pNext;
		}

		if (tempParticle != NULL)
		{
			tempParticle->pNext = _particle->pNext;
			free(_particle);
			return tempParticle->pNext;
		}

		return NULL;
	}
}
particleGroup* retireParticleGroup(particleGroup* _particleGroup)
{
	if (_particleGroup == dlParticleGroup)
	{
		particleGroup* tempParticleGroup = _particleGroup->pNext;
		dlParticleGroup = tempParticleGroup;
		free(_particleGroup);
		return tempParticleGroup;
	}
	else
	{
		particleGroup* tempParticleGroup = dlParticleGroup;
		while (tempParticleGroup->pNext != _particleGroup)
		{
			tempParticleGroup = tempParticleGroup->pNext;
		}
		tempParticleGroup->pNext = _particleGroup->pNext;
		free(_particleGroup);
		return tempParticleGroup->pNext;

	}

}


void updateParticle(sfRenderWindow* _window)
{
	particleGroup* tempParticleGroup = dlParticleGroup;

	while (tempParticleGroup != NULL)
	{
		if (tempParticleGroup->particlesLast == NULL)
		{
			tempParticleGroup = retireParticleGroup(tempParticleGroup);
			continue;
		}

		tempParticleGroup->time += getDeltaTime();

		if (tempParticleGroup->time < tempParticleGroup->particlesLife)
		{
			particle* tempParticle = tempParticleGroup->particlesLast;

			while (tempParticle != NULL)
			{
				// Handle loot particles (homing + collision)
				if (tempParticle->type == pointPart ||
					tempParticle->type == lifeforcePart ||
					tempParticle->type == spellcardPart)
				{
					// === FIND NEAREST PLAYER ===
					player* nearestPlayer = NULL;
					float nearestDistance = 99999.0f;

					player* tempPlayer = GetPlayerList();
					while (tempPlayer != NULL)
					{
						float dx = tempPlayer->pos.x - tempParticle->pos.x;
						float dy = tempPlayer->pos.y - tempParticle->pos.y;
						float distance = sqrt(dx * dx + dy * dy);

						if (distance < nearestDistance)
						{
							nearestDistance = distance;
							nearestPlayer = tempPlayer;
						}

						tempPlayer = tempPlayer->pNext;
					}

					// === HOMING BEHAVIOR ===
					if (nearestPlayer != NULL)
					{
						float dx = (nearestPlayer->pos.x + patchuliRect.width / 2) - tempParticle->pos.x;
						float dy = (nearestPlayer->pos.y + patchuliRect.height / 2) - tempParticle->pos.y;
						float angle = atan2(dy, dx);

						float homingStrength = 200.0f;
						float targetVelX = cos(angle) * 500.0f;
						float targetVelY = sin(angle) * 500.0f;

						tempParticle->velocity.x += (targetVelX - tempParticle->velocity.x) * homingStrength * getDeltaTime();
						tempParticle->velocity.y += (targetVelY - tempParticle->velocity.y) * homingStrength * getDeltaTime();
					}

					// === COLLISION DETECTION ===
					sfBool particleCollected = sfFalse;
					tempPlayer = GetPlayerList();  // Reset player list

					while (tempPlayer != NULL && !particleCollected)
					{
						sfVector2f playerCenter = {
							tempPlayer->pos.x + patchuliRect.width / 2,
							tempPlayer->pos.y + patchuliRect.height / 2
						};

						sfVector2f particleCenter;
						if (tempParticle->type == pointPart)
						{
							particleCenter.x = tempParticle->pos.x + lootpartRect.width / 2;
							particleCenter.y = tempParticle->pos.y + lootpartRect.height / 2;
						}
						else
						{
							particleCenter.x = tempParticle->pos.x + particlesRect.width / 2;
							particleCenter.y = tempParticle->pos.y + particlesRect.height / 2;
						}

						// Check collision
						if (Circle_Collision(particleCenter, playerCenter, 10, tempPlayer->scale))
						{
							// Give reward
							if (tempParticle->type == pointPart)
							{
								tempPlayer->score += tempParticle->pointValue * tempPlayer->noHitMultiplier;
							}
							else if (tempParticle->type == lifeforcePart)
							{
								if (tempPlayer->lifeForce < 6)
								{
									tempPlayer->lifeForce++;
								}
							}
							else if (tempParticle->type == spellcardPart)
							{
								if (tempPlayer->spellCard < 6)
								{
									tempPlayer->spellCard++;
								}
							}

							particleCollected = sfTrue;  // Mark for removal
						}

						tempPlayer = tempPlayer->pNext;
					}

					// Remove particle if collected
					if (particleCollected)
					{
						tempParticle = retireParticle(tempParticle, tempParticleGroup);
						continue;  // Skip to next particle
					}
				}

				// === NORMAL PARTICLE BEHAVIOR ===
				if (tempParticleGroup->time >= tempParticle->spawnTime &&
					tempParticleGroup->time < tempParticle->lifeTime)
				{
					// Fade out near end of life
					if (tempParticleGroup->time > tempParticle->lifeTime - 1.0f &&
						tempParticle->fadeValue > 0)
					{
						tempParticle->fadeValue -= (int)(lerp(tempParticle->fadeValue, 1, 6.0f * getDeltaTime()));
						if (tempParticle->fadeValue < 0) tempParticle->fadeValue = 0;
					}

					// Update position
					tempParticle->pos.x += tempParticle->velocity.x * getDeltaTime();
					tempParticle->pos.y += tempParticle->velocity.y * getDeltaTime();

					tempParticle = tempParticle->pNext;
				}
				else
				{
					// Particle lifetime expired
					tempParticle = retireParticle(tempParticle, tempParticleGroup);
				}
			}

			tempParticleGroup = tempParticleGroup->pNext;
		}
		else
		{
			// Particle group lifetime expired
			tempParticleGroup = retireParticleGroup(tempParticleGroup);
		}
	}
}

void DisplayParticle(sfRenderWindow* _window, sfCircleShape* _circle)
{
	particleGroup* tempParticleGroup = dlParticleGroup;

	while (tempParticleGroup != NULL)
	{

		particle* tempParticle = tempParticleGroup->particlesLast;

		while (tempParticle != NULL)
		{

			sfCircleShape_setPosition(_circle, tempParticle->pos);
			sfCircleShape_setOrigin(_circle, (sfVector2f) { (float) { tempParticle->scale }, (float) { tempParticle->scale } });
			if(tempParticle->type == pointPart)
				sfCircleShape_setTexture(_circle, GetTexture("lootpart"), NULL);
			else if (tempParticle->type == spellcardPart)
				sfCircleShape_setTexture(_circle, GetTexture("spellcardpart"), NULL);
			else if (tempParticle->type == lifeforcePart)
				sfCircleShape_setTexture(_circle, GetTexture("heartpart"), NULL);

			else
				sfCircleShape_setTexture(_circle, GetTexture("particles"), NULL);
			if (tempParticle->type == pointPart)
			{
				lootpartRect.left = tempParticle->color * particlesRect.width/2;
				sfCircleShape_setTextureRect(_circle, lootpartRect);
			}
			else
			{
				particlesRect.left = tempParticle->color * particlesRect.width;
				sfCircleShape_setTextureRect(_circle, particlesRect);
			}
			sfCircleShape_setFillColor(_circle, (sfColor) { 255, 255, 255, tempParticle->fadeValue });
			sfCircleShape_setRadius(_circle, tempParticle->scale);
			sfRenderWindow_drawCircleShape(_window, _circle, sfFalse);

			tempParticle = tempParticle->pNext;
		}
		tempParticleGroup = tempParticleGroup->pNext;
	}
}
void RemoveAllParticles()
{

	particleGroup* tempParticleGroup = dlParticleGroup;
	while (tempParticleGroup != NULL)
	{
		particleGroup* toDelete = tempParticleGroup;
		tempParticleGroup = tempParticleGroup->pNext;
		free(toDelete);
	}
	dlParticleGroup = 0;
}