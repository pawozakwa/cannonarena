#pragma once
#include "gameobject.h"
#include "Effect.h"


class Rocket :
	public GameObject
{
private:
	float angle;
	int kind;

	ALLEGRO_BITMAP *xplode, *xplode2;
	

public:
	Rocket(float x, float y, float SpeedX, float SpeedY, int kind);
	Rocket(void);
	~Rocket(void);

	void DrawRocket();
	void SetRotatedPic(char*p);
	void Update(float delta, float gravity);

	Effect *Detonate();

	void setAngle(int a);
	int getAngle();

	int getKind();

	
};

