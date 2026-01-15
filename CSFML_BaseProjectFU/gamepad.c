#include "Gamepad.h"

#define MAX_PLAYER 1


// Assign a gamepad to a player
void GamepadDetection()
{
	sfJoystick_update();
	int playernber = 0;

	for (unsigned int i = 0; i < 8; i++)
	{
		if (playernber >= MAX_PLAYER) break;
		if (sfJoystick_isConnected(i) == sfTrue)
		{
			sfJoystickIdentification joyid = sfJoystick_getIdentification(i);
			if (joyid.productId == 654 && joyid.vendorId == 1118)
			{
				//player[playernber].idGamepad = i;
				playernber++;
			}
		}
		else
		{
			//player[playernber].idGamepad = -1;
		}
	}
}

// Collect the position of the joystick and test if he's pressed
void GamepadManager(int _joystickId, sfEvent event)
{
	if (event.type == sfEvtJoystickConnected)
	{
		printf("Gamepad is Connected id %d\n", event.joystickConnect.joystickId);
		GamepadDetection();
	}
	if (event.type == sfEvtJoystickDisconnected)
	{

			printf("Gamepad is Disconnected id %d\n", event.joystickConnect.joystickId);
			isDisconnected = sfTrue;
		
	}

	if (event.joystickButton.joystickId == _joystickId)
	{
		if (event.joystickMove.axis == sfJoystickX) // StickL 1 X
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].stickL.x = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].stickL.x = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickY) // StickL 1 Y
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].stickL.y = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].stickL.y = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickU) // StickR 2 X
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].stickR.x = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].stickR.x = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickV) // StickR 2 Y
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].stickR.y = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].stickR.y = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickPovX) // Cross X
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].cross.x = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].cross.x = 0.0f;
			}
		}
		if (event.joystickMove.axis == sfJoystickPovY) // Cross Y
		{
			if (event.joystickMove.position > 10.0f || event.joystickMove.position < -10.0f)
			{
				Gamepad[_joystickId].cross.y = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].cross.y = 0.0f;
			}
		}

		if (event.joystickMove.axis == sfJoystickZ) // L2 et R2
		{
			if (event.joystickMove.position > 1.0f)
			{
				Gamepad[_joystickId].TriggerL = event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].TriggerL = 0.0f;
			}
			if (event.joystickMove.position < -1.0f)
			{
				Gamepad[_joystickId].TriggerR = -event.joystickMove.position;
			}
			else
			{
				Gamepad[_joystickId].TriggerR = 0.0f;
			}
		}

		if (event.type == sfEvtJoystickButtonPressed)
		{

			for (int i = 0; i < 14; i++)
			{
				if (event.joystickButton.button == i)
				{
					Gamepad[_joystickId].Bouton[i] = 1;
				}
			}
		}

		if (event.type == sfEvtJoystickButtonReleased)
		{
			for (int i = 0; i < 14; i++)
			{
				if (event.joystickButton.button == i)
				{
					Gamepad[_joystickId].Bouton[i] = 0;
				}
			}
		}

	}
}

// Get the int of the button pressed
int Gamepad_isButtonPressed(unsigned int _joystickId, gamepadPS4 _button)
{
	return Gamepad[_joystickId].Bouton[_button];
}

// Return the position of the joystick
float Gamepad_isJoystickMoved(unsigned int _joystickId, gamepadPS4 _Axis)
{
	switch (_Axis)
	{
	case STICKLX:
		return Gamepad[_joystickId].stickL.x;
		break;
	case STICKLY:
		return Gamepad[_joystickId].stickL.y;
		break;
	case CROSSX:
		return Gamepad[_joystickId].cross.x;
		break;
	case CROSSY:
		return Gamepad[_joystickId].cross.y;
		break;
	case STICKRX:
		return Gamepad[_joystickId].stickR.x;
		break;
	case STICKRY:
		return Gamepad[_joystickId].stickR.y;
		break;
	case TRIGGER_R2:
		return Gamepad[_joystickId].TriggerR;
		break;
	case TRIGGER_L2:
		return Gamepad[_joystickId].TriggerL;
		break;
	default:
		return 0.0f;
		break;
	}
}