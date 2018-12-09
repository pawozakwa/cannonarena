#pragma once
#include "gameobject.h"
#include "Rocket.h"

#include "Effect.h"

#include <vector>
#include "CollisionEngine.h"
#include <allegro5\allegro_primitives.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Bomb.h"

class Player :
	public GameObject
{
private:
	ALLEGRO_BITMAP *picture;
	ALLEGRO_BITMAP *animation[4];
	ALLEGRO_BITMAP *shieldPic;

	ALLEGRO_BITMAP *rocket;
	ALLEGRO_BITMAP *bomb;

	ALLEGRO_BITMAP *rightWing;
	ALLEGRO_BITMAP *leftWing;

	ALLEGRO_BITMAP *xplode;

	float wingsOpen;

	int PlayerSpeed; 
	int JetPower;		// Power of jet pack
	int jumpHeigh;		// Heigh of jumping 
	float jumpLeft;		// Counter of jump power

	float jetFuel;		// Amount of fuel

	float TargetX;		
	float TargetY;

	GameObject *temp;

	float rage;

	float TeleportTargetX;
	float TeleportTargetY;

	float number;		// Player number
	bool shield;		// Have shield
	int special;		// Have special rocket
						// Zero when no, 1 when fast rocket, 2 when nuke rocket
	float laserTime;

	int score;
	int spree;

	ALLEGRO_COLOR Color;
	float coolDown;
	float tpcoolDown;
	float respawn;

	

public:
	Player(char* p, float x, float y, float SpeedX, float SpeedY, ALLEGRO_COLOR color);
	Player();
	~Player(void);
	void Update(float delta, float gravity, int screenH, int screenW, std::vector<GameObject*> &blocklist);
	void StepLeft(float delta);
	void StepRight(float delta);


	bool JetLeft(float delta);
	bool JetRight(float delta);

	void Jump(float delta, std::vector<GameObject*> &blocklist);
	Rocket* Shot(int kind);

	GameObject* GetBomb();

	void setTargeting(float x, float y);
	void setTeleportTargeting(float x, float y);


	float getEye();
	void DrawPlayer();
	Effect *Detonate(std::vector<GameObject*> &blocklist, int killerName);

	vector <Effect*> teleport(std::vector<GameObject*> &blocklist, int scrW, int scrH);
	
	float getCoolDown();
	void setCoolDown(float c);
	
	float getRespawn();
	void setRespawn(float r);

	void setNumber(int n);
	int getNumber();

	void setScore(int n);
	int getScore();

	void addFuel(float f);
	float getFuel();

	void setShield(bool s);
	bool isShield();

	int getSpecial();
	void setSpecial(int i);

	int getSpree();
	void setLaserTime(float t);

	void setJumpLeft(float j);
	float getJumpLeft();

	void setColor(ALLEGRO_COLOR n);

	ALLEGRO_COLOR getColor();

	float getTpCooldown();
	void SetRage(float a);
	void SetRage();
	float getRage();
	float getWing();

	
};

