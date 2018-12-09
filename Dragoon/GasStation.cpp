#include "GasStation.h"


GasStation::GasStation(int x, int y)
{
	
	this->SetX(x);
	this->SetY(y-12);
	this->SetPic("images/gasStation.png");

	this->animation = al_load_bitmap("images/gasStation0.png");

	fuelAmount = 0.f;
	tankTime = 0.f;

	this->sound = al_load_sample("sounds/FuelGet.wav");


}

void GasStation::Refuel(Player *c){
	if(fuelAmount >= 25.f){
		this->customer = c;
		al_play_sample(sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
		fuelAmount = 0.f;
	}
}

void GasStation::Update(float delta){
	if(customer != NULL){
		tankTime -= delta*5;
		/*
		customer->SetX(this->x - 4);
		customer->SetY(this->y + 14);
		*/
		if(tankTime <= 0.f){
			customer->addFuel(10.f);
			customer = NULL;
		}
	}else{
		fuelAmount += delta*3;
		if(fuelAmount >= 25.f){
			tankTime = 8.f;
		}
	}
}

void GasStation::Draw(){
	al_draw_filled_rectangle(x + 5, y + 7 , x + 36, y + 47, al_map_rgb(0,10,0));
	al_draw_filled_rectangle(x + 5, y + 47 - (tankTime/8.f)*30 , x+36, y+47, al_map_rgb(0,255,0));
	al_draw_bitmap(animation, this->x, this->y, NULL);

	if(customer != NULL && customer->getRespawn() <= 0.f){
		al_draw_line(x+14 ,y+39 ,customer->GetOriginX(), customer->GetOriginY(), al_map_rgb(34, 34, 34), 4.f);
		al_draw_line(x+14 ,y+39 ,customer->GetOriginX(), customer->GetOriginY(), al_map_rgb(0, 108, 255), 2.f);
	}
}

GasStation::~GasStation(void)
{

}
