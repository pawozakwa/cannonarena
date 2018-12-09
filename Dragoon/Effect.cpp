#include "Effect.h"

#include <allegro5\allegro_primitives.h>


Effect::Effect(char *p, float x, float y, float sx, float sy):
	GameObject(p, x, y, sx, sy)
{
	this->lifeTime = -1;
	this->initialLifeTime = -1;
}



Effect::Effect(ALLEGRO_BITMAP* pic, float x, float y, float sx, float sy, float lifeTime)
{
	this->lifeTime = lifeTime;
	this->initialLifeTime = lifeTime;

	this->x = x;
	this->y = y;
	this->SpeedX = sx;
	this->SpeedY = sy;
	if(pic != NULL){
		this->SetPic(pic);
	}

	this->scoreDisplay = 0;
	this->kind = 0;
}

Effect::Effect(ALLEGRO_BITMAP* pic, float x, float y, float sx, float sy, float lifeTime, int k)
{
	this->lifeTime = lifeTime;
	this->initialLifeTime = lifeTime;

	this->x = x;
	this->y = y;
	this->SpeedX = sx;
	this->SpeedY = sy;
	if(pic != NULL){
		this->SetPic(pic);
	}

	this->scoreDisplay = 0;

	this->kind = k;

}


Effect::~Effect(void)
{
}



void Effect::UpdateEffect(float delta){
	this->y += this->SpeedY*delta;
	this->x += this->SpeedX*delta;
	if(lifeTime > 0){	
		lifeTime -= delta;
		if(lifeTime <= 0){
			this->alive = false;
		}
	}
}

void Effect::setScoreDisplay(int a){
	this->scoreDisplay = a;
	font = al_load_font("fonts/banana.ttf", 15, NULL);
}

void Effect::DrawEffect(){

	if(scoreDisplay == 0){
		float f = lifeTime/initialLifeTime;


		ALLEGRO_COLOR yellow = al_map_rgba(200, 200, 0, 150 - 15*f);
		ALLEGRO_COLOR red = al_map_rgba(200, 50, 0, 255*f);
		ALLEGRO_COLOR smoke = al_map_rgba(50, 50, 50, 255);

		if(kind == 1){
			yellow = al_map_rgba(50, 50, 255, 150 - 15*f);
			red = al_map_rgba(220, 220, 255, 255*f);
			smoke = al_map_rgba(20, 20, 120, 255);
		}else if(kind == 2){
			red = al_map_rgba(255, 0, 0, 255*f);
			yellow = al_map_rgba(100, 0, 0, 150 - 15*f);
			smoke = al_map_rgba(20, 0, 0, 255);
		}

		al_draw_filled_circle(x,y,5*f, smoke);
		al_draw_filled_circle(x,y,4*f, yellow);
		al_draw_filled_circle(x,y,2*f, red);
	}
	else{
		string string = to_string(scoreDisplay);
		char const *num = string.c_str();
		al_draw_text(font, al_map_rgb(10,10,10), x,y, NULL, num);
	}
}

void Effect::DrawSmoke(){
	float f = lifeTime/initialLifeTime;
	al_draw_tinted_bitmap(picture, al_map_rgba_f(1, 1, 1, f), x-picW/2, y-picH/2, 0);
}

void Effect::setAlive(bool a){
	this->alive = a;
}

bool Effect::isAlive(){
	return alive;
}