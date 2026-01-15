#pragma once
#include "tools.h"
#include "ressourcesManager.h"

typedef struct Font Font;
struct Font
{
	Font* pNext;
	char name[30];
	ressourceType type;
	sfFont* sfFont;
	State state;
};

typedef enum
{
	FONTNULL = -1, FONTPATH, FONTCONFIG
}StateFont;

void Font_Onload(State _state);
void AddFont(Font* _font);
sfFont* GetFont(char* _name);
sfFont* getDefaultFont();
Font* RemoveFont(Font* _font);
void RemoveAllFont();
void RemoveAllFontButALL();
void SetFont(State _state);

sfFont* defaultFont;