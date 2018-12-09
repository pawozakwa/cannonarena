#include "Bomb.h"


Bomb::Bomb(float x, float y, float SpeedX, float SpeedY):
	GameObject("images/bomb/bomb0.png", x, y, SpeedX, SpeedY)
{
	parent = NULL;
	relativeX = 0;
	relativeY = 0;

	time = 0.f;
	Alive = true;

	animation[0] = al_load_bitmap("images/bomb/bomb0.png");
	animation[1] = al_load_bitmap("images/bomb/bomb1.png");

	explodeSample = al_load_sample("sounds/rocket.wav");

	xplode = al_load_bitmap("images/roboBoom.png");
}


Bomb::~Bomb(void)
{
}

void Bomb::Update(float delta, float gravity){

	if(parent != NULL && !parent->isAliv())
			parent = NULL;

	if(parent == NULL){
		SpeedY += gravity*delta;

		SetX(x + SpeedX*delta);
		SetY(y + SpeedY*delta);

		SpeedX *= 0.999;


	}else{

		x = parent->GetX() + relativeX;
		y = parent->GetY() + relativeY;
		

	}
	

	time += delta*3;

}

void Bomb::DrawBomb(){
	if(parent == NULL)
		al_draw_rotated_bitmap(animation[(int)time % 2], picW/2, picH/2, x, y, time*6, NULL);
	else
		al_draw_bitmap(animation[(int)time % 2], x, y, NULL);



}

void Bomb::Glue(GameObject* g){
	parent = g;
	relativeX = x - parent->GetX();
	relativeY = y - parent->GetY();
}

GameObject* Bomb::getParent(){
	return parent;
}


Effect* Bomb::Detonate(int p){
	Alive = false;
	Effect *e;
	e = new Effect(xplode ,GetOriginX() - 85, GetOriginY() - 85, 0, - 10, 0.2);
	e->setParentPlayerNumber(p);
	al_play_sample(explodeSample, 1, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
	return e;
}

bool Bomb::isSticked(){
	if(parent == NULL)
		return false;
	return true;
}

bool Bomb::isAlive(){
	return Alive;
}