#include "tools.h"
#include "fontManager.h"

sfTime sftime;
sfClock* sfclock;

// Set the timer and the default font
void initTools()
{
	sfclock = sfClock_create();
	srand(time(NULL));
}



// Create a vector2f
sfVector2f vector2f(float _x, float _y)
{
	sfVector2f tmp = { _x, _y };
	return tmp;
}

// Create a vector2i
sfVector2i vector2i(int _x, int _y)
{
	sfVector2i tmp = { _x, _y };
	return tmp;
}

// Create a vector3f
sfVector3f vector3f(float _x, float _y, float _z)
{
	sfVector3f tmp = { _x, _y, _z };
	return tmp;
}

// Create a float rect
sfFloatRect FlRect(float _left, float _top, float _width, float _height)
{
	sfFloatRect rectTmp = { _left, _top,  _width, _height };
	return rectTmp;
}

// Create a int rect
sfIntRect IntRect(int _left, int _top, int _width, int _height)
{
	sfIntRect rectTmp = { _left, _top,  _width, _height };
	return rectTmp;
}

// Create a color
sfColor color(sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a)
{
	return sfColor_fromRGBA(r, g, b, a);
}

// Create lerp fonction
float lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}


sfVector2f vec2f_lerp(sfVector2f v0, sfVector2f v1, float t)
{
	sfVector2f tmp;
	tmp.x = (1 - t) * v0.x + t * v1.x; // Replace by lerp
	tmp.y = (1 - t) * v0.y + t * v1.y; // Replace by lerp
	return tmp;
}


sfVector3f vec3f_lerp(sfVector3f v0, sfVector3f v1, float t)
{
	sfVector3f tmp;
	tmp.x = (1 - t) * v0.x + t * v1.x;// Replace by lerp
	tmp.y = (1 - t) * v0.y + t * v1.y;// Replace by lerp
	tmp.z = (1 - t) * v0.z + t * v1.z;// Replace by lerp
	return tmp;
}


vector4f vec4f_lerp(vector4f v0, vector4f v1, float t)
{
	vector4f tmp;
	tmp.r = (1 - t) * v0.r + t * v1.r;// Replace by lerp
	tmp.g = (1 - t) * v0.g + t * v1.g;
	tmp.b = (1 - t) * v0.b + t * v1.b;
	tmp.a = (1 - t) * v0.a + t * v1.a;
	return tmp;
}


sfColor color_lerp(sfColor v0, sfColor v1, float t)
{
	sfColor tmp;
	tmp.r = (1 - t) * v0.r + t * v1.r;// Replace by lerp
	tmp.g = (1 - t) * v0.g + t * v1.g;
	tmp.b = (1 - t) * v0.b + t * v1.b;
	tmp.a = (1 - t) * v0.a + t * v1.a;
	return tmp;
}

// Restart timer
void restartClock()
{
	sftime = sfClock_restart(sfclock);
}

// Get Delta Time
float getDeltaTime()
{
	float tmp = sfTime_asSeconds(sftime);
	if (tmp > 0.1) tmp = 0.0f;
	return tmp;
}

// Set a sprite with his texture
sfSprite* CreateSprite(char* _path)
{
	sfTexture* tmp = sfTexture_createFromFile(_path, NULL);
	sfSprite* spTmp = sfSprite_create();
	sfSprite_setTexture(spTmp, tmp, sfTrue);
	return spTmp;
}

// Set text with his font
sfText* CreateText(char* _path)
{
	sfFont* tmp = sfFont_createFromFile(_path);
	sfText* txtTmp = sfText_create();
	sfText_setFont(txtTmp, tmp);
	return txtTmp;
}

// Check the hitbox between two circle vectors
sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2)
{
	// Version non optimis�
	// if (sqrt(pow(_pos1.x - _pos2.x, 2) + pow(_pos1.y - _pos2.y, 2)) < _rayon1 + _rayon2) 
	
	// Version Optimis�
	if ((_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y) < (_rayon1 + _rayon2) * (_rayon1 + _rayon2))
	{
		return sfTrue;
	}
	else return sfFalse;
}

// Check the hitbox between two rectangle vectors
sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2)
{
	if ((_box2.left >= _box1.left + _box1.width)      // trop � droite
		|| (_box2.left + _box2.width <= _box1.left) // trop � gauche
		|| (_box2.top >= _box1.top + _box1.height) // trop en bas
		|| (_box2.top + _box2.height <= _box1.top))  // trop en haut
		return sfFalse;
	else
		return sfTrue;
}

// Get a random int between two value
int iRand(int _min, int _max)
{
	if (_max > _min)
		return rand() % (_max - _min + 1) + _min;
	else
		return _min;
}

// Get a random int between two value
int rand_int(int _min, int _max)
{
	return rand() % (_max - _min) + _min;
}

// Get a random float between two value
double rand_float(float _min, float _max)
{
	return ((rand() / (double)RAND_MAX) * ((double)_max - (double)_min) + (double)_min);
}

// Set one frame per second
float FrameRate()
{
	return 1.0f / getDeltaTime();
}
float Clamp(float x)
{
	if (x < 1.f && x > -1.f)
	{
		if (x < 0.f) 
		{
			return -1.0f;
		}
		else
		{
			return 1.0f;
		}
	}

}

// Display the frame rate per second
void displayFrameRate(sfRenderWindow* _window, sfText* _text)
{
	static float displayTimer = 0.0f;
	static char frameRate[25];

	displayTimer += getDeltaTime();

	if (displayTimer > 0.1f)
	{
		sprintf_s(frameRate, 25, "FPS : %.2f", FrameRate());
		displayTimer = 0.0f;
	}

	sfText_setString(_text, frameRate);
	sfRenderWindow_drawText(_window, _text, NULL);
}

// Save the screenshot in resource file
void screenshot(sfRenderWindow* _window)
{
	char hours[9] = { 0 }, date[9] = { 0 }, tmph[7] = { 0 }, tmpd[7] = { 0 };
	_strtime(hours);
	_strdate(date);
	sfVector2u windowSize;


	windowSize = sfRenderWindow_getSize(_window);
	sfTexture* texture = sfTexture_create(windowSize.x, windowSize.y);
	sfTexture_updateFromRenderWindow(texture, _window, 0, 0);
	sfImage* screenshot = sfTexture_copyToImage(texture);
	char filename[100];
	int j = 0;
	for (int i = 0; i < 9; i++) if (hours[i] != ':') tmph[j++] = hours[i];
	j = 0;
	for (int i = 0; i < 9; i++) if (date[i] != '/') tmpd[j++] = date[i];

	sprintf(filename, "../Ressources/Screenshots/Screenshot-%s-%s.jpg", tmpd, tmph);
	sfImage_saveToFile(screenshot, filename);
}