#include "powerUp.h"
#include <math.h>


powerUp::powerUp(int kind, float lifeTime)
{

	this->kind = kind;
	this->lifeTime = lifeTime;
	int los = rand() % 20;
	if(los < 1 && kind != 5){
		this->SetPic("images/powers/random.png");
	}else{

		switch (kind)
		{
		case 1:
			this->SetPic("images/powers/speed.png");
			break;
		case 2:
			this->SetPic("images/powers/nuke.png");
			break;
		case 3:
			this->SetPic("images/powers/fuel.png");
			break;
		case 4:
			this->SetPic("images/powers/shield.png");
			break;
		case 5:
			this->SetPic("images/powers/bomb.png");
			break;
		case 6:
			this->SetPic("images/powers/scull.png");
			break;
		case 7:
			this->SetPic("images/powers/laser.png");
			break;
		case 8:
			this->SetPic("images/powers/bomb1.png");
			break;
		default:
			break;
		}
	}
	front = al_load_bitmap("images/powers/front.png");
	back = al_load_bitmap("images/powers/back.png");


	angle = 0;
}


powerUp::~powerUp(void)
{
}

void powerUp::Draw(){
	if(kind != 5){
		al_draw_bitmap(back, x, y, NULL);
		al_draw_rotated_bitmap(this->picture, picW/2, picH/2, x + picW/2, y + picH/2, angle, NULL);
		al_draw_bitmap(front, x, y, NULL);
	}else{
		al_draw_rotated_bitmap(this->picture, picW/2, picH/2, x + picW/2, y + picH/2, angle, NULL);
	}

}

bool powerUp::isAlive(){
	if(lifeTime <= 0)
		return false;
	else
		return true;
}

void powerUp::Update(float delta){
	lifeTime - delta;
	angle += delta;
}

void powerUp::setLifeTime(float l){
	this->lifeTime = l;
}

int powerUp::getKind(){
	return kind;
}