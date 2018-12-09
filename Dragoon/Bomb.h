#pragma once
#include "gameobject.h"
#include "CollisionEngine.h"
#include <vector>
class Bomb :
	public GameObject
{
private:
	GameObject* parent;
	int relativeX;
	int relativeY;

	float time;

	bool Alive;

	ALLEGRO_SAMPLE *explodeSample;

	ALLEGRO_BITMAP *animation[2];

	ALLEGRO_BITMAP *xplode;

public:


	Bomb(float x, float y, float SpeedX, float SpeedY);
	~Bomb(void);
	void Update(float delta, float gravity);
	void DrawBomb();
	void Glue(GameObject* g);
	GameObject* getParent();

	Effect *Detonate(int p);

	bool isSticked();
	bool isAlive();
	

};

