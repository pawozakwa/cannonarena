#include "Rocket.h"
#include "Utils.h"

#include <iostream>


Rocket::Rocket(void)
{
}

Rocket::Rocket( float x, float y, float SpeedX, float SpeedY, int kind):
	GameObject("images/rocket.png", x, y, SpeedX, SpeedY)
{
	this->kind = kind;
	angle= Utils::getVectorAngle(SpeedX, SpeedY);
	this->SetRotatedPic("images/rocket.png");
	className = "Rocket";

	if(kind == 2)		{
		this->explodeSample = al_load_sample("sounds/nuke.wav");
		xplode2 = al_load_bitmap("images/boom2.png");
	}
	else{
		this->explodeSample = al_load_sample("sounds/rocket.wav");
		xplode = al_load_bitmap("images/boom.png");
	}

	kicked = false;

}

Rocket::~Rocket(void)
{
	//al_destroy_sample(this->explodeSample);
}

void Rocket::DrawRocket(){
	if(!kicked){
		if(kind == 0) al_draw_tinted_bitmap(picture, al_map_rgb(255, 50, 50), x, y, NULL);

		else if(kind == 1) al_draw_tinted_bitmap(picture, al_map_rgb(50,50,255), x, y, NULL);

		else if(kind == 2) al_draw_tinted_bitmap(picture, al_map_rgb(50,255,50), x, y, NULL);
	}else{
		if(kind == 0) al_draw_tinted_rotated_bitmap(picture, al_map_rgb(255, 50, 50),picW/2, picH/2, x + picW/2, y + picH/2,angle, NULL);		

		else if(kind == 1) al_draw_tinted_rotated_bitmap(picture, al_map_rgb(50,50,255),picW/2, picH/2, x + picW/2, y + picH/2,angle, NULL);

		else if(kind == 2) al_draw_tinted_rotated_bitmap(picture, al_map_rgb(50,255,50),picW/2, picH/2, x + picW/2, y + picH/2,angle, NULL);
	}
	
}

void Rocket::SetRotatedPic(char*p){
	ALLEGRO_BITMAP *tmp = al_load_bitmap(p);

	ALLEGRO_BITMAP *tmpr = al_create_bitmap(40,40);
	if(!tmp){
		cout << "Problem with rocket object picture" << endl;
	}else{
		this->picH = al_get_bitmap_height(picture);
		this->picW = al_get_bitmap_width(picture);
	}

	al_set_target_bitmap(tmpr);	
	al_draw_rotated_bitmap(tmp, al_get_bitmap_width(tmp)/2, al_get_bitmap_height(tmp)/2, al_get_bitmap_width(tmpr)/2 + 5, al_get_bitmap_height(tmpr)/2, angle, NULL);

	this->SetPic(tmpr);
}

void Rocket::Update(float delta, float gravity){
	//delta = delta * 0.5f;

	if(kind == 1) delta *= 3;

	if(kicked){
		SpeedY += gravity*delta/2;
		angle += delta*5;
	}

	this->y += this->SpeedY*delta;
	this->x += this->SpeedX*delta;
}

Effect *Rocket::Detonate(){
	this->Alive = false;
	Effect *e;
	if(kind == 2){
		e = new Effect(xplode2 ,GetOriginX() - 155, GetOriginY() - 128, 0, - 10, 0.4);
	}else{
		e = new Effect(xplode ,GetOriginX() - 45, GetOriginY() - 45, 0, - 10, 0.2);
	}


	e->setParentPlayerNumber(this->parentPlayerNumber);
	al_play_sample(this->explodeSample, 1, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
	return e;
}

void Rocket::setAngle(int a){angle = a; }
int Rocket::getAngle(){return angle; }
int Rocket::getKind(){
	return kind; }