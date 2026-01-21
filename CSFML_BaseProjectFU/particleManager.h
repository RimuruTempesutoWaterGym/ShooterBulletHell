#pragma once
#include "tools.h"

// liste de particles
typedef struct particle particle;
struct particle
{
	particle* pNext;
	int color;
	int fadeValue;
	sfVector2f pos;
	sfVector2f velocity;
	float lifeTime;
	float spawnTime;
	int scale;

};
// liste de groupe de particle
typedef struct particleGroup particleGroup;
struct particleGroup
{
	particleGroup* pNext;
	particle* particlesLast;
	float particlesLife;
	float time;
	int numberOfParticles;


};
void prepareParticleGroup(sfRenderWindow* _window, int nbParticles, sfVector2f _pos,int _rangeY, int _rangeX);
void prepareParticle(sfRenderWindow* _window, particleGroup* _particleGroup, sfVector2f _pos, int _rangeY, int _rangeX);
void ajoutParticle(particleGroup* _particleGroup, particle* _particle);
void ajoutParticleGroup(particleGroup* _particleGroup);
void updateParticle(sfRenderWindow* _window, sfCircleShape* _circle);
particleGroup* retireParticleGroup(particleGroup* _particleGroup);
particle* retireParticle(particle* _particle, particleGroup* _particleGroup);
void initParticle();