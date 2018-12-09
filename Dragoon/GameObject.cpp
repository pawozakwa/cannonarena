#include "GameObject.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#include <iostream>

using namespace std;


GameObject::GameObject()
{
}

GameObject::GameObject(char* p, float x, float y, float SpeedX, float SpeedY){
	if(p!=NULL)
		this->SetPic(p);

	this->x = x;
	this->y = y;
	this->SpeedX = SpeedX;
	this->SpeedY = SpeedY;

	this->Alive = true;

}

GameObject::~GameObject(void){
	for (int i = 0; i < picW; i++)
	{
		delete bitMap[i];
		
	}
	delete bitMap;

}

/*
void GameObject::Update(float deltaTime, float gravity, int screenH, int screenW){
this->x += this->SpeedX*deltaTime;

if(!(this->y > screenH - picH)){
this->SpeedY += gravity * deltaTime;	
this->y += this->SpeedY*deltaTime;
}

SpeedX *= 0.8;

if(x <= 0 || SpeedX < 0)
x = SpeedX = 0;

if(x > screenW - picW || SpeedX > 0){
SpeedX = 0;	
x = screenW - picW;
}

if(y > screenH - picH){
y = screenH - picH;
SpeedY = 0;
}else
if( y < 0){
y = 0;
SpeedY = 0;
}
}
*/

void GameObject::Draw(){
	al_draw_bitmap(picture, x, y, NULL);
}

void GameObject::applyVecField(float x, float y){
	this->SpeedX += x;
	this->SpeedY += y;

	if(x || y != 0)
		kicked = true;
}


//SETTERS & GETTERS
void GameObject::SetPic(char* p){
	this->picture = al_load_bitmap(p);
	if(!this->picture){
		cout << "Problem with GameObject picture" << endl;
	}else{
		this->picH = al_get_bitmap_height(picture);
		this->picW = al_get_bitmap_width(picture);
	}

	this->bitMap = new bool*[picW];
	al_lock_bitmap(this->picture, al_get_bitmap_format(picture), ALLEGRO_LOCK_READONLY);
	for (int i = 0; i < picW; i++)
	{
		bitMap[i] = new bool[picH];
		for (int j = 0; j < picH; j++)
		{
			bitMap[i][j] = false;
			if( al_get_pixel(picture,i,j).a > 0 )
				bitMap[i][j] = true;
		}
	}
	al_unlock_bitmap(picture);
}

void GameObject::SetPic(ALLEGRO_BITMAP *p){
	this->picture = p;
	if(!this->picture){
		cout << "Problem with object picture" << endl;
	}else{
		this->picH = al_get_bitmap_height(picture);
		this->picW = al_get_bitmap_width(picture);
	}

	this->bitMap = new bool*[picW];
	al_lock_bitmap(this->picture, al_get_bitmap_format(picture), ALLEGRO_LOCK_READONLY);
	for (int i = 0; i < picW; i++)
	{
		bitMap[i] = new bool[picH];
		for (int j = 0; j < picH; j++)
		{
			bitMap[i][j] = false;
			if( al_get_pixel(picture,i,j).a > 0 )
				bitMap[i][j] = true;
		}
	}
	al_unlock_bitmap(picture);
}

ALLEGRO_BITMAP* GameObject::GetPic(){
	return this->picture;
}

bool** GameObject::GetBitMap(){
	return bitMap;
}

void GameObject::SetX(float x){
	this->x = (float)x;
}
void GameObject::SetY(float y){
	this->y = (float)y;
}

float GameObject::GetX(){return x;}
float GameObject::GetY(){return y;}

float GameObject::GetSpeedX(){return SpeedX;}
float GameObject::GetSpeedY(){return SpeedY;}

void GameObject::SetSpeedX(float x){this->SpeedX = x;}
void GameObject::SetSpeedY(float y){this->SpeedY = y;}


float GameObject::GetOriginX(){ return x + picW/2;}
float GameObject::GetOriginY(){ return y + picH/2;}

int GameObject::getParentPlayerNumber(){
	return this->parentPlayerNumber;
}
void GameObject::setParentPlayerNumber(int number){
	this->parentPlayerNumber = number;
}

bool GameObject::isAliv(){
	return this->Alive;
}

