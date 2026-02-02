#include "stateManager.h"
#include "ressourcesManager.h"
#include "textureManager.h"
#include "soundManager.h"
#include "pause.h"
#include "defeat.h"
#include "credits.h"
#include "win.h"
#include "menu.h"
#include "game.h"
#include "dialogBox.h"
#include "options.h"
#include "viewManager.h"
#include "fontManager.h"
#include "particleManager.h"
#include "scoreManager.h"

// Init all resources and init other file depending the state
void stateInit(Window* _window)
{
	static sfBool firstload = sfFalse;
	if (!firstload)
	{
		OnLoad("../Ressources/ressources.load");
		Texture_Onload(ALL);
		Sound_Onload(ALL);
		Font_Onload(ALL);
		spLoading = sfSprite_create();
		sfSprite_setTexture(spLoading, GetTexture("loading"), sfTrue);
		sfIntRect AnimRect = { 0, 0, 128, 128 };
		sfSprite_setOrigin(spLoading, vector2f(64.0f, 64.0f));
		sfSprite_setPosition(spLoading, vector2f(mainView->PosView.x, mainView->PosView.y));
		sfSprite_setTextureRect(spLoading, AnimRect);

		SFXVolume = 50.0f;
		musicVolume = 50.0f;
		w.state = sfTrue;
		defaultFont = GetFont("zelda");

		firstload = sfTrue;
	}
	if (!onePass)
	{
		isDialogBox = sfFalse;
		onePass = sfTrue;
		if (state == INTRO)
		{
			
		}
		if (state == MENU)
		{

			initMenu(_window);
			initOptions(_window);
			initCredits(_window);
		}
		if (state == GAME)
		{
			w.mutex = sfMutex_create();
			w.state = sfFalse;
			loadingThread = sfThread_create(&initGame, _window);
			sfThread_launch(loadingThread);
			initPause(_window);
			initDefeat(_window);
			initWin(_window);
			initOptions(_window);
		
		}
		if (state == END)
		{
			
		}
	}
}

void stateEventUpdate(Window* _window, sfEvent* _event)
{
	if (w.state)
	{
			if (state == INTRO)
			{

			}
			else if (state == MENU)
			{
				
			}
			else if (state == GAME)
			{
				
			}
			else if (state == END)
			{

			}
		
	}
}

// Update file depending the state and config a "loading screen" if w.state is false
void stateUpdate(Window* _window)
{
	if (w.state)
	{
		if (!isDialogBox)
		{
			if (state == INTRO)
			{

			}
			else if (state == MENU)
			{
				if (isOption)
					updateOptions(_window);
				else if (isCredits)
				{
					updateCredits(_window);
				}
					updateMenu(_window);
			}
			else if (state == GAME)
			{
				if (isPaused)
				{
					if (isOption)
						updateOptions(_window);
					else
					updatePause(_window);	
				}	
				else if (isDefeated)
				{
						updateDefeat(_window);
				}
				else if (isWon)
				{
					updateWin(_window);
				}
				else
					updateGame(_window);
			}
			else if (state == END)
			{

			}
		}
		else
		{
			dialogBoxUpdate(_window);
		}
	}
	else if (!w.state)
	{
		static sfIntRect AnimRect = { 0, 0, 128, 128 };
		static int frame = 0;
		static float timer = 0.0f;
		timer += getDeltaTime();

		sfMutex_lock(w.mutex);
		sfSprite_setOrigin(spLoading, vector2f(64.0f, 64.0f));
		sfSprite_setPosition(spLoading, vector2f(mainView->PosView.x, mainView->PosView.y));
		sfSprite_setTextureRect(spLoading, AnimRect);
		if (timer > 0.1f)
		{
			frame++;
			if (frame > 8)
				frame = 0;
			AnimRect.left = frame * 128;
			
			timer = 0.0f;
		}
		sfMutex_unlock(w.mutex);
	}
}

// Display file depending the state and display the "loading screen" if w.state is false
void stateDisplay(Window* _window)
{
	if (w.state)
	{
		if (state == INTRO)
		{

		}
		if (state == MENU)
		{
			displayMenu(_window);
			if (isOption)
			{
				displayOptions(_window);
			}
			if (isCredits)
			{
				displayCredits(_window);
			}
			if (isDialogBox)
			{
				dialogBoxDisplay(_window);
			}
		}
		if (state == GAME)
		{
			displayGame(_window);
			if (isPaused)
			{
				displayPause(_window);
				if (isOption)
				{
					displayOptions(_window);
				}
			}
			else if (isDefeated)
			{
				displayDefeat(_window);
				displayHighScores(_window->renderWindow);
			}
			else if (isWon)
			{
				displayWin(_window);
				displayHighScores(_window->renderWindow);
			}
			if (isDialogBox)
			{
				dialogBoxDisplay(_window);
			}
		}
		if (state == END)
		{

		}
	}
	else if (!w.state)
	{
		sfMutex_lock(w.mutex);
			
		sfRenderWindow_drawSprite(_window->renderWindow, spLoading, NULL);
		
		sfMutex_unlock(w.mutex);
	}
}


void stateDeinit(Window* _window)
{
	if (state == INTRO)
	{

	}
	if (state == MENU)
	{
		deinitMenu();
		deinitOptions();
		deinitCredits();
	}
	if (state == GAME)
	{
		deinitGame();
		deinitOptions();
		deinitPause();
		deinitDefeat();
		deinitWin();
	}
	if (state == END)
	{

	}
}


void changeState(Window* _window, State _state)
{
	stateDeinit(_window);
	state = _state;
	onePass = sfFalse;
	stateInit(_window);
}


void togglePause()
{
	if (!isDefeated || !isWon)
	{
		isPaused = !isPaused;
	}
}
void toggleDefeated()
{
	isDefeated = !isDefeated;
}
void toggleWin()
{
	isWon = !isWon;
}

void toggleOptions()
{
	isOption = !isOption;
}
sfBool getDefeatedStatus()
{
	return isDefeated;
}
sfBool getWinStatus()
{
	return isWon;
}

void toggleCredits()
{
	isCredits = !isCredits;
}
sfBool getCreditsStatus()
{
	return isCredits;
}