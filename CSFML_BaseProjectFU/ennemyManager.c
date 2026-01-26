#include "ennemyManager.h"
#include "textureManager.h"
#include "particleManager.h"
#include "shotManager.h"
#include "tools.h"

// derniere element de la liste des ennemies
int ennemyNumber = 0;
ennemy* dlEnnemies;
int wave = 1;
// cr�� un �l�ment de la liste des ennemies


void initEnnemy()
{

}


// cree un element de la liste des ennemies
void prepareEnnemy(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    // Basic stats
    tempEnnemy->life = 3;
    tempEnnemy->maxLife = 3;
    tempEnnemy->scale = 1;
    tempEnnemy->cooldown = 0;

    // Initial state
    tempEnnemy->state = STATE_ENTER;

    // Entry pattern: come from top-left, move to center-top area
    tempEnnemy->startPos = (sfVector2f){ -100, 100 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 300 }; // Center-top of screen (assuming 1920x1080)
    tempEnnemy->pos = tempEnnemy->startPos;

    // Movement timing
    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 2.0f; // Takes 2 seconds to reach position

    // Pattern behavior (wave movement)
    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 150.0f;  // How far left/right to wave
    tempEnnemy->waveFrequency = 1.5f;    // How fast to wave


    ajoutEnnemy(tempEnnemy);
}
void prepareBoss1(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    // Junko's stats
    tempEnnemy->life = 200;  
    tempEnnemy->maxLife = 200;  // Final boss health
    tempEnnemy->scale = 2.5f;
    tempEnnemy->cooldown = 0;

    // Movement
    tempEnnemy->state = STATE_ENTER;
    tempEnnemy->startPos = (sfVector2f){ 960, -200 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 250 };
    tempEnnemy->pos = tempEnnemy->startPos;

    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 3.0f;

    // Gentle floating movement
    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 100.0f;
    tempEnnemy->waveFrequency = 0.3f;
    // Allocate boss-specific data
    BossData* bossData = (BossData*)calloc(1, sizeof(BossData));
    bossData->phaseTimer = 0.0f;
    bossData->currentPhase = 0;
    bossData->ringTimer = 0.0f;
    bossData->laneTimer = 0.0f;

    // Store boss data 
     tempEnnemy->bossData = bossData;

    ajoutEnnemy(tempEnnemy);
}
void prepareBossEnnemy(sfRenderWindow* _window)
{
    ennemyNumber++;
    ennemy* tempEnnemy = (ennemy*)calloc(1, sizeof(ennemy));

    // Boss stats
    tempEnnemy->life = 100;  // Boss has more health
    tempEnnemy->maxLife = 100;  // Boss has more health
    tempEnnemy->scale = 2.0f; // Bigger sprite
    tempEnnemy->cooldown = 0;

    // Movement state
    tempEnnemy->state = STATE_ENTER;

    // Boss enters from top center
    tempEnnemy->startPos = (sfVector2f){ 960, -150 };
    tempEnnemy->targetPos = (sfVector2f){ 960, 200 };  // Stop near top
    tempEnnemy->pos = tempEnnemy->startPos;

    // Movement timing
    tempEnnemy->moveTimer = 0.0f;
    tempEnnemy->moveDuration = 2.5f;  // Slow, dramatic entrance

    // Pattern behavior - smooth horizontal movement
    tempEnnemy->patternTimer = 0.0f;
    tempEnnemy->waveAmplitude = 300.0f;  // Move left-right across screen
    tempEnnemy->waveFrequency = 0.5f;    // Slow sweeping motion

    tempEnnemy->velocity = (sfVector2f){ 0, 0 };

    ajoutEnnemy(tempEnnemy);
}


// ajoute un element de la liste des ennemies
void ajoutEnnemy( ennemy* _ennemy)
{
	_ennemy->pNext = dlEnnemies;
	dlEnnemies = _ennemy;
}

