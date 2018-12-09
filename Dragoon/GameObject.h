#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string>

using namespace std;


class GameObject
{
protected:
	float x, y;
	ALLEGRO_BITMAP *picture;


	float SpeedX, SpeedY;
	
	bool **bitMap;

	ALLEGRO_SAMPLE *explodeSample;
	
	int parentPlayerNumber;
	bool Alive;

	bool kicked;



public:
	
	int picH, picW;

	string className;

	GameObject();
	GameObject(char* p, float x, float y, float SpeedX, float SpeedY);



	~GameObject(void);
	//void Update(float deltaTime, float gravity, int screenH, int screenW);
	void Draw();

	void applyVecField(float x, float y);

	void SetX(float x);
	void SetY(float y);
	float GetX();
	float GetY();
	void SetSpeedX(float x);
	void SetSpeedY(float y);
	float GetSpeedX();
	float GetSpeedY();
	bool** GetBitMap();
	float GetOriginX();
	float GetOriginY();

	void SetPic(char*p);
	void SetPic(ALLEGRO_BITMAP *p);

	ALLEGRO_BITMAP* GetPic();


	int getParentPlayerNumber();
	void setParentPlayerNumber(int number);

	bool isAliv();
	
};

