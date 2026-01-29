#include "game.h"
#include "gamepadx.h"
#include "textureManager.h"
#include "viewManager.h"
#include "stateManager.h"
#include "Gamepad.h"
#include "pause.h"
#include "soundManager.h"
#include "ennemyManager.h"
#include "particleManager.h"
#include "playerManager.h"
#include "shotManager.h"
#include "waveManager.h"


#include <Windows.h>

//sfSprite* sprite;
sfSprite* spBG1;
sfSprite* spBG2;
sfSprite* spEnnemy;
sfSprite* spPlayer;
sfCircleShape* spShot;
sfCircleShape* spParticle;
sfCircleShape* spHitboxPlayer;



sfBool gamepadChangePos;
sfVector2f BG1Pos;
sfVector2f BG2Pos;
float ennemyTimer;
float startTimer;


float timer;
WaveManager waveManager;
int nbreJoueur;
int waveCount;

float waveTimer;
float soloTimer;
float defaultwaveTimer;
float defaultsoloTimer;
float bossTimer;

sfBool phaseWave;
sfBool phaseBoss;
sfBool phaseBossCompleted;
sfBool gameover;

void initGame(Window* _window)
{
	Texture_Onload(GAME);
	Sound_Onload(GAME);
	Play_Music(GAME);

	initPlayer();
	initParticle();
	initShot();

	spBG1 = sfSprite_create();
	spBG2 = sfSprite_create();
	spEnnemy = sfSprite_create();
	spPlayer = sfSprite_create();
	spShot = sfCircleShape_create();
	spParticle = sfCircleShape_create();
	spHitboxPlayer = sfCircleShape_create();
	initWaveManager(&waveManager);
	startWave(&waveManager, 1);
	sfSprite_setTexture(spBG1, GetTexture("bg"), sfTrue);
	sfSprite_setTexture(spBG2, GetTexture("bg"), sfTrue);

	sfSprite_setPosition(spBG2, vector2f(0.0f, -1080.0f));
	

	preparePlayer(_window);
	sfVector3f ambientLight = { 0.1f, 0.1f, 0.3f };
	
	BG1Pos = vector2f(0.0f, 0.0f);
	BG2Pos = vector2f(0.0f, -1080.0f);

	SetViewPosition(mainView, vector2f(960.f, 540.f));
	
	
	GamepadDetection();



	w.state = sfTrue;

}

void updateGame(Window* _window)
{
	if (!isPaused)
	{
		timer += getDeltaTime();



		updateWaveManager(&waveManager, _window->renderWindow);

		if (!waveManager.waveActive && !isActualEnnemyAlive())
		{
			static float nextWaveDelay = 0.0f;
			nextWaveDelay += getDeltaTime();

			if (nextWaveDelay >= 3.0f)
			{
				waveManager.currentWave++;

				if (waveManager.currentWave <= 4)
				{
					startWave(&waveManager, waveManager.currentWave);
				}

				nextWaveDelay = 0.0f;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			if (Gamepad_isButtonPressed(i, OPTION) && timer > 0.2f)
			{
				togglePause();
				timer = 0.0f;
			}
		}
		if (sfKeyboard_isKeyPressed(sfKeyEscape) == sfTrue && timer > 0.2f)
		{
			togglePause();
			timer = 0.0f;
		}
		//stop ici, manette connect�e rep�r�e, et boutonPressed retourne la puissance de 2 correspondante au bouton
		for (int i = 0; i <= 3; i++)
		{
			if (isConnected(i))
			{
				timer = 0.0f;

			}
		}

		gamepadChangePos = sfFalse;


		BG1Pos.y = BG1Pos.y + 100.f * getDeltaTime();
		BG2Pos.y = BG2Pos.y + 100.f * getDeltaTime();
		if (BG1Pos.y >= 1080.0f)
			BG1Pos.y = BG2Pos.y - 1080.0f;
		if (BG2Pos.y >= 1080.0f)
			BG2Pos.y = BG1Pos.y - 1080.0f;

		sfSprite_setPosition(spBG1, BG1Pos);
		sfSprite_setPosition(spBG2, BG2Pos);
		updateEnnemy(_window->renderWindow);
		updatePlayer(_window->renderWindow);
		updateShot(_window->renderWindow);
		updateParticle(_window->renderWindow);
	}
}

void displayGame(Window* _window)
{
	sfRenderWindow_drawSprite(_window->renderWindow, spBG1, NULL);

	sfRenderWindow_drawSprite(_window->renderWindow, spBG2, NULL);
	DisplayEnnemy(_window->renderWindow, spEnnemy);
	DisplayPlayer(_window->renderWindow, spPlayer, spHitboxPlayer);
	DisplayShot(_window->renderWindow, spShot);
	DisplayParticle(_window->renderWindow, spParticle);


}
	
void deinitGame()
{
	deinitPause();

	sfSprite_destroy(spBG1);
	sfSprite_destroy(spBG2);
	sfSprite_destroy(spEnnemy);
	sfSprite_destroy(spPlayer);
	sfCircleShape_destroy(spShot);
	sfCircleShape_destroy(spParticle);
	sfCircleShape_destroy(spHitboxPlayer);
	RemoveAllTextureButALL();
	RemoveAllSoundButALL();
}