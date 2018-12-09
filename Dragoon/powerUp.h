#pragma once
#include "gameobject.h"
class powerUp :
	public GameObject
{
private:
	int kind;
	float lifeTime;

	ALLEGRO_BITMAP * back;
	ALLEGRO_BITMAP * front;

	float angle;

public:
	powerUp(int kind, float lifeTime);
	~powerUp(void);
	void Draw();
	bool isAlive();
	void Update(float delta);
	void setLifeTime(float l);
	int getKind();
};

