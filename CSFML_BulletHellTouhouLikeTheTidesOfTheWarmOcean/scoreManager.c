#include "scoreManager.h"
#include "fontManager.h"
Score HighScore[10];
int lastScoreIndex = -1;
void initHighScore()
{

	loadHighScore("scores/Highscore.dat");
}
void changeHighScore(int _score)
{


	Score newScore = { _score ,time(NULL) };


	lastScoreIndex = -1;
	for (int i = 0; i < 10; i++)
	{

		if (newScore.score > HighScore[i].score)
		{
			for (int j = 9; j > i; j--)
			{
		
				HighScore[j] = HighScore[j - 1];
			}
	
			HighScore[i] = newScore;
			lastScoreIndex = i;
			saveHighScore("scores/Highscore.dat");

			break;
		}

	}
}
void saveHighScore(const char* filename)
{

	FILE* file = fopen(filename, "wb");
	if (file == NULL)
	{
		printf("Erreur: Impossible de créer le fichier %s\n", filename);
		return;
	}

	int nbScore = 10;

	fwrite(&nbScore, sizeof(Score), 1, file);
	fwrite(HighScore, sizeof(Score), nbScore, file);
	fclose(file);
	printf("HighScore sauvegardée dans %s\n", filename);
}
void createHighScore()
{
	for (int i = 0; i < 10; i++)
	{

			HighScore[i].score = 0;
			HighScore[i].date = 0;
		
		
	}
}
void loadHighScore(const char* filename)
{
	FILE* file = fopen(filename, "rb");

	if (file == NULL)
	{
		printf("Aucune sauvegarde trouvée. Création de %s...\n", filename);
		createHighScore();
		saveHighScore(filename);
		return;
	}

	int nbScore;
	fread(&nbScore, sizeof(Score), 1, file);

	if (nbScore != 10)
	{

		fclose(file);
		return;
	}

	fread(HighScore, sizeof(Score), nbScore , file);
	fclose(file);
	printf("HighScore chargée depuis %s\n", filename);
}
void displayHighScores(sfRenderWindow* window)
{
	static float highlightTimer = 0.0f;
	highlightTimer += getDeltaTime();

	sfText* txtScore = sfText_create();
	sfText_setFont(txtScore, getDefaultFont());
	sfText_setCharacterSize(txtScore, 20);

	char buffer[50];
	char dateStr[100];

	for (int i = 0; i < 10; i++)
	{
		if (HighScore[i].score > 0)
		{
			struct tm* timeInfo = localtime(&HighScore[i].date);
			strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M", timeInfo);

			sprintf(buffer, "%d. %d - %s", i + 1, HighScore[i].score, dateStr);

			sfText_setString(txtScore, buffer);

			
			if (i == lastScoreIndex)
			{
	
				float pulse = (sin(highlightTimer * 5.0f) + 1.0f) / 2.0f; 
				sfUint8 r = (sfUint8)(255);
				sfUint8 g = (sfUint8)(255);
				sfUint8 b = (sfUint8)(255 * (1.0f - pulse * 0.5f));  

				sfText_setColor(txtScore, (sfColor) { r, g, b, 255 });
				sfText_setStyle(txtScore, sfTextBold);
			}
			else
			{
				sfText_setColor(txtScore, sfWhite);
				sfText_setStyle(txtScore, sfTextRegular);
			}

			sfText_setPosition(txtScore, (sfVector2f) { 100, 100 + (i * 30) });
			sfRenderWindow_drawText(window, txtScore, NULL);
		}
	}

	sfText_destroy(txtScore);
}