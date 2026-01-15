#include "soundManager.h"

Sound* soundBegin = 0;

// Set config texture depending the type  
void Sound_Onload(State _state)
{
	ressources* tempRessources = ResBegin;
	while (tempRessources != NULL)
	{
		if (tempRessources->type == MUSIC)
		{
			if (tempRessources->state == _state)
			{
				Sound* tempSound = (Sound*)calloc(1, sizeof(Sound));
				strcpy(tempSound->name, tempRessources->name);
				tempSound->type = tempRessources->type;
				tempSound->state = tempRessources->state;

				tempSound->music = sfMusic_createFromFile(tempRessources->path);
				tempSound->compteur = 0;
				AddSound(tempSound);
			}
		}
		if (tempRessources->type == SOUNDFX)
		{
			if (tempRessources->state == _state)
			{
				//Cr�er et initialise une texture temporaire
				Sound* tempSound = (Sound*)calloc(1, sizeof(Sound));
				strcpy(tempSound->name, tempRessources->name);
				tempSound->type = tempRessources->type;
				tempSound->state = tempRessources->state;

				tempSound->sound = sfSound_create();
				tempSound->sfBuffer = sfSoundBuffer_createFromFile(tempRessources->path);
				sfSound_setBuffer(tempSound->sound, tempSound->sfBuffer);
				tempSound->compteur = 0;
				//Ajoute la texture a la liste
	
				AddSound(tempSound);
			}
		}
		tempRessources = tempRessources->pNext;
	}
}

// Set the right texture with the same name
sfMusic* GetMusic(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == MUSIC)
			{
				return tempSound->music;
			}
		}
		tempSound = tempSound->pNext;
	}
	return NULL;
}

// Set the right texture with the same name
sfSound* GetSound(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == SOUNDFX)
			{
				return tempSound->sound;
			}
		}
		tempSound = tempSound->pNext;
	}
	return NULL;
}

void AddSound(Sound* _sound)
{
	_sound->pNext = soundBegin;
	soundBegin = _sound;
}

// Search the right texture to remove in the chained list
Sound* RemoveSound(Sound* _sound)
{
	if (_sound == soundBegin)
	{
		Sound* tempSound = _sound->pNext;
		soundBegin = tempSound;
		free(_sound);
		return tempSound;
	}
	else
	{
		Sound* tempSound = soundBegin;
		while (tempSound->pNext != _sound)
		{
			tempSound = tempSound->pNext;
		}
		tempSound->pNext = _sound->pNext;
		free(tempSound);
		return tempSound->pNext;
	}
}

// Remove all textures in the chained list
void RemoveAllSound()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		Sound* todelete = tempSound;
		tempSound = tempSound->pNext;
		free(todelete);
	}
	soundBegin = 0;
}

// Remove all textures in the chained list except the essential
void RemoveAllSoundButALL()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if (tempSound->state != ALL)
		{
			if (tempSound == soundBegin)
			{
		
				Sound* tempSound2 = tempSound->pNext;
				soundBegin = tempSound2;
				if (tempSound->type == SOUNDFX)
				{
					sfSound_destroy(tempSound->sound);
				}
				else
				{
					sfMusic_destroy(tempSound->music);
				}
				free(tempSound);
				tempSound = tempSound2;
			}
			else
			{
				Sound* toDelete = tempSound;
				tempSound = tempSound->pNext;
				if (tempSound->type == SOUNDFX)
				{
					sfSound_destroy(toDelete->sound);
				}
				else
				{
					sfMusic_destroy(toDelete->music);
				}

				free(toDelete);
			}
		}
		else
		{
			tempSound = tempSound->pNext;
		}
	}
}
void Play_Music(State _state)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if (tempSound->state == _state)
		{
			if (tempSound->type == MUSIC)
			{
				sfMusic_stop(tempSound->music);
				sfMusic_play(tempSound->music);
				sfMusic_setLoop(tempSound->music, sfTrue);
				sfMusic_setVolume(tempSound->music, musicVolume);
				return tempSound->sound;
			}
		}

		tempSound = tempSound->pNext;
	}

	return NULL;
}

void SetVolumeMusic()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
	
		if (tempSound->type == MUSIC)
		{
			sfMusic_setVolume(tempSound->music, musicVolume);
			return tempSound->sound;
		}

		tempSound = tempSound->pNext;
	}

	return NULL;
}

void Play_SFX(char* _name, State _state)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if (tempSound->state == _state)
		{

			if (tempSound->type == SOUNDFX && !strcmp(tempSound->name,_name))
			{
				sfSound_stop(tempSound->sound);
				sfSound_play(tempSound->sound);
				sfSound_setVolume(tempSound->sound, SFXVolume);
				return tempSound->sound;
			}
		}

		tempSound = tempSound->pNext;
	}

	return NULL;
}

void SetVolumeSFX()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if (tempSound->type == SOUNDFX)
		{
			sfMusic_setVolume(tempSound->music, SFXVolume);
			return tempSound->sound;
		}

		tempSound = tempSound->pNext;
	}

	return NULL;
}