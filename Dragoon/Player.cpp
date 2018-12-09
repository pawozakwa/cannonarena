#include "Player.h"
#include "Utils.h"
#include <math.h>

#define PI 3.14159

using namespace std;


Player::Player(char* p, float x, float y, float SpeedX, float SpeedY, ALLEGRO_COLOR color):
	GameObject(p, x, y, SpeedX, SpeedY)
{
	className = "Player";

	PlayerSpeed = 650;
	JetPower = 750;
	jumpHeigh = -150;

	TargetY = 0.1f;
	TargetX = -1.f;

	TeleportTargetY = 0.1f;
	TeleportTargetX = -1.f;

	setTargeting(0.1f, -1.f);

	jetFuel = 10;
	special = 0;
	shield = false;
	coolDown = 10.f;
	tpcoolDown = 10.f;

	wingsOpen = 0.f;

	respawn = 3.f;
	laserTime = 0.f;

	jumpLeft = 0.75f;

	Color = color;

	score = 0;
	spree = 0;

	animation[0] = al_load_bitmap("images/Player.png");
	animation[1] = al_load_bitmap("images/Player1.png");
	animation[2] = al_load_bitmap("images/Player2.png");
	animation[3] = al_load_bitmap("images/Player3.png");
	shieldPic = al_load_bitmap("images/shield.png");

	rocket = al_load_bitmap("images/rocket.png");
	bomb = al_load_bitmap("images/bomb/bomb1.png");

	rightWing = al_load_bitmap("images/rightwing.png");
	leftWing = al_load_bitmap("images/leftwing.png");

	xplode = al_load_bitmap("images/boom.png");

	this->explodeSample = al_load_sample("sounds/robot.wav");

	temp = new GameObject();
	temp->SetPic("images/Player.png");

	rage = 0.f;

	/*
	this->addFuel(150);*/
	//this->setSpecial(1);
}

Player::Player(){}

Player::~Player(void){


}

void Player::Update(float delta, float gravity, int screenH, int screenW, vector<GameObject*> &blocklist){

	//  Moving player with acctual speed & detecting collisions

	temp->SetX(x + SpeedX*delta);

	temp->SetY(y);

	bool collision = false;
	for (int i = 0; i < blocklist.size(); i++)
	{
		if(CollisionEngine::isCollidding(temp, blocklist.at(i), true))
			collision = true;
	}
	if(!collision){
		x += SpeedX*delta;		
	}else{
		SpeedX = 0;
	}
	collision = false;
	if(SpeedY < 0){
		temp->SetY(y + SpeedY*delta - 1);
	}else{
		temp->SetY(y + SpeedY*delta + 1);
	}


	temp->SetX(x);
	for (int i = 0; i < blocklist.size(); i++)
	{
		if(CollisionEngine::isCollidding(temp, blocklist.at(i), true))
			collision = true;
	}
	if(!collision){

		y += SpeedY*delta;
		SpeedY += gravity * delta;	
	}else{
		SpeedY = 0;
	}

	//floor(SpeedY);
	//////////////////////////////////////

	// Friction
	if( SpeedX != 0){
		if(SpeedY == 0)
			SpeedX *= 0.99;
		else
			SpeedX *= 0.999;
	}
	///////////



	// Checking ends of screen
	if(x <= 0){
		x = SpeedX = 0;
	}else if(x > screenW - picW){
		SpeedX = 0;	
		x = screenW - picW;
	}

	if(y > screenH - picH){
		y = screenH - picH;
		SpeedY = 0;
		//y -= 3;
	}else if(y < 0){
		y = 0;
		SpeedY = 0;
	}
	/////////////////////////



	// Countdown of cooldown and respawn
	if(coolDown > 0){
		this->coolDown -= delta*5;
	}

	if(tpcoolDown > 0){
		this->tpcoolDown -= delta*5;
	}

	if(respawn > 0){
		this->respawn -= delta*5;
		if(respawn < 0)
			this->respawn = 0.f;
	}


	// Control speed
	if(SpeedX > 400)
		SpeedX = 400;

	if(SpeedX < -400)
		SpeedX = -400;

	if(SpeedY < -200)
		SpeedY = -200;

	if(laserTime > 0)
		laserTime -= delta;
	else
		laserTime = 0;

	if(respawn >= 0.f)
		this->Alive = true;

	if(wingsOpen > 1.f)
		wingsOpen = 1.f;

	if((wingsOpen > 0.f && SpeedY > 0) || jetFuel <= 0.f)
		wingsOpen *= 0.95f;

	if(rage > 0)
		rage -= delta;
	else
		rage = 0;

}

void Player::StepLeft(float delta){
	if(SpeedY == 0){
		SpeedX -= PlayerSpeed*delta;
	}else{
		SpeedX -= PlayerSpeed*delta/5;
	}
}

//Step left too, if using joypad
void Player::StepRight(float delta){
	if(SpeedY == 0){
		SpeedX += PlayerSpeed*delta;//*1.5f;
	}else{
		SpeedX += PlayerSpeed*delta/5;
	}
}

