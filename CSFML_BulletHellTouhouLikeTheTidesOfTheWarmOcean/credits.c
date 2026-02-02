#include "defeat.h"
#include "stateManager.h"
#include "viewManager.h"
#include "gamepad.h"
#include "game.h"
#include "dialogBox.h"
#include "soundManager.h"

sfRectangleShape* rshapeCreditsInterface;

sfText* txtCredits;
sfText* txtNames;
sfText* txtReturnBack;
sfTexture* texCursorCredits;
sfSprite* spCursorCredits;



void initCredits(Window* _window)
{
    rshapeCreditsInterface = sfRectangleShape_create();
    sfRectangleShape_setSize(rshapeCreditsInterface, (sfVector2f) { (float)mainView->defaultVideoMode.x, (float)mainView->defaultVideoMode.y });
    sfRectangleShape_setFillColor(rshapeCreditsInterface, (sfColor) { 0, 0, 0, 255 });

    txtCredits = sfText_create();
    sfText_setFont(txtCredits, getDefaultFont());
    sfText_setCharacterSize(txtCredits, 60);
    sfText_setColor(txtCredits, sfRed);
    sfText_setString(txtCredits, "CREDITS");

    txtNames = sfText_create();
    sfText_setFont(txtNames, getDefaultFont());
    sfText_setCharacterSize(txtNames, 40);
    sfText_setColor(txtNames, sfWhite);
    sfText_setString(txtNames,
        "DEV : FERREIRO JESSY\n"
        "ARTISTE BOSSES : FERREIRO JESSY\n"
        "COMPOSITEUR : ZUN (Team Shanghai Alice)\n"
        "REMIXEUR : TEA_BASIRA\n"
        "SPRITES BALLES: ZUN(Team Shanghai Alice)\n"
        "SPRITES HUD: ZUN(Team Shanghai Alice)\n"
        "FOND DU MENU: ZUN(Team Shanghai Alice)\n"
        "SFX: ZUN(Team Shanghai Alice)\n"
        "SPRITES MINIONS : REALE-LY\n"
        "FOND DE L'HUD : ROBOXEL\n"
        "D'APRES L'OEUVRE TOUHOU DE : ZUN\n"
    );

    txtReturnBack = sfText_create();
    sfText_setFont(txtReturnBack, getDefaultFont());
    sfText_setCharacterSize(txtReturnBack, 20);
    sfText_setColor(txtReturnBack, sfWhite);
    sfText_setString(txtReturnBack, "Exit to Menu");

    texCursorCredits = sfTexture_createFromFile("../Ressources/Textures/cursor.png", NULL);
    spCursorCredits = sfSprite_create();
    sfSprite_setTexture(spCursorCredits, texCursorCredits, sfTrue);

 
}

void updateCredits(Window* _window)
{
    static float timer = 0.0f;
    timer += getDeltaTime();

    for (int i = 0; i < 8; i++)
    {

        if (Gamepad_isButtonPressed(i, CROIX) && timer > 0.2f)
        {
            Play_SFX("buttonClick");
            toggleCredits();
            timer = 0.0f;
        }
    }


    if (sfKeyboard_isKeyPressed(sfKeyEnter) == sfTrue && timer > 0.2f)
    {
        Play_SFX("buttonClick");
        toggleCredits();
        timer = 0.0f;
    }

    // Update background position
    sfRectangleShape_setPosition(rshapeCreditsInterface, (sfVector2f) {
        mainView->PosView.x - mainView->defaultVideoMode.x / 2,
            mainView->PosView.y - mainView->defaultVideoMode.y / 2
    });

    // Base positions
    sfVector2f viewCenter = GetViewPosition(mainView);
    float baseX = viewCenter.x - 100.0f;
    float cursorOffset = 20.0f;

    // Title
    sfText_setPosition(txtCredits, vector2f(viewCenter.x - 100.0f, viewCenter.y - 400.0f));

    // Menu items - set positions based on selection
    sfText_setPosition(txtNames, vector2f(300, viewCenter.y - 300.0f));
    sfText_setPosition(txtReturnBack, vector2f(baseX + cursorOffset, 900));
    sfSprite_setPosition(spCursorCredits, vector2f(baseX, 900));

}
void displayCredits(Window* _window)
{
    sfRenderWindow_setView(_window->renderWindow, mainView->view);
    sfRenderWindow_drawRectangleShape(_window->renderWindow, rshapeCreditsInterface, NULL);

    sfRenderWindow_drawSprite(_window->renderWindow, spCursorCredits, NULL);

    sfRenderWindow_drawText(_window->renderWindow, txtCredits, NULL);
    sfRenderWindow_drawText(_window->renderWindow, txtNames, NULL);
    sfRenderWindow_drawText(_window->renderWindow, txtReturnBack, NULL);
}

void deinitCredits()
{
    sfRectangleShape_destroy(rshapeCreditsInterface);
    sfText_destroy(txtCredits);
    sfText_destroy(txtNames);
    sfText_destroy(txtReturnBack);
    sfSprite_destroy(spCursorCredits);
    sfTexture_destroy(texCursorCredits);
}
