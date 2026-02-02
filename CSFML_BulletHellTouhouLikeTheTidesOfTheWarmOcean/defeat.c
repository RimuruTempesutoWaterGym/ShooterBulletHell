#include "defeat.h"
#include "stateManager.h"
#include "viewManager.h"
#include "gamepad.h"
#include "game.h"
#include "dialogBox.h"
#include "soundManager.h"

sfRectangleShape* rshapeDefeatInterface;

sfText* txtDefeat;
sfText* txtRetry;
sfText* txtBackToMenu;
sfTexture* texCursorDefeat;
sfSprite* spCursorDefeat;

int defeatSelect;

void initDefeat(Window* _window)
{
    rshapeDefeatInterface = sfRectangleShape_create();
    sfRectangleShape_setSize(rshapeDefeatInterface, (sfVector2f) { (float)mainView->defaultVideoMode.x, (float)mainView->defaultVideoMode.y });
    sfRectangleShape_setFillColor(rshapeDefeatInterface, (sfColor) { 0, 0, 0, 150 });

    txtDefeat = sfText_create();
    sfText_setFont(txtDefeat, getDefaultFont());
    sfText_setCharacterSize(txtDefeat, 60); 
    sfText_setColor(txtDefeat, sfRed);
    sfText_setString(txtDefeat, "VAICU");

    txtRetry = sfText_create();
    sfText_setFont(txtRetry, getDefaultFont());
    sfText_setCharacterSize(txtRetry, 20);
    sfText_setColor(txtRetry, sfWhite);
    sfText_setString(txtRetry, "Retry");

    txtBackToMenu = sfText_create();
    sfText_setFont(txtBackToMenu, getDefaultFont());
    sfText_setCharacterSize(txtBackToMenu, 20);
    sfText_setColor(txtBackToMenu, sfWhite);
    sfText_setString(txtBackToMenu, "Exit to Menu");

    texCursorDefeat = sfTexture_createFromFile("../Ressources/Textures/cursor.png", NULL);
    spCursorDefeat = sfSprite_create();
    sfSprite_setTexture(spCursorDefeat, texCursorDefeat, sfTrue);

    defeatSelect = 0;
}

void updateDefeat(Window* _window)
{
    static float timer = 0.0f;
    timer += getDeltaTime();

    for (int i = 0; i < 8; i++)
    {
        if (Gamepad_isJoystickMoved(i, CROSSY) > 0 && timer > 0.2f)
        {
            defeatSelect--;
            if (defeatSelect < 0)
                defeatSelect = 1;
            timer = 0.0f;
        }
        if (Gamepad_isJoystickMoved(i, CROSSY) < 0 && timer > 0.2f)
        {
            defeatSelect++;
            if (defeatSelect > 1)
                defeatSelect = 0;
            timer = 0.0f;
        }
        if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
        {
            Play_SFX("buttonClick");

            if (defeatSelect == 0)
            {
                refreshGame(_window);
                toggleDefeated();
            }
            else if (defeatSelect == 1)
            {
                dialogBoxCreate(ALERT, "Do you want to go back ?\n", 1);
            }

            timer = 0.0f;
        }
    }

    if (sfKeyboard_isKeyPressed(sfKeyEscape) == sfTrue && timer > 0.2f)
    {
        dialogBoxCreate(ALERT, "Do you want to go back ?\n", 1);
        timer = 0.0f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyDown) == sfTrue && timer > 0.2f)
    {
        defeatSelect++;
        if (defeatSelect > 1)
            defeatSelect = 0;
        timer = 0.0f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyUp) == sfTrue && timer > 0.2f)
    {
        defeatSelect--;
        if (defeatSelect < 0)
            defeatSelect = 1;
        timer = 0.0f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyEnter) == sfTrue && timer > 0.2f)
    {
        Play_SFX("buttonClick");

        if (defeatSelect == 0)
        {
            toggleDefeated();
            refreshGame(_window);
        }
        else if (defeatSelect == 1)
        {
            dialogBoxCreate(ALERT, "Do you want to go back ?\n", 1);
        
        }

        timer = 0.0f;
    }

    // Update background position
    sfRectangleShape_setPosition(rshapeDefeatInterface, (sfVector2f) {
        mainView->PosView.x - mainView->defaultVideoMode.x / 2,
            mainView->PosView.y - mainView->defaultVideoMode.y / 2
    });

    // Base positions
    sfVector2f viewCenter = GetViewPosition(mainView);
    float baseX = viewCenter.x - 100.0f;
    float cursorOffset = 20.0f;

    // Title
    sfText_setPosition(txtDefeat, vector2f(viewCenter.x - 100.0f, viewCenter.y - 300.0f));

    // Menu items - set positions based on selection
    if (defeatSelect == 0)
    {
        sfText_setPosition(txtRetry, vector2f(baseX + cursorOffset, viewCenter.y - 200.0f));
        sfSprite_setPosition(spCursorDefeat, vector2f(baseX, viewCenter.y - 195.0f));
    }
    else
    {
        sfText_setPosition(txtRetry, vector2f(baseX, viewCenter.y - 200.0f));
    }

    if (defeatSelect == 1)
    {
        sfText_setPosition(txtBackToMenu, vector2f(baseX + cursorOffset, viewCenter.y - 150.0f));
        sfSprite_setPosition(spCursorDefeat, vector2f(baseX, viewCenter.y - 145.0f));
    }
    else
    {
        sfText_setPosition(txtBackToMenu, vector2f(baseX, viewCenter.y - 150.0f));
    }
}
void displayDefeat(Window* _window)
{
	sfRenderWindow_setView(_window->renderWindow, mainView->view);
	sfRenderWindow_drawRectangleShape(_window->renderWindow, rshapeDefeatInterface, NULL);

	sfRenderWindow_drawSprite(_window->renderWindow, spCursorDefeat, NULL);

	sfRenderWindow_drawText(_window->renderWindow, txtDefeat, NULL);
	sfRenderWindow_drawText(_window->renderWindow, txtRetry, NULL);
	sfRenderWindow_drawText(_window->renderWindow, txtBackToMenu, NULL);
}

void deinitDefeat()
{
	sfRectangleShape_destroy(rshapeDefeatInterface);
	sfText_destroy(txtDefeat);
	sfText_destroy(txtRetry);
	sfText_destroy(txtBackToMenu);
	sfSprite_destroy(spCursorDefeat);
	sfTexture_destroy(texCursorDefeat);
}
