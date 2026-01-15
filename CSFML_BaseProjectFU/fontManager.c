#include "fontManager.h"

Font* fontBegin = 0;

// Set config font depending the type in the ressources.load
void Font_Onload(State _state)
{
	ressources* tempRessources = ResBegin;
	while (tempRessources != NULL)
	{
		if (tempRessources->type == FONT)
		{
			if (tempRessources->state == _state)
			{
				Font* tempFont = (Font*)calloc(1, sizeof(Font));
				strcpy(tempFont->name, tempRessources->name);
				tempFont->type = tempRessources->type;
				tempFont->state = tempRessources->state;

				tempFont->sfFont = sfFont_createFromFile(tempRessources->path);
				AddFont(tempFont);
			
			}
		}
		tempRessources = tempRessources->pNext;

	}
}
// Get the default font
sfFont* getDefaultFont()
{
	return defaultFont;
}
// Set the right font with the same name
sfFont* GetFont(char* _name)
{
	Font* tempFont = fontBegin;
	while (tempFont != NULL)
	{
		if ((strcmp(_name, tempFont->name)) == 0)
		{
		
			return tempFont->sfFont;
		}
		tempFont = tempFont->pNext;
	}
	return 0;
}

void AddFont(Font* _font)
{
	_font->pNext = fontBegin;
	fontBegin = _font;
}

// Search the right font to remove in the chained list
Font* RemoveFont(Font* _font)
{
	if (_font == fontBegin)
	{
		Font* tempFont = _font->pNext;
		fontBegin = tempFont;
		free(_font);
		return tempFont;
	}
	else
	{
		Font* tempFont = fontBegin;
		while (tempFont->pNext != _font)
		{
			tempFont = tempFont->pNext;
		}
		tempFont->pNext = _font->pNext;
		free(_font);
		return tempFont->pNext;
	}
}

// Remove all fonts in the chained list
void RemoveAllFont()
{
	Font* tempFont = fontBegin;
	while (tempFont != NULL)
	{
		Font* toDelete = tempFont;
		tempFont = tempFont->pNext;
		free(toDelete);
	}
	fontBegin = 0;
}

// Remove all fonts in the chained list except those in [ALL]
void RemoveAllFontButALL()
{
	Font* tempFont = fontBegin;
	while (tempFont != NULL)
	{
		if (tempFont->state != ALL)
		{
			
			if (tempFont == fontBegin)
			{

				Font* tempFont2 = tempFont->pNext;
				fontBegin = tempFont2;
				
				sfFont_destroy(tempFont->sfFont);

				free(tempFont);
				tempFont = tempFont2;
			}
			else
			{
				Font* toDelete = tempFont;
				tempFont = tempFont->pNext;
				sfFont_destroy(toDelete->sfFont);
				free(toDelete);
			}
		}
		else
		{
			tempFont = tempFont->pNext;
		}
	}
}

//void SetFont(State _state, char* _name)
//{
//	Font* tempFont = fontBegin;
//	while (tempFont != NULL)
//	{
//		printf("%s", tempFont->name);
//		if (tempFont->state == _state)
//		{
//			if (tempFont->type == FONT && !strcmp(tempFont->name, _name))
//			{
//
//			}
//		}
//	}
//}