void bossAttackPurelyBulletHell(sfRenderWindow* _window, sfVector2f* _pos, BossData* bossData,float _life, float _maxHealth)
{
    bossData->phaseTimer += getDeltaTime();
    bossData->ringTimer += getDeltaTime();

    // Phase 1: Slow purple/red rings (0-15 seconds)
    if (_life > _maxHealth / 2)
    {
        if (bossData->ringTimer >= 0.3f)  // Slow, deliberate rings
        {
            // Purple ring
            int bulletCount = 70;
            wave = -wave;
            float angle = 0;
            for (int i = 0; i < bulletCount; i++)
            {
                if (wave == -1)
                {
                     angle = (bulletCount/20) +(i / (float)bulletCount) * 2.0f * 3.14159265359f;
                }
                else
                {
                     angle =  (i / (float)bulletCount) * 2.0f * 3.14159265359f;
                }
                shot* tempShot = (shot*)calloc(1, sizeof(shot));
                tempShot->shooter = opponent;
                tempShot->typeShot = normalShot;
                tempShot->life = 1;
                tempShot->damage = 1;
                tempShot->scale = 15;
                tempShot->hasHit = sfFalse;
                tempShot->hitTimer = 0;

                tempShot->pos.x = _pos->x + 47 ;
                tempShot->pos.y = _pos->y + 42 ;

                // Slow speed for sluggish feel
                float speed = 200.0f;
                tempShot->velocity.x = -cos(angle) * speed;
                tempShot->velocity.y = -sin(angle) * speed;

                ajoutShot(tempShot);
            }

            bossData->ringTimer = 0.0f;
        }
    }
    // Phase 2: Add fast blue rings on top (15-30 seconds)
    else if ( _life < _maxHealth/2 && _life > _maxHealth / 4)
    {
        // Continue purple rings but faster
        if (bossData->ringTimer >= 0.8f)
        {
            int bulletCount = 40;
            float randomOffset = (rand() % 628) / 100.0f;

            for (int i = 0; i < bulletCount; i++)
            {
                float angle = randomOffset + (i / (float)bulletCount) * 2.0f * 3.14159265359f;

                shot* tempShot = (shot*)calloc(1, sizeof(shot));
                tempShot->shooter = opponent;
                tempShot->typeShot = normalShot;
                tempShot->life = 1;
                tempShot->damage = 1;
                tempShot->scale = 10;
                tempShot->hasHit = sfFalse;
                tempShot->hitTimer = 0;

                tempShot->pos.x = _pos->x + 47;
                tempShot->pos.y = _pos->y + 42;

                // Fast blue bullets
                float speed = 300.f;
                tempShot->velocity.x = -cos(angle) * speed;
                tempShot->velocity.y = -sin(angle) * speed;

                ajoutShot(tempShot);
            }

            bossData->ringTimer = 0.0f;
        }
    }
    // Phase 3: CHAOS - Dense rings + confining lanes (30+ seconds)
    else
    {
        bossData->laneTimer += getDeltaTime();

        // Super dense rings
        if (bossData->ringTimer >= 0.5f)
        {
            int bulletCount = 48;  // VERY dense
            float randomOffset = (rand() % 628) / 100.0f;

            for (int i = 0; i < bulletCount; i++)
            {
                float angle = randomOffset + (i / (float)bulletCount) * 2.0f * 3.14159265359f;

                shot* tempShot = (shot*)calloc(1, sizeof(shot));
                tempShot->shooter = opponent;
                tempShot->typeShot = normalShot;
                tempShot->life = 1;
                tempShot->damage = 1;
                tempShot->scale = 8;
                tempShot->hasHit = sfFalse;
                tempShot->hitTimer = 0;

                tempShot->pos.x = _pos->x + 47;
                tempShot->pos.y = _pos->y + 42;

                float speed = 150.0f + (rand() % 60);
                tempShot->velocity.x = -cos(angle) * speed;
                tempShot->velocity.y = -sin(angle) * speed;

                ajoutShot(tempShot);
            }

            bossData->ringTimer = 0.0f;
        }

        // 24-way confining lanes
        if (bossData->laneTimer >= 2.0f)
        {
            int laneCount = 24;

            for (int i = 0; i < laneCount; i++)
            {
                float angle = (i / (float)laneCount) * 2.0f * 3.14159265359f;

                // Create a "lane" of bullets
                for (int depth = 0; depth < 8; depth++)
                {
                    shot* tempShot = (shot*)calloc(1, sizeof(shot));
                    tempShot->shooter = opponent;
                    tempShot->typeShot = normalShot;
                    tempShot->life = 1;
                    tempShot->damage = 1;
                    tempShot->scale = 12    ;
                    tempShot->hasHit = sfFalse;
                    tempShot->hitTimer = 0;

                    // Spread bullets out along the lane
                    float distance = 50.0f + (depth * 30.0f);
                    tempShot->pos.x = _pos->x + 47 + cos(angle) * distance;
                    tempShot->pos.y = _pos->y + 42 + sin(angle) * distance;

                    // All move outward slowly
                    float speed = 100.0f;
                    tempShot->velocity.x = -cos(angle) * speed;
                    tempShot->velocity.y = -sin(angle) * speed;

                    ajoutShot(tempShot);
                }
            }

            bossData->laneTimer = 0.0f;
        }
    }
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
     
        tempEnnemy->moveTimer += getDeltaTime();
            switch (tempEnnemy->state)
            {
            case STATE_ENTER:
            {
                float t = tempEnnemy->moveTimer / tempEnnemy->moveDuration;
                if (t >= 1.0f)
                {
                    t = 1.0f;
                    tempEnnemy->state = STATE_PATTERN;
                    tempEnnemy->moveTimer = 0.0f;
                    tempEnnemy->patternTimer = 0.0f;
                }

                // Ease in
                t = easeInOutQuad(t);
                tempEnnemy->pos = vec2f_lerp(tempEnnemy->startPos, tempEnnemy->targetPos, t);
                break;
            }

            case STATE_PATTERN:
            {
                tempEnnemy->patternTimer += getDeltaTime();

                // Boss moves side to side smoothly
                float wave = sin(tempEnnemy->patternTimer * tempEnnemy->waveFrequency) * tempEnnemy->waveAmplitude;
                tempEnnemy->pos.x = tempEnnemy->targetPos.x + wave;

                // Attack cooldown
                tempEnnemy->cooldown += getDeltaTime();

                // Rotating spiral every 0.1 seconds
                if (tempEnnemy->bossData != NULL)
                {
                    bossAttackPurelyBulletHell(_window, &tempEnnemy->pos, tempEnnemy->bossData, tempEnnemy->life,tempEnnemy->maxLife);
                }
                if (tempEnnemy->bossData == NULL && tempEnnemy->cooldown >= 0.1f)
                {
                    bossAttackSpiral(_window, &tempEnnemy->pos, tempEnnemy->patternTimer);
                    tempEnnemy->cooldown = 0.0f;
                }
           
                // Dense ring every 3 seconds
                static float ringTimer = 0.0f;
                ringTimer += getDeltaTime();
                if (tempEnnemy->bossData == NULL && ringTimer >= 3.0f)
                {
                    bossAttackExpandingRing(_window, &tempEnnemy->pos);
                    ringTimer = 0.0f;
                }

                break;
            }
        }
       
        
  
            // Draw
            sfSprite_setPosition(_sprite, tempEnnemy->pos);
            sfSprite_setTexture(_sprite, GetTexture("enemy"), sfTrue);
            sfSprite_setScale(_sprite, (sfVector2f) { tempEnnemy->scale, tempEnnemy->scale });
            sfRenderWindow_drawSprite(_window, _sprite, NULL);
        

            tempEnnemy = tempEnnemy->pNext;
      
    }
}

ennemy* GetEnnemyList()
{
	return dlEnnemies;
}

