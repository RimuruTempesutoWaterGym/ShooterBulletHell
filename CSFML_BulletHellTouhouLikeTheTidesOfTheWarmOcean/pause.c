#include "pause.h"
#include "stateManager.h"
#include "viewManager.h"
#include "gamepad.h"
#include "dialogBox.h"
#include "soundManager.h"

sfRectangleShape* rshape;

sfText* txtPause;
sfText* txtContinue;
sfText* txtReturnMenu;
sfText* txtSoundVolume;

sfTexture* texCursor;
sfSprite* spCursor;

int pauseSelect;

void initPause(Window* _window)
{
    rshape = sfRectangleShape_create();
    sfRectangleShape_setSize(rshape, (sfVector2f) { (float)mainView->defaultVideoMode.x, (float)mainView->defaultVideoMode.y });
    sfRectangleShape_setFillColor(rshape, (sfColor) { 0, 0, 0, 150 });

    txtPause = sfText_create();
    sfText_setFont(txtPause, getDefaultFont());
    sfText_setCharacterSize(txtPause, 60); // ADD THIS!
    sfText_setColor(txtPause, sfRed);
    sfText_setString(txtPause, "PAUSE");

    txtContinue = sfText_create();
    sfText_setFont(txtContinue, getDefaultFont());
    sfText_setCharacterSize(txtContinue, 20);
    sfText_setColor(txtContinue, sfWhite);
    sfText_setString(txtContinue, "Return to Game");

    txtReturnMenu = sfText_create();
    sfText_setFont(txtReturnMenu, getDefaultFont());
    sfText_setCharacterSize(txtReturnMenu, 20);
    sfText_setColor(txtReturnMenu, sfWhite);
    sfText_setString(txtReturnMenu, "Exit to Menu");

    txtSoundVolume = sfText_create();
    sfText_setFont(txtSoundVolume, getDefaultFont());
    sfText_setCharacterSize(txtSoundVolume, 20);
    sfText_setColor(txtSoundVolume, sfWhite);
    sfText_setString(txtSoundVolume, "Settings");

    texCursor = sfTexture_createFromFile("../Ressources/Textures/cursor.png", NULL);
    spCursor = sfSprite_create();
    sfSprite_setTexture(spCursor, texCursor, sfTrue);

    pauseSelect = 0;
}

void updatePause(Window* _window)
{
    static float timer = 0.0f;
    timer += getDeltaTime();

    for (int i = 0; i < 8; i++)
    {
        if (Gamepad_isButtonPressed(i, OPTION) && timer > 0.2f)
        {
            togglePause();
            timer = 0.0f;
        }

        if (Gamepad_isJoystickMoved(i, CROSSY) > 0 && timer > 0.2f)
        {
            pauseSelect--;
            if (pauseSelect < 0)
                pauseSelect = 2;
            timer = 0.0f;
        }
        if (Gamepad_isJoystickMoved(i, CROSSY) < 0 && timer > 0.2f)
        {
            pauseSelect++;
            if (pauseSelect > 2)
                pauseSelect = 0;
            timer = 0.0f;
        }
        if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
        {
            Play_SFX("buttonClick");

            if (pauseSelect == 0)
            {
                togglePause();
            }
            else if (pauseSelect == 1)
            {
                toggleOptions();
            }
            else if (pauseSelect == 2)
            {
                dialogBoxCreate(ALERT, "Do you want to go back ?\n", 1);
            }

            timer = 0.0f;
        }
    }

    if (sfKeyboard_isKeyPressed(sfKeyEscape) == sfTrue && timer > 0.2f)
    {
        togglePause();
        timer = 0.0f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyDown) == sfTrue && timer > 0.2f)
    {
        pauseSelect++;
        if (pauseSelect > 2)
            pauseSelect = 0;
        timer = 0.0f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyUp) == sfTrue && timer > 0.2f)
    {
        pauseSelect--;
        if (pauseSelect < 0)
            pauseSelect = 2;
        timer = 0.0f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyEnter) == sfTrue && timer > 0.2f)
    {
        Play_SFX("buttonClick");

        if (pauseSelect == 0)
        {
            togglePause();
        }
        else if (pauseSelect == 1)
        {
            toggleOptions();
        }
        else if (pauseSelect == 2)
        {
            dialogBoxCreate(ALERT, "Do you want to go back ?\n", 1);
        }

        timer = 0.0f;
    }

    // Update background position
    sfRectangleShape_setPosition(rshape, (sfVector2f) {
        mainView->PosView.x - mainView->defaultVideoMode.x / 2,
            mainView->PosView.y - mainView->defaultVideoMode.y / 2
    });

    // Base positions
    sfVector2f viewCenter = GetViewPosition(mainView);
    float baseX = viewCenter.x - 100.0f;
    float cursorOffset = 20.0f; // How much to shift text when selected

    // Title
    sfText_setPosition(txtPause, vector2f(viewCenter.x - 100.0f, viewCenter.y - 300.0f));

    // Menu items - set positions based on selection
    if (pauseSelect == 0)
    {
        sfText_setPosition(txtContinue, vector2f(baseX + cursorOffset, viewCenter.y - 200.0f));
        sfSprite_setPosition(spCursor, vector2f(baseX, viewCenter.y - 195.0f));
    }
    else
    {
        sfText_setPosition(txtContinue, vector2f(baseX, viewCenter.y - 200.0f));
    }

    if (pauseSelect == 1)
    {
        sfText_setPosition(txtSoundVolume, vector2f(baseX + cursorOffset, viewCenter.y - 150.0f));
        sfSprite_setPosition(spCursor, vector2f(baseX, viewCenter.y - 145.0f));
    }
    else
    {
        sfText_setPosition(txtSoundVolume, vector2f(baseX, viewCenter.y - 150.0f));
    }

    if (pauseSelect == 2)
    {
        sfText_setPosition(txtReturnMenu, vector2f(baseX + cursorOffset, viewCenter.y - 100.0f));
        sfSprite_setPosition(spCursor, vector2f(baseX, viewCenter.y - 95.0f));
    }
    else
    {
        sfText_setPosition(txtReturnMenu, vector2f(baseX, viewCenter.y - 100.0f));
    }
}

void displayPause(Window* _window)
{
	sfRenderWindow_setView(_window->renderWindow, mainView->view);
	sfRenderWindow_drawRectangleShape(_window->renderWindow, rshape, NULL);
	
	sfRenderWindow_drawSprite(_window->renderWindow, spCursor, NULL);
	
	sfRenderWindow_drawText(_window->renderWindow, txtPause, NULL);
	sfRenderWindow_drawText(_window->renderWindow, txtContinue, NULL);
	sfRenderWindow_drawText(_window->renderWindow, txtReturnMenu, NULL);
	sfRenderWindow_drawText(_window->renderWindow, txtSoundVolume, NULL);
}

void deinitPause()
{
	sfRectangleShape_destroy(rshape);
	sfText_destroy(txtPause);
	sfText_destroy(txtContinue);
	sfText_destroy(txtReturnMenu);
	sfText_destroy(txtSoundVolume);
	sfSprite_destroy(spCursor);
	sfTexture_destroy(texCursor);
}