bool Player::JetRight(float delta){

	if(jetFuel > 0){
		jetFuel -= delta;
		SpeedX -= JetPower*0.8f*delta;	
		SpeedY -= JetPower*delta;
		wingsOpen += 0.1f;
		return true;

	}	
	wingsOpen += 0.01f;
	jetFuel = 0;

	return false;
}

bool Player::JetLeft(float delta){
	if(jetFuel > 0){
		jetFuel -= delta;
		SpeedX += JetPower*0.8f*delta;	
		SpeedY -= JetPower*delta;
		wingsOpen += 0.1f;
		return true;

	}
	wingsOpen += 0.01f;
	jetFuel = 0;

	return false;
}

void Player::Jump(float delta, std::vector<GameObject*> &blocklist){
	temp->SetX(x);
	temp->SetY(y +2);

	bool collision = false;
	for (int i = 0; i < blocklist.size(); i++)
	{
		if(CollisionEngine::isCollidding(temp, blocklist.at(i), false))
			if(blocklist.at(i)->GetX() < temp->GetX() + temp->picH - 5)
				collision = true;
	}
	if(SpeedY == 0 && collision){
		jumpLeft = 0.75f;

	}
	if(jumpLeft > 0.f){
		SpeedY = jumpHeigh;
		jumpLeft -= delta;
	}
}

Rocket* Player::Shot(int kind){

	int rocketSpeed = 40;
	Rocket *r = new Rocket(this->GetOriginX()-20 + TargetX*5, this->getEye()-20 + TargetY*5, TargetX*rocketSpeed, TargetY*rocketSpeed, kind);
	r->setParentPlayerNumber(this->number);
	if(kind != 0)	
		special = 0;
	return r;
}

GameObject* Player::GetBomb(){
	float PlayerVelocityMultiplyerX = 0.1f * SpeedX;
	float PlayerVelocityMultiplyerY = 0.1f * SpeedY;
	float BombSpeed = 40;

	Bomb *r = new Bomb(this->GetOriginX() + TargetX*3, this->getEye() + TargetY*3, TargetX*BombSpeed + PlayerVelocityMultiplyerX, TargetY*BombSpeed  + PlayerVelocityMultiplyerY);
	r->setParentPlayerNumber(this->number);
	this->special = 0;
	return r;
}

void Player::setTargeting(float x, float y){
	x = (2*x + TargetX)/3;
	y = (2*y + TargetY)/3;

	float lenghtNow = sqrt(x*x + y*y);
	float a = 10/lenghtNow;


	TargetX = (x*a);
	TargetY = (y*a);
}

void Player::setTeleportTargeting(float x, float y){

	float lenghtNow = sqrt(x*x + y*y);
	float a = 10/lenghtNow;


	TeleportTargetX = (x*a);
	TeleportTargetY = (y*a);
}


float Player::getEye(){
	return y+0.4*picH;
}

void Player::DrawPlayer(){
	int anim = GetX();
	anim = (anim/3) % 4;

	al_draw_bitmap(leftWing, x + 6 - 18 * wingsOpen, y, NULL);
	al_draw_bitmap(rightWing, x + 10 + 18 * wingsOpen, y, NULL);


	al_draw_circle(x+ 19, y + 28, 7, this->Color, 4); // Wheel

	//al_draw_bitmap(animation[anim], x,y, NULL);
	al_draw_rotated_bitmap(animation[anim], 0,0,x,y,0,NULL);
	//this->Draw();	
	if(shield)
		al_draw_bitmap(shieldPic, GetOriginX() - al_get_bitmap_width(shieldPic)/2, GetOriginY() - al_get_bitmap_height(shieldPic)/2, NULL);

	// Draw Player color
	//al_draw_line(GetOriginX() , getEye()+ 3,  GetOriginX(), getEye() + 12, Color, 4);

	al_draw_line(GetOriginX() , getEye(),  GetOriginX() + TargetX*1.5f, getEye() + TargetY*1.5f, al_map_rgb(0,0,0), 5);	// Draw Target Line

	al_draw_line(GetOriginX() , getEye(),  GetOriginX() + TargetX*1.4f, getEye() + TargetY*1.4f, al_map_rgb(100,100,100), 3);	// Draw Target Line


	int laserLenght = 10;

	if(laserTime > 0){
		//al_draw_line(GetOriginX() , getEye(),  GetOriginX() + TargetX*100.f, getEye() + TargetY*100.f, Color, 1);	// Draw Target Line
		laserLenght = 100;

	}

	// Rysowanie celownika
	for (int i = 3; i < laserLenght; i++)
	{
		al_draw_filled_circle(GetOriginX() + TargetX * i, getEye() + TargetY * i, 2, Color);
	}



	if(special == 1)
		al_draw_tinted_rotated_bitmap(rocket,  al_map_rgb(50,50,255), al_get_bitmap_width(rocket)/2, al_get_bitmap_height(rocket)/2, GetOriginX(), getEye() - 5, Utils::getVectorAngle(TargetX,TargetY), NULL);

	if(special == 2)																																	   	
		al_draw_tinted_rotated_bitmap(rocket,  al_map_rgb(50,255,50), al_get_bitmap_width(rocket)/2, al_get_bitmap_height(rocket)/2, GetOriginX(), getEye() - 5, Utils::getVectorAngle(TargetX,TargetY), NULL);

	if(special == 3)																																	   	
		al_draw_rotated_bitmap(bomb, al_get_bitmap_width(rocket)/2, al_get_bitmap_height(rocket)/2, GetOriginX(), getEye() - 5, Utils::getVectorAngle(TargetX,TargetY), NULL);

	if(coolDown <= 0)
		al_draw_tinted_rotated_bitmap(rocket,  al_map_rgb(255, 50, 50), al_get_bitmap_width(rocket)/2, al_get_bitmap_height(rocket)/2, GetOriginX(), getEye(), Utils::getVectorAngle(TargetX,TargetY), NULL);

}

