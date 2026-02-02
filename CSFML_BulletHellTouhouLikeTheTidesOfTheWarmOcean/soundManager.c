#include "soundManager.h"

Sound* soundBegin = 0;
Sound* playedSoundBegin = 0;
sfMusic* newMusic = NULL;
sfMusic* oldMusic = NULL;
float soundFXDelay = 0;


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
				strcpy(tempSound->bufferPath, tempRessources->path);
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
char* GetSoundBufferPath(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == SOUNDFX)
			{
				return tempSound->bufferPath;
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
	oldMusic = NULL;
	playedSound* tempPlayedSound = playedSoundBegin;
	while (tempPlayedSound != NULL)
	{
		playedSound* toDelete = tempPlayedSound;
		tempPlayedSound = tempPlayedSound->pNext;
		free(toDelete);
	}
	playedSoundBegin = 0;

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
				else if (tempSound->type == MUSIC)
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
				else if (tempSound->type == MUSIC)
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
void Play_Music(char* _name)
{
	newMusic = GetMusic(_name);
	if (oldMusic != NULL)
	{
		sfMusic_stop(oldMusic);
	}
	sfMusic_play(newMusic);
	sfMusic_setLoop(newMusic, sfTrue);
	sfMusic_setVolume(newMusic, musicVolume);
	oldMusic = newMusic;
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

void Play_SFX(char* _name)
{

	if (!strcmp(_name, "laser") && soundFXDelay > 0.1f )
	{
		playedSound* newSound = malloc(sizeof(playedSound));
		newSound->sound = sfSound_create();
		newSound->buffer = sfSoundBuffer_createFromFile(GetSoundBufferPath(_name));
		sfSound_setBuffer(newSound->sound, newSound->buffer);
		sfSound_setVolume(newSound->sound, SFXVolume);

		sfSound_play(newSound->sound);
		newSound->hasPlayed = 1;

		newSound->pNext = playedSoundBegin;
		playedSoundBegin = newSound;
		soundFXDelay = 0;
	}
	else if (strcmp(_name, "laser") )
	{
		playedSound* newSound = malloc(sizeof(playedSound));
		newSound->sound = sfSound_create();
		newSound->buffer = sfSoundBuffer_createFromFile(GetSoundBufferPath(_name));
		sfSound_setBuffer(newSound->sound, newSound->buffer);
		sfSound_setVolume(newSound->sound, SFXVolume);

		sfSound_play(newSound->sound);
		newSound->hasPlayed = 1;

		newSound->pNext = playedSoundBegin;
		playedSoundBegin = newSound;
		}
}
void Update_SFX_Cleanup()
{
	soundFXDelay += getDeltaTime();
	
	playedSound* current = playedSoundBegin;
	playedSound* prev = NULL;

	while (current != NULL)
	{
		if (sfSound_getStatus(current->sound) == sfStopped)
		{

			playedSound* toDelete = current;
			if (prev == NULL) playedSoundBegin = current->pNext;
			else prev->pNext = current->pNext;

			current = current->pNext;
			sfSound_destroy(toDelete->sound);
			sfSoundBuffer_destroy(toDelete->buffer);
			free(toDelete);
		}
		else
		{
			prev = current;
			current = current->pNext;
		}
	}
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