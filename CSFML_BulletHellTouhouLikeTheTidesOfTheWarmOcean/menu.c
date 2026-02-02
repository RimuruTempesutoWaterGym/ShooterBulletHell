#include "menu.h"
#include "textureManager.h"
#include "gamepad.h"
#include "viewManager.h"
#include "soundManager.h"
#include "fontManager.h"

sfFont* venture3D;

sfText* Play;
sfText* Credits;
sfText* Options;
sfText* Quit;
sfSprite* spBgMenu;

sfTexture* texture;
sfSprite* sprite;

int menuSelection;


void initMenu(Window* _window)
{
	Texture_Onload(MENU);
	Sound_Onload(MENU);
	Font_Onload(MENU);
	Play_Music("menu");

	venture3D = GetFont("big");

	SetViewPosition(mainView, vector2f(mainView->defaultVideoMode.x / 2.0f, mainView->defaultVideoMode.y / 2.0f));

	Play = sfText_create();
	Credits = sfText_create();
	Options = sfText_create();
	Quit = sfText_create();
	spBgMenu = sfSprite_create();
	sfText_setFont(Play, venture3D);
	sfText_setFont(Credits, venture3D);
	sfText_setFont(Options, venture3D);
	sfText_setFont(Quit, venture3D);
	sfText_setString(Play, "Play");
	sfText_setString(Credits, "Credits");
	sfText_setString(Options, "Settings");
	sfText_setString(Quit, "Quit");
	sfText_setCharacterSize(Play, 72);
	sfText_setCharacterSize(Credits, 72);
	sfText_setCharacterSize(Options, 72);
	sfText_setCharacterSize(Quit, 72);
	sfText_setPosition(Play, vector2f(mainView->PosView.x + 100.0f ,mainView->PosView.y - 100.0f));
	sfText_setPosition(Credits, vector2f(mainView->PosView.x + 200.0f, mainView->PosView.y - 0.0f));
	sfText_setPosition(Options, vector2f(mainView->PosView.x + 300.0f, mainView->PosView.y + 100.0f));
	sfText_setPosition(Quit, vector2f(mainView->PosView.x + 200.0f, mainView->PosView.y + 200.0f));
	sfSprite_setTexture(spBgMenu, GetTexture("touhoutmp"), sfTrue);
	sfSprite_setPosition(spBgMenu, vector2f(0.0f, -200.f));
	sfSprite_setScale(spBgMenu, vector2f(1.42f, 1.42f));


	GamepadDetection();
	menuSelection = 0;
}

void updateMenu(Window* _window)
{
	static float timer = 0.0f;
	timer += getDeltaTime();

	switch (menuSelection)
	{
	case 0:
		sfText_setColor(Play, sfBlue);
		sfText_setColor(Credits, sfWhite);
		sfText_setColor(Options, sfWhite);
		sfText_setColor(Quit, sfWhite);
		break;
	case 1:
		sfText_setColor(Play, sfWhite);
		sfText_setColor(Credits, sfBlue);
		sfText_setColor(Options, sfWhite);
		sfText_setColor(Quit, sfWhite);
		break;
	case 2:
		sfText_setColor(Play, sfWhite);
		sfText_setColor(Credits, sfWhite);
		sfText_setColor(Options, sfBlue);
		sfText_setColor(Quit, sfWhite);
		break;
	case 3:
		sfText_setColor(Play, sfWhite);
		sfText_setColor(Credits, sfWhite);
		sfText_setColor(Options, sfWhite);
		sfText_setColor(Quit, sfBlue);
		break;
	default:
		break;
	}

	for (int i = 0; i < 8; i++)
	{
		if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
		{
			switch (menuSelection)
			{
			case 0:
				changeState(_window, GAME);
				break;
			case 1:
				toggleCredits();
				break;
			case 2:
				toggleOptions();
				break;
			case 3:
				_window->isDone = sfTrue;
				break;
			default:
				break;
			}
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, CROSSY) > 0 && timer > 0.2f)
		{
			menuSelection--;
			if (menuSelection < 0)
				menuSelection = 3;
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, CROSSY) < 0 && timer > 0.2f)
		{
			menuSelection++;
			if (menuSelection > 3)
				menuSelection = 0;
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, STICKLY) < 0 && timer > 0.5f)
		{
			menuSelection--;
			if (menuSelection < 0)
				menuSelection = 3;
			timer = 0.0f;
		}
		if (Gamepad_isJoystickMoved(i, STICKLY) > 0 && timer > 0.5f)
		{
			menuSelection++;
			if (menuSelection > 3)
				menuSelection = 0;
			timer = 0.0f;
		}
	}

	if (sfKeyboard_isKeyPressed(sfKeyUp) == sfTrue && timer > 0.2f)
	{
		menuSelection--;
		if (menuSelection < 0)
			menuSelection = 3;
		timer = 0.0f;
	}
	if (sfKeyboard_isKeyPressed(sfKeyDown) == sfTrue && timer > 0.2f)
	{
		menuSelection++;
		if (menuSelection > 3)
			menuSelection = 0;
		timer = 0.0f;
	}
	if (sfKeyboard_isKeyPressed(sfKeyEnter) == sfTrue && timer > 0.2f)
	{
		Play_SFX("buttonClick");
		switch (menuSelection)
		{
		case 0:
			changeState(_window, GAME);
			break;
		case 1:
			toggleCredits();
			break;
		case 2:
			toggleOptions();
			break;
		case 3:
			_window->isDone = sfTrue;
			break;
		default:
			break;
		}
		timer = 0.0f;	
	}

}

void displayMenu(Window* _window)
{
	sfRenderWindow_drawSprite(_window->renderWindow, spBgMenu, NULL);
	sfRenderWindow_drawText(_window->renderWindow, Play, NULL);
	sfRenderWindow_drawText(_window->renderWindow, Credits, NULL);
	sfRenderWindow_drawText(_window->renderWindow, Options, NULL);
	sfRenderWindow_drawText(_window->renderWindow, Quit, NULL);
}

void deinitMenu()
{
	

	RemoveAllTextureButALL();
	RemoveAllSoundButALL();
	RemoveAllFontButALL();
	sfText_destroy(Play);
	sfText_destroy(Credits);
	sfText_destroy(Options);
	sfText_destroy(Quit);
}