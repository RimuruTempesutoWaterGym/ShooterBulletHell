#include "gamepadx.h"
#include "gamepad.h"

float timer = 0;

// This define makes your program compile faster by excluding things we are not using
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif

// Return true if a gamepad is connected
sfBool isConnected(t_joyNum joyNum)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    auto result = XInputGetState(joyNum, &state);
    return  (result == ERROR_SUCCESS);
}

// Return the number of max button in a xboxgamepad
unsigned int getButtonCount(t_joyNum joyNum)
{
    return 16;
}

// Return true if the specified button is triggered (check getTriggers method)
sfBool isButtonPressed(t_joyNum joyNum, t_buttonNum buttonNum)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    XInputGetState(joyNum, &state);
    return  (state.Gamepad.wButtons & buttonNum);
}

// Return true if a gamepad is connected at least
sfBool isAnyXBox360ControllerConnected()
{
    return  (isConnected(0) || isConnected(1) || isConnected(2) || isConnected(3));
}

// Return true if the gamepad have a audio support
sfBool voiceSupported(t_joyNum joyNum)
{
    XINPUT_CAPABILITIES caps;
    ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));

    auto result = XInputGetCapabilities(joyNum, XINPUT_FLAG_GAMEPAD, &caps);

    if (result != ERROR_SUCCESS)
        return sfFalse;

    return  (caps.Flags & XINPUT_CAPS_VOICE_SUPPORTED);
}

// Return the state of the trigger with their values (LT + RT)
// The value is between 0 and 1

void getTriggers(t_joyNum joyNum, float* left, float* right)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    XInputGetState(joyNum, &state);
   



    // Normalise
    if(XINPUT_GAMEPAD_TRIGGER_THRESHOLD/255< (float)(state.Gamepad.bLeftTrigger) / 255)
    *left = 1;
    else
    *left = 0;
    if (XINPUT_GAMEPAD_TRIGGER_THRESHOLD / 255 < (float)(state.Gamepad.bRightTrigger) / 255)
    *right = 1;
	else
    *right = 0;

}

// Elle modifie directement l'�tat des arguments left et right pass� avec la valeur des sticks
// The value vary between -100 and 100
void getSticksPosition(t_joyNum joyNum, sfVector2f* left, sfVector2f* right)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    XInputGetState(joyNum, &state);

    // Verifie la "DEAD ZONE"
    // Stick Gauche
    if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
        state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

        state.Gamepad.sThumbLX = 0;
        state.Gamepad.sThumbLY = 0;

    }

    // Stick Droit
    if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
        state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

        state.Gamepad.sThumbRX = 0;
        state.Gamepad.sThumbRY = 0;

    }

    // Converti les valeurs dans le style SFML (-100..100)
    left->x = (float)(state.Gamepad.sThumbLX / 327);
    left->y = (float)(state.Gamepad.sThumbLY / 327);
    right->x = (float)(state.Gamepad.sThumbRX / 327);
    right->y = (float)(state.Gamepad.sThumbRY / 327);
}

// Cette m�thode configure les vibrations de 0.0 � 1.0
// 0 arr�te les vibrations, 1 vibration au plus fort
void setVibration(t_joyNum joyNum, float leftMotor, float rightMotor)
{
    XINPUT_VIBRATION vib;
    ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));

    vib.wLeftMotorSpeed = (unsigned short)(leftMotor * 65535.0f);
    vib.wRightMotorSpeed = (unsigned short)(rightMotor * 65535.0f);

    XInputSetState(joyNum, &vib);
}