Effect *Player::Detonate(std::vector<GameObject*> &spawns, int killerName){
	Effect *e;
	if(!this->rage > 0.f){
		e = new Effect(xplode,GetOriginX() - 45, GetOriginY() - 45, 0, - 10, 0.2);
		e->setParentPlayerNumber(killerName);
		al_play_sample(this->explodeSample, 1, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);


		this->Alive = false;
		respawn = 8.f;
		jetFuel = 10.f;
		spree = 0;
		special = 0;
		//jetFuel = false;
		shield = false;

		int s = std::rand() % spawns.size();

		x = spawns.at(s)->GetX();
		y = spawns.at(s)->GetY();

		SpeedX = 0;
		SpeedY = 0;

	}else{
		e = NULL;
	}
	return e;
}

vector <Effect*> Player::teleport(std::vector<GameObject*> &blocklist, int scrW, int scrH){
	vector <Effect*> vec;
	if(jetFuel >= 5.f && tpcoolDown <= 0.f){

		int particleSpeed = 10;
		bool good = false;
		float distance = 25;
		Player * tmp = new Player();
		tmp->SetPic("images/Player.png");
		for (int k = 0; k < 15; k++){						
			vec.push_back( new Effect(NULL, this->GetOriginX() + rand() % 30 - 15, this->GetOriginY() + rand() % 50 - 25, TeleportTargetX * particleSpeed, TeleportTargetY * particleSpeed, 0.4f, 1));
		}
		while(!good){
			tmp->SetX(x + TeleportTargetX * distance);
			tmp->SetY(y + TeleportTargetY * distance);
			good = true;
			for (int i = 0; i < blocklist.size(); i++)
			{
				if(CollisionEngine::isCollidding(tmp, blocklist.at(i), true))
					good = false;
			}

			if(!(tmp->GetX() > 35 && tmp->GetX() < scrW -35 && tmp->GetY() > 0 && tmp->GetY() < scrH)){
				good = false;
			}

			distance -= 1.f;
			if(distance < -1){ 
				delete tmp;
				vec.clear();
				return vec;
			}
		}
		x = tmp->GetX();
		y = tmp->GetY();
		jetFuel -= 5.f;
		tpcoolDown = 4.f;

		for (int k = 0; k < 15; k++){						
			vec.push_back( new Effect(NULL, this->GetOriginX() + rand() % 30 - 15, this->GetOriginY() + rand() % 50 - 25, TeleportTargetX * particleSpeed, TeleportTargetY * particleSpeed, 0.4f, 1));
		}
		delete tmp;
		return vec;
	}
	vec.clear();
	return vec;
}

float Player::getCoolDown(){ return this->coolDown;}
void Player::setCoolDown(float c){ this->coolDown = c;}

float Player::getRespawn(){ return this->respawn;}
void Player::setRespawn(float r){ this->respawn = r;};

void  Player::setNumber(int n){	this->number = n;}
int  Player::getNumber(){ return this->number;}

void  Player::setScore(int n){	
	this->score = n;
	this->spree += 10;
}

int  Player::getScore(){ return this->score;

}

void Player::addFuel(float f){	this->jetFuel += f;}

float Player::getFuel(){ return this->jetFuel;}

void Player::setShield(bool s){shield = s;}

bool Player::isShield(){return shield;}

void Player::setSpecial(int i){	this->special = i;}

int Player::getSpecial(){ return special;}

int Player::getSpree(){ return spree; }

void Player::setLaserTime(float t){
	this->laserTime = t;
}

void Player::setJumpLeft(float j){
	this->jumpLeft = j;
}

float Player::getJumpLeft(){
	return jumpLeft;
}

void Player::setColor(ALLEGRO_COLOR n){	this->Color = n;}
ALLEGRO_COLOR Player::getColor(){ return this->Color;}

float Player::getTpCooldown(){ return tpcoolDown; }

void Player::SetRage(){
	rage = 5.9f;
	coolDown = 1.f;
	jetFuel += 15.f;
}

void Player::SetRage(float a){
	rage = 0.f;
}

float Player::getRage(){
	return rage;
}

float Player::getWing(){
	return wingsOpen;
}