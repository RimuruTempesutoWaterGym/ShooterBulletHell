#pragma once
#include "tools.h"
#include "SFML/Audio.h"
#include "ressourcesManager.h"

typedef struct Sound Sound;
struct Sound
{
	Sound* pNext;
	char name[30];
	ressourceType type;
	State state;
	sfSoundBuffer* sfBuffer;
	int compteur;
	union MyUnion
	{
		sfMusic* music;
		sfSound* sound;
	
	};

};

typedef enum
{
	SOUNDNULL = -1, SOUNDPATH, SOUNDCONFIG
}StateSound;

float SFXVolume;
float musicVolume;



void Sound_Onload(State _state);
sfSound* GetSound(char* _name);
void Play_Music(State _state);
void Play_SFX(char* _name, State _state);
void SetVolumeMusic();
void SetVolumeSFX();
void AddSound(Sound* _sound);
Sound* RemoveSound(Sound* _sound);
void RemoveAllSound();
void RemoveAllSoundButALL();