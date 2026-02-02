	#pragma once

	#include "tools.h"

	typedef struct Score Score;
	struct Score
	{
	
		int score;
		time_t date;
	};
	void initHighScore();
	void saveHighScore(const char* filename);
	void loadHighScore(const char* filename);
	void changeHighScore(int _score);
	void displayHighScores(sfRenderWindow* window);