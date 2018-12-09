#pragma once
#include "gameobject.h"
#include "Player.h"

class GasStation :
	public GameObject
{
private:
	ALLEGRO_BITMAP* animation;
	float fuelAmount;
	float tankTime;
	Player* customer;

	ALLEGRO_SAMPLE * sound;

public:
	GasStation(int x, int y);
	void Refuel(Player *c);
	void Update(float delta);
	void Draw();
	~GasStation(void);
};

