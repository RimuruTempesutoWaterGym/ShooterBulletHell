#include "particleManager.h"
#include "tools.h"

// derniere element de la liste de groupe de particules
particleGroup* dlParticleGroup = 0;

	float lifeTime = 0;
// cr�� un �l�ment de la liste de groupe de particules


void initParticle()
{

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
	tempParticle->color = sfBlue;
	tempParticle->spawnTime = rand_int(0, 2);
	tempParticle->lifeTime = rand_int(2, 4) + tempParticle->spawnTime;
	tempParticle->scale = rand_int(5, 15);
	tempParticle->pos.x = _pos.x + rand_int(-_rangeX, _rangeX);
	tempParticle->pos.y = _pos.y + rand_int(-_rangeY, _rangeY);
	tempParticle->velocity.x = rand_int(-_rangeX, _rangeX);
	tempParticle->velocity.y = rand_int(-_rangeY, _rangeY);
	ajoutParticle(_particleGroup, tempParticle);
	return tempParticle;
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
		while (tempParticle->pNext != _particle)
		{
			tempParticle = tempParticle->pNext;
		}
		tempParticle->pNext = _particle->pNext;
		free(_particle);
		return tempParticle->pNext;

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
//affiche toutes les particules des groupe de particules
void updateParticle(sfRenderWindow* _window, sfCircleShape* _circle)
{

	particleGroup* tempParticleGroup = dlParticleGroup;

	while (tempParticleGroup != NULL)
	{
		tempParticleGroup->time += getDeltaTime();
		if (tempParticleGroup->time < tempParticleGroup->particlesLife)
		{

			particle* tempParticle = tempParticleGroup->particlesLast;

			while (tempParticle != NULL)
			{
				if (tempParticleGroup->time < tempParticle->lifeTime && tempParticleGroup->time > tempParticle->spawnTime)
				{
					if (tempParticleGroup->time > tempParticle->lifeTime - 1 && tempParticle->color.a > 0)
					{

						tempParticle->color.a -= lerp(tempParticle->color.a, 1, 6 * getDeltaTime());

					}
					sfCircleShape_setOrigin(_circle, (sfVector2f) { (float) { tempParticle->scale }, (float) { tempParticle->scale } });
					sfCircleShape_setPosition(_circle, tempParticle->pos);
					sfCircleShape_setFillColor(_circle, tempParticle->color);
					sfCircleShape_setRadius(_circle, tempParticle->scale);
					sfRenderWindow_drawCircleShape(_window, _circle, sfFalse);
					sfCircleShape_setOrigin(_circle, (sfVector2f) { (float) { (tempParticle->scale - 5) }, (float) { (tempParticle->scale - 5) } });
					sfCircleShape_setPosition(_circle, tempParticle->pos);
					sfCircleShape_setFillColor(_circle, tempParticle->color);
					sfCircleShape_setRadius(_circle, tempParticle->scale - 5);
					sfRenderWindow_drawCircleShape(_window, _circle, sfFalse);
					tempParticle->pos.x += tempParticle->velocity.x * getDeltaTime();
					tempParticle->pos.y += tempParticle->velocity.y * getDeltaTime();
					tempParticle = tempParticle->pNext;

				}
				else
				{
					tempParticle = retireParticle(tempParticle, tempParticleGroup);

				}


			}
			tempParticleGroup = tempParticleGroup->pNext;
		}
		else
		{
			tempParticleGroup = retireParticleGroup(tempParticleGroup);

		}
	}
}
