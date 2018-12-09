#pragma once
#include "gameobject.h"
#include <allegro5\allegro_font.h>

class Effect :
	public GameObject
{
private:
	

	bool alive; // If true effect should be draw
	float lifeTime; // If its less than zero its unlimited
	float initialLifeTime;

	int scoreDisplay;
	ALLEGRO_FONT* font;

	int kind;


public:
	Effect(char *p, float x, float y, float sx, float sy);
	Effect(ALLEGRO_BITMAP* pic, float x, float y, float sx, float sy,float lifeTime);
	Effect(ALLEGRO_BITMAP* pic, float x, float y, float sx, float sy,float lifeTime, int k);

	~Effect(void);

	void UpdateEffect(float delta);
	void setScoreDisplay(int a);
	
	void DrawEffect();
	void DrawSmoke();

	void setAlive(bool a);
	bool isAlive();


};

