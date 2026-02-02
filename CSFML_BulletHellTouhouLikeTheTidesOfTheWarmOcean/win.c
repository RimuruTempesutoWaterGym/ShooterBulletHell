#include "win.h"
#include "stateManager.h"
#include "viewManager.h"
#include "gamepad.h"
#include "game.h"
#include "dialogBox.h"
#include "soundManager.h"

sfRectangleShape* rshapeWinInterface;

sfText* txtWin;
sfText* txtRetryAfterWin;
sfText* txtBackToMenuAfterWin;
sfTexture* texCursorWin;
sfSprite* spCursorWin;

int winSelect;

void initWin(Window* _window)
{
    rshapeWinInterface = sfRectangleShape_create();
    sfRectangleShape_setSize(rshapeWinInterface, (sfVector2f) { (float)mainView->defaultVideoMode.x, (float)mainView->defaultVideoMode.y });
    sfRectangleShape_setFillColor(rshapeWinInterface, (sfColor) { 0, 0, 0, 150 });

    txtWin = sfText_create();
    sfText_setFont(txtWin, getDefaultFont());
    sfText_setCharacterSize(txtWin, 60);
    sfText_setColor(txtWin, sfGreen);
    sfText_setString(txtWin, "VICTOIRE!!!");

    txtRetryAfterWin = sfText_create();
    sfText_setFont(txtRetryAfterWin, getDefaultFont());
    sfText_setCharacterSize(txtRetryAfterWin, 20);
    sfText_setColor(txtRetryAfterWin, sfWhite);
    sfText_setString(txtRetryAfterWin, "Retry");

    txtBackToMenuAfterWin = sfText_create();
    sfText_setFont(txtBackToMenuAfterWin, getDefaultFont());
    sfText_setCharacterSize(txtBackToMenuAfterWin, 20);
    sfText_setColor(txtBackToMenuAfterWin, sfWhite);
    sfText_setString(txtBackToMenuAfterWin, "Exit to Menu");

    texCursorWin = sfTexture_createFromFile("../Ressources/Textures/cursor.png", NULL);
    spCursorWin = sfSprite_create();
    sfSprite_setTexture(spCursorWin, texCursorWin, sfTrue);

    winSelect = 0;
}

void updateWin(Window* _window)
{
    static float timer = 0.0f;
    timer += getDeltaTime();

    for (int i = 0; i < 8; i++)
    {
        if (Gamepad_isJoystickMoved(i, CROSSY) > 0 && timer > 0.2f)
        {
            winSelect--;
            if (winSelect < 0)
                winSelect = 1;
            timer = 0.0f;
        }
        if (Gamepad_isJoystickMoved(i, CROSSY) < 0 && timer > 0.2f)
        {
            winSelect++;
            if (winSelect > 1)
                winSelect = 0;
            timer = 0.0f;
        }
        if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
        {
            Play_SFX("buttonClick");

            if (winSelect == 0)
            {
                refreshGame(_window);
                toggleDefeated();
            }
            else if (winSelect == 1)
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
        winSelect++;
        if (winSelect > 1)
            winSelect = 0;
        timer = 0.0f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyUp) == sfTrue && timer > 0.2f)
    {
        winSelect--;
        if (winSelect < 0)
            winSelect = 1;
        timer = 0.0f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyEnter) == sfTrue && timer > 0.2f)
    {
        Play_SFX("buttonClick");

        if (winSelect == 0)
        {
            toggleWin();
            refreshGame(_window);
        }
        else if (winSelect == 1)
        {
            dialogBoxCreate(ALERT, "Do you want to go back ?\n", 1);

        }

        timer = 0.0f;
    }


    sfRectangleShape_setPosition(rshapeWinInterface, (sfVector2f) {
        mainView->PosView.x - mainView->defaultVideoMode.x / 2,
            mainView->PosView.y - mainView->defaultVideoMode.y / 2
    });


    sfVector2f viewCenter = GetViewPosition(mainView);
    float baseX = viewCenter.x - 100.0f;
    float cursorOffset = 20.0f;

    sfText_setPosition(txtWin, vector2f(viewCenter.x - 100.0f, viewCenter.y - 300.0f));

    if (winSelect == 0)
    {
        sfText_setPosition(txtRetryAfterWin, vector2f(baseX + cursorOffset, viewCenter.y - 200.0f));
        sfSprite_setPosition(spCursorWin, vector2f(baseX, viewCenter.y - 195.0f));
    }
    else
    {
        sfText_setPosition(txtRetryAfterWin, vector2f(baseX, viewCenter.y - 200.0f));
    }

    if (winSelect == 1)
    {
        sfText_setPosition(txtBackToMenuAfterWin, vector2f(baseX + cursorOffset, viewCenter.y - 150.0f));
        sfSprite_setPosition(spCursorWin, vector2f(baseX, viewCenter.y - 145.0f));
    }
    else
    {
        sfText_setPosition(txtBackToMenuAfterWin, vector2f(baseX, viewCenter.y - 150.0f));
    }
}
void displayWin(Window* _window)
{
    sfRenderWindow_setView(_window->renderWindow, mainView->view);
    sfRenderWindow_drawRectangleShape(_window->renderWindow, rshapeWinInterface, NULL);

    sfRenderWindow_drawSprite(_window->renderWindow, spCursorWin, NULL);

    sfRenderWindow_drawText(_window->renderWindow, txtWin, NULL);
    sfRenderWindow_drawText(_window->renderWindow, txtRetryAfterWin, NULL);
    sfRenderWindow_drawText(_window->renderWindow, txtBackToMenuAfterWin, NULL);
}

void deinitWin()
{
    sfRectangleShape_destroy(rshapeWinInterface);
    sfText_destroy(txtWin);
    sfText_destroy(txtRetryAfterWin);
    sfText_destroy(txtBackToMenuAfterWin);
    sfSprite_destroy(spCursorWin);
    sfTexture_destroy(texCursorWin);
}
