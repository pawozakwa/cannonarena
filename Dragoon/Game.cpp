#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>

#include <allegro5\allegro_ttf.h>
#include <Windows.h>

#include "GameObject.h"
#include "CollisionEngine.h"
#include "Rocket.h"
#include "Utils.h"
#include "Effect.h"
#include "powerUp.h"
#include "Bomb.h"
#include "VectorField.h"
#include "LevelsManager.h"
#include "GasStation.h"

#include <iostream>
#include <vector>
#include <future>

#define Stick1x stick[0].axis[0]
#define Stick1y stick[0].axis[1]
#define Stick2x stick[0].axis[2]
#define Stick2y stick[1].axis[0]

using namespace std;

int PlayTheGame(){

	ALLEGRO_DISPLAY * gameWindow;

	bool gameRunning = true;
	int ScreenWidth = 1280;
	int ScreenHeight = 760;
	ScreenHeight -= ScreenHeight %40;
	ScreenWidth -= ScreenWidth %40;

	string levelName = "else";


	float gravity = 200.f;
	char* appName = "          CANNON ARENA  by .:Xv0:.";

	float gameTime = 350.f;

	//gameTime = 10.f;

	float initialGameTime = gameTime;

	const bool debug = false;

	if(!al_init()){
		cout << "Error with initialization alllegro" << endl;
		return -1;
	}

	if(!al_install_audio()){
		cout << "failed to initialize audio!" << endl;
		return -1;
	}


	//al_set_new_display_flags(  ALLEGRO_RESIZABLE);
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	gameWindow = al_create_display(ScreenWidth + 200, ScreenHeight);

	al_init_image_addon();
	if(!al_install_joystick()){
		cout << "Joy installation problem" << endl;
		return -1;
	}

	const int playerNumber = al_get_num_joysticks();


	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	cout << al_get_num_joysticks() << " pads connected!" << endl;
	al_set_app_name(appName);
	al_set_window_title(gameWindow, appName);

	//Draw Menu
	ALLEGRO_BITMAP *background = al_load_bitmap("images/menu.png");
	ALLEGRO_BITMAP *tip = al_load_bitmap("images/menuTip.png");
	ALLEGRO_BITMAP *helpPic = al_load_bitmap("images/help.png");

	bool ready = false;
	bool help = false;
	bool credits = false;
	string mapList[] = {"classic", "arena", "catacombs", "heaven", "skyscrapers", "hell", "killhouse", "else", "kosmat"};
	int map = 0;


	ALLEGRO_FONT *font = al_create_builtin_font();
	ALLEGRO_FONT *banana = al_load_ttf_font("fonts/banana.ttf",22,NULL);
	ALLEGRO_FONT *menuFont = al_load_ttf_font("fonts/banana.ttf",40,NULL);

	// Using joys
	ALLEGRO_JOYSTICK ** joysticks = new ALLEGRO_JOYSTICK*[playerNumber];
	ALLEGRO_JOYSTICK_STATE ** joyStates = new ALLEGRO_JOYSTICK_STATE*[playerNumber];
	for (int i = 0; i < playerNumber; i++)
	{
		joysticks[i] = al_get_joystick(i);
		joyStates[i] = new ALLEGRO_JOYSTICK_STATE();
	}

	ALLEGRO_BITMAP* backMenu = al_load_bitmap("images/backMenu.png");
	do
	{
		al_rest(0.15f);
		al_get_joystick_state(joysticks[0], joyStates[0]);
		help = false;
		credits = false;



		if(joyStates[0]->button[1]){
			map++;
			map %= 9;}

		if(joyStates[0]->button[3])
			help = true;

		if(joyStates[0]->button[0])
			credits = true;

		if(joyStates[0]->button[2]){
			ready = true;
			background = al_load_bitmap("images/load.png");
		}


		al_draw_bitmap(backMenu,0,0,NULL);
		for (int i = 0; i < 300; i++)
		{
			al_draw_filled_circle(490 + rand()%510, 250 + rand()%50, rand()%20, al_map_rgb(0, 200 + rand()%50, 0));
		}
		al_draw_bitmap(background,0,0,NULL);
		al_draw_bitmap(tip,0,0,NULL);
		al_draw_text(menuFont ,al_map_rgb(200, 200,200), ScreenWidth / 2 - 100, 400, NULL, "Level => ");
		al_draw_text(menuFont ,al_map_rgb(150, 150,150), ScreenWidth / 2 + 100, 400, NULL, mapList[map].c_str());

		al_draw_text(menuFont ,al_map_rgb(200, 200,200), ScreenWidth / 2 - 50, 450, NULL, "Players => ");		
		al_draw_text(menuFont ,al_map_rgb(100, 100,100), ScreenWidth / 2 + 175, 450, NULL, to_string(playerNumber).c_str());

		if(help)
			al_draw_bitmap(helpPic,0,0,NULL);



		al_flip_display();

	} while (!ready);

	levelName = mapList[map];


	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_bitmap(background,0,0,NULL);
	al_flip_display();

	ALLEGRO_BITMAP *barBack = al_load_bitmap("images/interface/barBack.png");
	ALLEGRO_BITMAP *screen = al_load_bitmap("images/interface/screen.png");

	ALLEGRO_BITMAP *emptyPic = al_load_bitmap("images/null.png");
	ALLEGRO_BITMAP *boom = al_load_bitmap("images/boom.png");
	ALLEGRO_BITMAP *boom2 = al_load_bitmap("images/boom2.png");
	ALLEGRO_BITMAP *bigBoom = al_load_bitmap("images/roboBoom.png");

	ALLEGRO_BITMAP *boomSmoke = al_load_bitmap("images/boomSmoke.png");
	ALLEGRO_BITMAP *boom2Smoke = al_load_bitmap("images/boom2Smoke.png");
	ALLEGRO_BITMAP *bigBoomSmoke = al_load_bitmap("images/roboBoomSmoke.png");

	ALLEGRO_BITMAP *spawn = al_load_bitmap("images/spawn.png");
	ALLEGRO_BITMAP *splash = al_load_bitmap("images/splash.png");




	if(!al_init_acodec_addon()) {
		cout << "failed to initialize audio codecs!" << endl;
		return -1;
	}

	if (!al_reserve_samples(8)){
		cout << "failed to reserve samples!" << endl;
		return -1;
	}

	if(!al_install_keyboard()){
		cout << "Keyboard installation problem" << endl;
		return -1;
	}



	ALLEGRO_COLOR PlayersColors[8] ={
		al_map_rgb(46,46,250),  //blue
		al_map_rgb(180,0,0),    //red
		al_map_rgb(4,180,4),	//green
		al_map_rgb(215,223,1),  //yellow
		al_map_rgb(250,250,250),//white		
		al_map_rgb(255,128,0),  //orange
		al_map_rgb(0,0,0),		//black
		al_map_rgb(125,125,125),//grey
	};




	background = al_load_bitmap("images/back.jpg");
	ALLEGRO_SAMPLE *pick_sample = al_load_sample("sounds/pick.wav");
	ALLEGRO_SAMPLE *evil = al_load_sample("sounds/evil.wav");
	ALLEGRO_SAMPLE *applause = al_load_sample("sounds/applause.wav");
	ALLEGRO_SAMPLE *applause2 = al_load_sample("sounds/applause2.wav");
	ALLEGRO_SAMPLE *rock = al_load_sample("sounds/rock.wav");
	ALLEGRO_SAMPLE *launch = al_load_sample("sounds/launch.wav");
	ALLEGRO_SAMPLE *spawnSound = al_load_sample("sounds/spawn.wav");

	ALLEGRO_EVENT_QUEUE *event = al_create_event_queue();	
	al_register_event_source(event, al_get_keyboard_event_source());


	vector <GameObject*> BlocksList;
	vector <Rocket*> rocketsList;
	vector <Effect*> explosions;
	vector <Effect*> effectsList;
	vector <Effect*> decals;
	vector <Effect*> otherEffects;
	vector <powerUp*> powerUps;
	vector <Bomb*> bombs;
	vector <GasStation*> stations;

	vector <GameObject*> spawns;

	// Level project
	int* levelDefinition = LevelsManager::getLevel(levelName);

	// TO DO loop to adding ground blocks -aka- creating level
	int counter = 0;
	for (int i = 0; i < ScreenHeight/40; i++)
	{
		for (int j = 0; j < ScreenWidth/40; j++)
		{
			if(levelDefinition[counter] == 1){
				BlocksList.push_back(new GameObject("images/block.png", j*40, i*40, 0.f, 0.f));
			}else if(levelDefinition[counter] == 5){
				BlocksList.push_back(new GameObject("images/window.png", j*40, i*40, 0.f, 0.f));
			}else if(levelDefinition[counter] == 2){
				powerUp* bomb = new powerUp(5,180.f);
				bomb->SetX(j*40);
				bomb->SetY(i*40);
				powerUps.push_back(bomb);
			}else if(levelDefinition[counter] == 3){
				stations.push_back(new GasStation(j*40, i*40));
			}else if(levelDefinition[counter] == 4){
				spawns.push_back(new GameObject(NULL, (float)j*40, (float)i*40,0.f,0.f));
			}
			counter++;
		}
	}

	Player ** players = new Player*[playerNumber];
	for (int i = 0; i < playerNumber; i++)
	{
		//srand(GetTickCount64());
		int place = rand() % spawns.size();
		players[i] = new Player("images/Player.png", spawns.at(place)->GetX(), spawns.at(place)->GetY(), 0.0f,0.f, PlayersColors[i]);
		players[i]->setNumber(i);
	}



	// Declaring clocks and getint current time
	float newTime, oldTime, deltaTime;
	newTime = GetTickCount64();

	// Creating Vector Field
	VectorField * Forces;
	Forces = new VectorField(ScreenWidth, ScreenHeight);


	//---------------------------------------------- Main loop ---------------------------------------------//
	while(gameTime > 0.f){

		// Delta time calculating
		oldTime = newTime;
		newTime = GetTickCount64();
		deltaTime = newTime - oldTime;
		deltaTime /= 1000;
		srand(newTime);
		/////////////////////////





		gameTime -= deltaTime;
		// Keyboard reacting
		ALLEGRO_EVENT e;
		al_get_next_event(event, &e);

		ALLEGRO_KEYBOARD_STATE *keyboard = new ALLEGRO_KEYBOARD_STATE();

		// Using joypads
		for (int i = 0; i < playerNumber; i++)
		{
			if(players[i]->getRespawn() <= 0){
				al_get_joystick_state(joysticks[i], joyStates[i]);

				// Jump
				if(joyStates[i]->button[2])
					players[i]->Jump(deltaTime, BlocksList);
				if(!joyStates[i]->button[2])
					players[i]->setJumpLeft(0.f);


				float movement = deltaTime * joyStates[i]->Stick1x;

				players[i]->StepRight(movement);

				players[i]->setTargeting(joyStates[i]->Stick2x, joyStates[i]->Stick2y);

				players[i]->setTeleportTargeting(joyStates[i]->Stick1x, joyStates[i]->Stick1y);

				// Teleport
				if(joyStates[i]->button[1]){


					vector <Effect*> v = players[i]->teleport(BlocksList, ScreenWidth, ScreenHeight);
					if(!v.empty()){
						al_play_sample(spawnSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

						effectsList.insert(effectsList.end(), v.begin(), v.end());
					}


				}


				// Standard shot
				if(joyStates[i]->button[5]){
					if(players[i]->getCoolDown() <= 0.f){
						rocketsList.push_back(players[i]->Shot(0));
						al_play_sample(launch, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
						al_set_target_bitmap(al_get_backbuffer(gameWindow)); //return to screen focus drawing after generating rotated missle

						if(players[i] -> getRage() > 1){

							players[i]->setCoolDown(0.2f + 7.f * 1/((players[i]->getRage())*(players[i]->getRage())));

						}else{
							players[i]->setCoolDown(7.f);
						}


						//players[i]->setCoolDown(2.f);
					}
				}

				// Power shot
				if(joyStates[i]->button[4] && players[i]->getSpecial() != 0){
					if(players[i]->getSpecial() == 3){
						bombs.push_back((Bomb*)players[i]->GetBomb());	
					}else{
						rocketsList.push_back(players[i]->Shot(players[i]->getSpecial()));					
						al_play_sample(launch, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
						al_set_target_bitmap(al_get_backbuffer(gameWindow)); //return to screen focus drawing after generating rotated missle
					}
				}			

				//Launch bomb
				if(joyStates[i]->button[3]){
					for (int j = 0; j < bombs.size(); j++){
						if(bombs.at(j)->isAlive() && bombs.at(j)->getParentPlayerNumber() == i){
							otherEffects.push_back(new Effect(bigBoomSmoke,bombs.at(j)->GetOriginX(), bombs.at(j)->GetOriginY(), 0, - 10, 50));
							for (int p = 0; p < 50; p++){


								effectsList.push_back(
									new Effect(NULL,  bombs.at(j)->GetOriginX() +rand()%50 - 25, bombs.at(j)->GetOriginY()+rand()%50 - 25, rand()%200 - 100, rand()%200 - 190, 1.3f));
							}
							explosions.push_back(bombs.at(j)->Detonate(i));

						}
					}
				}


				// Right Jet
				if(joyStates[i]->button[7]){
					if(players[i]->JetRight(deltaTime)){
						for (int p = 0; p < 5; p++)						
							effectsList.push_back(new Effect(emptyPic, players[i]->GetOriginX() +7, players[i]->GetOriginY() + 5,  30 + rand()%30, 100+rand()%100, 0.29));

						effectsList.push_back(new Effect(emptyPic, players[i]->GetOriginX() - 7 - players[i]->getWing()*18, players[i]->GetOriginY() + 2,  0, 50, 0.29));
						effectsList.push_back(new Effect(emptyPic, players[i]->GetOriginX() + 7 + players[i]->getWing()*18, players[i]->GetOriginY() + 2,  0, 50, 0.29));
					}
				}
				// Left Jet
				if(joyStates[i]->button[6]){
					if(players[i]->JetLeft(deltaTime)){
						for (int p = 0; p < 5; p++)
							effectsList.push_back(new Effect(emptyPic, players[i]->GetOriginX() -7, players[i]->GetOriginY() + 5, -30 - rand()%30, 100+rand()%100, 0.29));
						effectsList.push_back(new Effect(emptyPic, players[i]->GetOriginX() - 7 - players[i]->getWing()*18, players[i]->GetOriginY() + 2,  0, 50, 0.29));
						effectsList.push_back(new Effect(emptyPic, players[i]->GetOriginX() + 7 + players[i]->getWing()*18, players[i]->GetOriginY() + 2,  0, 50, 0.29));

					}
				}

				//TEST
				if(joyStates[i]->button[0] && debug){
					players[i]->SetRage();
				}
			}
		}

		// Using keyboard
		al_get_keyboard_state(keyboard);
		if(al_key_down(keyboard, ALLEGRO_KEY_D)){
			players[0]->StepRight(deltaTime);
		}
		if(al_key_down(keyboard, ALLEGRO_KEY_A)){
			players[0]->StepLeft(deltaTime);
		}
		if(al_key_down(keyboard, ALLEGRO_KEY_W)){
			players[0]->Jump(deltaTime, BlocksList);			
		}
		if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE || al_key_down(keyboard, ALLEGRO_KEY_ESCAPE))
		{
			al_destroy_display(gameWindow);
			al_destroy_event_queue(event);
			return -1;
		}
		if( al_key_down(keyboard, ALLEGRO_KEY_F2)){
			al_destroy_display(gameWindow);
			al_destroy_event_queue(event);
			return 0;
		}


		bool pause = al_key_down(keyboard, ALLEGRO_KEY_P);
		while(pause){
			deltaTime = 0.f;
			al_rest(1);
			while(pause){
				al_get_keyboard_state(keyboard);
				pause = al_key_down(keyboard, ALLEGRO_KEY_P);
			}
		}
		///////////////////




		////////////// LOGIC ///////////////////////////////////////

		Forces->Calculate(explosions);

		// Appling changes for players
		for (int i = 0; i < playerNumber; i++){

			players[i]->Update(deltaTime, gravity, ScreenHeight, ScreenWidth, BlocksList);

			if(players[i]->getRespawn() <= 0){

				//Appling forces;

				players[i]->applyVecField(Forces->getFieldX(players[i]->GetOriginX(), players[i]->GetOriginY()), Forces->getFieldY(players[i]->GetOriginX(),players[i]->GetOriginY()));

				for (int j = 0; j < players[i]->getRage() ; j++)
				{
					effectsList.push_back(
						new Effect(NULL,  players[i]->GetOriginX() + rand()%10 - 5, players[i]->GetOriginY() + rand()%30 - 15, rand()%20 - 10, rand()%90 - 90, 0.9f,2));

				}

				// Checking explosions contact
				for (int j = 0; j < explosions.size(); j++)
				{
					if(explosions.at(j)->isAlive() && CollisionEngine::isCollidding(players[i], explosions.at(j), true)){
						if(players[i]->isShield()){
							players[i]->setShield(false);
							explosions.at(j)->setAlive(false);
							break;
						}else if(players[i]->getRage() > 0.f)
						{
							break;
						}
						else{
							otherEffects.push_back(new Effect(boomSmoke,players[i]->GetOriginX(), players[i]->GetOriginY(), 0, -10, 50));
							decals.push_back(new Effect(splash, players[i]->GetOriginX() - 45, players[i]->GetOriginY()-37, 0, 0, 30));

							for (int p = 0; p < 30; p++){
								effectsList.push_back(
									new Effect(NULL,  players[i]->GetOriginX() + rand()%90 -45, players[i]->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f));
							}
							/*
							Effect* e = players[i]->Detonate(spawns, explosions.at(j)->getParentPlayerNumber());

							if (e != NULL){
								explosions.push_back(e);
							}*/
							explosions.push_back(players[i]->Detonate(spawns, explosions.at(j)->getParentPlayerNumber()));

							int killer = explosions.at(j)->getParentPlayerNumber();
							if(killer != i){							
								players[killer]->setScore(players[killer]->getScore() + 10);
								Effect *e = new Effect(emptyPic, players[killer]->GetX(),players[killer]->GetY(), 0, -20, 3);
								e->setScoreDisplay(10);
								effectsList.push_back(e);
								switch (players[killer]->getSpree())
								{
								case 40:
									al_play_sample(applause, 1.2f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
									break;								
								case 50:
									al_play_sample(rock, 1.9f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
									players[killer]->SetRage();
									break;
								case 100:
									al_play_sample(rock, 1.9f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
									players[killer]->SetRage();
									break;
								case 150:
									al_play_sample(rock, 1.9f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
									players[killer]->SetRage();
									break;
								case 70:
									al_play_sample(applause2, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
									break;

								default:
									break;
								}
							}else{
								players[killer]->setScore(players[killer]->getScore() - 10);
								players[killer]->addFuel(-10.f);
								Effect *e = new Effect(emptyPic, players[killer]->GetX(),players[killer]->GetY(), 0, -20, 3);
								e->setScoreDisplay(10);
								effectsList.push_back(e);

							}
							break;
						}
					}
				}

				// Checking powerUps contact
				for (int j = 0; j < powerUps.size(); j++){
					if(players[i]->getRespawn() <= 0 && powerUps.at(j)->isAlive() && CollisionEngine::isCollidding(players[i], powerUps.at(j), true)){

						switch (powerUps.at(j)->getKind())
						{
						case 1:
							if(players[i]->getSpecial() == 0){
								players[i]->setSpecial(1);
								powerUps.at(j)->setLifeTime(0.f);
								al_play_sample(pick_sample, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							}
							break;

						case 2:
							if(players[i]->getSpecial() == 0){
								players[i]->setSpecial(2);
								powerUps.at(j)->setLifeTime(0.f);
								al_play_sample(pick_sample, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							}
							break;

						case 3:
							players[i]->addFuel(5.f);
							powerUps.at(j)->setLifeTime(0.f);
							al_play_sample(pick_sample, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							break;

						case 4:
							if(!players[i]->isShield()){
								players[i]->setShield(true);
								powerUps.at(j)->setLifeTime(0.f);
								al_play_sample(pick_sample, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							}
							break;

						case 5:
							{
								players[i]->SetRage(0.f);
								otherEffects.push_back(new Effect(boomSmoke,players[i]->GetOriginX(), players[i]->GetOriginY(), 0, - 10, 50));
								decals.push_back(new Effect(splash, players[i]->GetOriginX() - 45, players[i]->GetOriginY()-37, 0, 0, 30));

								for (int p = 0; p < 30; p++){
									effectsList.push_back(
										new Effect(NULL,  players[i]->GetOriginX() + rand()%90 -45, players[i]->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f));
								}
								explosions.push_back(players[i]->Detonate(spawns, i));
								/*Effect* e = players[i]->Detonate(spawns, explosions.at(j)->getParentPlayerNumber());

								if (e != NULL){
									explosions.push_back(e);
									players[i]->setScore(players[i]->getScore() - 10);
								}
								*/
								//powerUps.at(j)->setLifeTime(0.f);
							}
							break;

						case 6:
							{
								for (int x = 0; x < playerNumber; x++){
									if(x != i && players[i]->getRespawn() <= 0){
										otherEffects.push_back(new Effect(boomSmoke,players[x]->GetOriginX(), players[x]->GetOriginY(), 0, - 10, 50));
										decals.push_back(new Effect(splash, players[x]->GetOriginX() - 45, players[x]->GetOriginY()-37, 0, 0, 30));
										for (int p = 0; p < 30; p++){
											effectsList.push_back(
												new Effect(NULL,  players[x]->GetOriginX() + rand()%90 -45, players[x]->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f, 2));
										}
										Effect* e = players[x]->Detonate(spawns, i);

										if (e != NULL){
											explosions.push_back(e);
											//explosions.push_back(players[x]->Detonate(spawns, i));

											players[i]->setScore(players[i]->getScore() + 10);

										}
										Effect *f = new Effect(emptyPic, players[i]->GetX(),players[i]->GetY(), 0, -20, 3);
										f->setScoreDisplay(10);
										effectsList.push_back(f);
									}
									for (int p = 0; p < 30; p++){
										effectsList.push_back(
											new Effect(NULL,  players[i]->GetOriginX() + rand()%90 -45, players[i]->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f, 2));
									}
								}
								powerUps.at(j)->setLifeTime(0.f);
								al_play_sample(evil, 1.5f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							}
							break;

						case 7:
							players[i]->setLaserTime(15.f);
							al_play_sample(pick_sample, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							powerUps.at(j)->setLifeTime(0.f);
							break;

						case 8:
							if(players[i]->getSpecial() == 0){
								players[i]->setSpecial(3);
								powerUps.at(j)->setLifeTime(0.f);
								al_play_sample(pick_sample, 1.0f, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
							}
							break;


						default:
							break;
						}
					}
				}
				//Checking station contact
				for (int j = 0; j < stations.size(); j++){				
					if(CollisionEngine::isCollidding(players[i], stations.at(j), true)){
						stations.at(j)->Refuel(players[i]);
					}
				}
			}

			if(players[i]->getRespawn() > 0 && players[i]->getRespawn() < 0.3){

				al_play_sample(spawnSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
				decals.push_back(new Effect(spawn, players[i]->GetX() - 5 , players[i]->GetY() - 5 ,0 ,0 ,1));
				players[i]->setRespawn(0);
				//players[i]->addFuel(10.f);
			}

		}


		// Rocket calculation and appling changes
		vector <Rocket*> tmpList;
		while (!rocketsList.empty())
		{
			Rocket *tmp = rocketsList.back();
			rocketsList.pop_back();
			float partDelta = 0.03f;
			if(tmp != NULL && tmp->getKind() == 1) 	
				float partDelta = 0.01f;

			int deltaAmount = (int)partDelta/deltaTime;
			if(tmp!= NULL){
				for (int z = 0; z < 2; z++){
					if(tmp->getKind() == 0)
						effectsList.push_back(new Effect(emptyPic, tmp->GetOriginX() +5, tmp->GetOriginY(), rand()%50 -25, rand()%50 -25, 0.7));
					else if(tmp->getKind() == 2)
						effectsList.push_back(new Effect(emptyPic, tmp->GetOriginX() +5, tmp->GetOriginY(), rand()%50 -25, rand()%50 -25, 1.2f));
				}

				if(tmp->getKind() == 1)
					effectsList.push_back(new Effect(emptyPic, tmp->GetOriginX() +5, tmp->GetOriginY(), tmp->GetSpeedX()/2, tmp->GetSpeedY()/2, 1.2f));
			}
			for (int dt= 0; (float)dt * partDelta <= deltaTime && tmp != NULL; dt ++){ // loop around mid distance

				if(tmp->GetY() < ScreenHeight){
					tmp->applyVecField(Forces->getFieldX(tmp->GetOriginX(), tmp->GetOriginY()), Forces->getFieldY(tmp->GetOriginX(),tmp->GetOriginY()));
				}

				tmp->Update(partDelta, gravity);

				//if rocket is faster

				if(tmp->GetY() > ScreenHeight){
					tmp = NULL;
				}


				if(tmp != NULL){
					for (int j = 0; j < playerNumber; j++)
					{
						if(players[j]->getRespawn() == 0 && CollisionEngine::isCollidding(tmp, players[j], true)){
							if(tmp->getKind() == 2){
								otherEffects.push_back(new Effect(boom2Smoke, tmp->GetOriginX(), tmp->GetOriginY(), 0, - 10, 20));
							}else{
								otherEffects.push_back(new Effect(boomSmoke, tmp->GetOriginX() , tmp->GetOriginY(), 0, - 10, 20));
							}
							for (int p = 0; p < 30; p++){
								effectsList.push_back(
									new Effect(NULL,  tmp->GetOriginX() + rand()%90 -45, tmp->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f));
							}
							explosions.push_back(tmp->Detonate());
							tmp = NULL;
							break;
						}
					}
				}


				if(tmp != NULL){
					for (int j = 0; j < BlocksList.size(); j++)
					{
						if(CollisionEngine::isCollidding(tmp, BlocksList.at(j), true)){
							if(tmp->getKind() == 2){
								otherEffects.push_back(new Effect(boom2Smoke, tmp->GetOriginX(), tmp->GetOriginY(), 0, - 10, 20));
							}else{
								otherEffects.push_back(new Effect(boomSmoke, tmp->GetOriginX(), tmp->GetOriginY(), 0, - 10, 20));
							}
							for (int p = 0; p < 30; p++){
								effectsList.push_back(
									new Effect(NULL,  tmp->GetOriginX() + rand()%90 -45, tmp->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f));
							}
							explosions.push_back(tmp->Detonate());
							tmp = NULL;
							break;
						}
					}
				}


				if(tmp != NULL){
					for (int j = 0; j < rocketsList.size(); j++)
					{
						if(rocketsList.at(j) != NULL && rocketsList.at(j)->getParentPlayerNumber() != tmp->getParentPlayerNumber()){
							if(CollisionEngine::isCollidding(tmp, rocketsList.at(j), true)){



								if(tmp->getKind() == 2){
									otherEffects.push_back(new Effect(boom2Smoke, tmp->GetOriginX(), tmp->GetOriginY(), 0, - 10, 20));
								}else{
									otherEffects.push_back(new Effect(boomSmoke, tmp->GetOriginX(), tmp->GetOriginY(), 0, - 10, 20));
								}
								for (int p = 0; p < 30; p++){
									effectsList.push_back(
										new Effect(NULL,  tmp->GetOriginX() + rand()%90 -45, tmp->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f));
								}
								explosions.push_back(tmp->Detonate());




								if(rocketsList.at(j)->getKind() == 2){
									otherEffects.push_back(new Effect(boom2Smoke, rocketsList.at(j)->GetOriginX(), rocketsList.at(j)->GetOriginY(), 0, - 10, 20));
								}else{
									otherEffects.push_back(new Effect(boomSmoke, rocketsList.at(j)->GetOriginX() , rocketsList.at(j)->GetOriginY(), 0, - 10, 20));
								}
								for (int p = 0; p < 30; p++){
									effectsList.push_back(
										new Effect(NULL,  rocketsList.at(j)->GetOriginX() + rand()%90 -45, rocketsList.at(j)->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f));
								}
								explosions.push_back(rocketsList.at(j)->Detonate());

								tmp = NULL;
								rocketsList.at(j) = NULL;
								break;
							}
						}
					}
				}
			}


			if(tmp != NULL){
				for (int j = 0; j < explosions.size(); j++){
					if(CollisionEngine::isCollidding(tmp, explosions[j], true)){
						if(tmp->getKind() == 2){
							otherEffects.push_back(new Effect(boom2Smoke, tmp->GetOriginX(), tmp->GetOriginY(), 0, - 10, 20));
						}else{
							otherEffects.push_back(new Effect(boomSmoke, tmp->GetOriginX() , tmp->GetOriginY(), 0, - 10, 20));
						}
						for (int p = 0; p < 30; p++){
							effectsList.push_back(
								new Effect(NULL,  tmp->GetOriginX() + rand()%90 -45, tmp->GetOriginY() + rand()%90 -45, rand()%90 - 45, rand()%90 - 45, 1.8f));
						}
						explosions.push_back(tmp->Detonate());
						tmp = NULL;
						break;
					}
				}

			}


			if(tmp != NULL){
				tmpList.push_back(tmp);				
			}
		}	
		rocketsList = tmpList;		



		// Random powerUps spawn
		int los = rand() % 10000;
		srand(GetTickCount64()%123123);
		//los = los% 1000;
		int powerKind = 0;
		if(los <= 200){
			if(los <= 20) powerKind = 1; // Fast rocket
			else if(los > 20 && los <= 30) powerKind = 2; // Nuke rocket
			else if(los > 30 && los <= 50) powerKind = 7; // laserTargeting
			else if(los > 50 && los <= 70) powerKind = 4; // Shield
			else if(los > 70 && los <= 80) powerKind = 8; // Bomb
			else if(los > 80 && los <= 85) powerKind = 6; // Gold scull
		}

		if(powerKind != 0){
			bool good;
			powerUp *pow = new powerUp(powerKind, 45.f);

			good = true;
			pow->SetX(rand()%ScreenWidth);

			pow->SetY(rand()%ScreenHeight);
			for(int i = 0; i < BlocksList.size(); i++){		
				if(CollisionEngine::isCollidding(pow, BlocksList.at(i),false)){
					good = false;
				}
			}
			for(int i = 0; i < powerUps.size(); i++){		
				if(CollisionEngine::isCollidding(pow, powerUps.at(i),false)){
					good = false;
				}
			}
			if(good){
				powerUps.push_back(pow);
			}
		}



		// Updating smoke etc list.
		for (int i = 0; i < otherEffects.size(); i++)
		{
			otherEffects.at(i)->UpdateEffect(deltaTime);
		}	

		// Updating smoke etc list.
		for (int i = 0; i < decals.size(); i++)
		{
			decals.at(i)->UpdateEffect(deltaTime);
		}


		// Updating powerup list
		for (int i = 0; i < powerUps.size(); i++)
		{
			powerUps.at(i)->Update(deltaTime);
		}

		// Updatign bomb list
		vector<Bomb*> tempBombs;
		for (int b = 0; b < bombs.size(); b++)
		{
			bombs.at(b)->Update(deltaTime, gravity);

			if(bombs.at(b)->GetY() < ScreenHeight){
				bombs.at(b)->applyVecField(Forces->getFieldX(bombs.at(b)->GetOriginX(), bombs.at(b)->GetOriginY()), Forces->getFieldY(bombs.at(b)->GetOriginX(),bombs.at(b)->GetOriginY()));
			}

			if(!bombs.at(b)->isSticked()){
				effectsList.push_back(new Effect(emptyPic, bombs.at(b)->GetX(), bombs.at(b)->GetY(), 0, -20, 0.30));

				for (int i = 0;!bombs.at(b)->isSticked() && i < rocketsList.size() ; i++)
				{
					if(CollisionEngine::isCollidding(bombs.at(b), rocketsList.at(i), true)){
						bombs.at(b)->Glue(rocketsList.at(i));
						break;
					}				
				}
				for (int i = 0;!bombs.at(b)->isSticked() && i < BlocksList.size() ; i++)
				{
					if(CollisionEngine::isCollidding(bombs.at(b), BlocksList.at(i), true)){
						bombs.at(b)->Glue(BlocksList.at(i));
						break;
					}				
				}
				for (int i = 0;!bombs.at(b)->isSticked() && i < playerNumber ; i++)
				{
					if(bombs.at(b)->getParentPlayerNumber() != i && CollisionEngine::isCollidding(bombs.at(b), players[i], true)){
						bombs.at(b)->Glue(players[i]);
						break;
					}				
				}

			}

			if(bombs.at(b)->isAlive())
				tempBombs.push_back(bombs.at(b));


		}		
		bombs.clear();
		bombs = tempBombs;

		// Updating station list
		for (int i = 0; i < stations.size(); i++){
			stations.at(i)->Update(deltaTime);
		}






		// Cleannig explsions;
		vector<Effect*> tmpList2;
		for (int i = 0; i < explosions.size(); i++){
			if(explosions.at(i)->isAlive())
				tmpList2.push_back(explosions.at(i));
		}
		explosions = tmpList2;


		// Cleainig powerUps list
		vector<powerUp*> tmpList3;
		for (int i = 0; i < powerUps.size(); i++){
			if(powerUps.at(i)->isAlive())
				tmpList3.push_back(powerUps.at(i));
		}
		powerUps = tmpList3;

		// Cleainig effects list
		vector<Effect*> tmpList4;
		for (int i = 0; i < effectsList.size(); i++){
			if(effectsList.at(i)->isAlive())
				tmpList4.push_back(effectsList.at(i));
		}
		effectsList = tmpList4;

		// Cleainig effects list
		vector<Effect*> tmpList5;
		for (int i = 0; i < otherEffects.size(); i++){
			if(otherEffects.at(i)->isAlive())
				tmpList5.push_back(otherEffects.at(i));
		}
		otherEffects = tmpList5;



		////////////DRAW////////////////////////////////////

		// Setting background color, and drawing background
		al_clear_to_color(al_map_rgb(100, 200, 150));
		if(!debug)
			al_draw_bitmap(background,0,0,NULL);


		// Drawing decals etc
		for (int i = 0; i < decals.size(); i++)
		{
			if(decals.at(i)->isAlive()){
				decals.at(i)->Draw();
			}
		}		

		// Drawing blocks
		for (int i = 0; i < BlocksList.size(); i++)
		{
			BlocksList.at(i)->Draw();
		}

		// Drawing Gas Station
		for (int i = 0; i < stations.size(); i++){
			stations.at(i)->Draw();
		}

		// Drawing Fire effects
		for (int i = 0; i < effectsList.size(); i++){
			effectsList.at(i)->UpdateEffect(deltaTime);
			if(effectsList.at(i)->isAlive()){
				effectsList.at(i)->DrawEffect();
			}
		}

		// Drawing players
		for (int i = 0; i < playerNumber; i++){
			if(players[i]->getRespawn() == 0){
				players[i]->DrawPlayer();
			}
		}		

		//Drawing powerups
		for (int i = 0; i < powerUps.size(); i++){
			if(powerUps.at(i)->isAlive())
				powerUps.at(i)->Draw();
		}

		// Drawing rockets
		for (int i = 0; i < rocketsList.size(); i++){
			rocketsList.at(i)->DrawRocket();
		}

		// Drawing bombs
		for (int i = 0; i < bombs.size(); i++){
			bombs.at(i)->DrawBomb();
		}

		// Drawing smoke etc
		for (int i = 0; i < otherEffects.size(); i++)
		{
			if(otherEffects.at(i)->isAlive()){
				otherEffects.at(i)->DrawSmoke();
			}
		}	

		//Drawing explosion
		for (int i = 0; i < explosions.size(); i++){
			explosions.at(i)->UpdateEffect(deltaTime);			
			if(explosions.at(i)->isAlive()){
				explosions.at(i)->Draw();
			}
		}


		//Drawing Scores
		al_draw_bitmap(barBack, ScreenWidth, 0, NULL);

		for (int i = 0; i < playerNumber; i++){
			string sscore = to_string(players[i]->getScore());
			string sfuel = to_string((int)(players[i]->getFuel()*10));
			char const *score = sscore.c_str();
			char const *fuel = sfuel.c_str();
			al_draw_bitmap(screen, ScreenWidth, i*100, NULL);

			al_draw_text(banana ,players[i]->getColor(), ScreenWidth + 30, 100*i +30, NULL, "Score:");
			al_draw_text(banana ,players[i]->getColor(), ScreenWidth + 130, 100*i +30, NULL, score);
			al_draw_text(banana ,players[i]->getColor(), ScreenWidth + 30, 100*i +48, NULL, "Fuel:");
			al_draw_text(banana ,players[i]->getColor(), ScreenWidth + 130, 100*i +48, NULL, fuel);

			switch(players[i]->getSpecial()){
			case 1:
				al_draw_circle(ScreenWidth + 150, 100*i + 39, 3, al_map_rgb(0,0,240),4);
				break;
			case 2:
				al_draw_circle(ScreenWidth + 150, 100*i + 39, 3, al_map_rgb(0,240,0),4);
				break;
			default:
				break;
			}
		}

		// drawing time left
		al_draw_filled_rectangle(5,5,(gameTime / initialGameTime)*(ScreenWidth-5), 35, al_map_rgba(0,255,0,50));


		//                   draw debugs
		ALLEGRO_COLOR orange = al_map_rgb(255, 150, 100);
		if(debug){
			al_draw_line(players[0]->GetOriginX(),players[0]->GetOriginY(),players[0]->GetOriginX() + players[0]->GetSpeedX(), players[0]->GetOriginY() + players[0]->GetSpeedY(), al_map_rgb(255,0,0), 1.f);

			al_draw_line(players[0]->GetOriginX(),players[0]->GetOriginY(),players[0]->GetOriginX() + joyStates[0]->Stick2x*100, players[0]->GetOriginY() + joyStates[0]->Stick2y*100, al_map_rgb(255,255,255), 3.f);

			//al_draw_text(font ,orange, 40,50, NULL, "FPS");
			//int fps = 1/deltaTime;
			//string sfps = to_string(fps);
			//char const *cfps = sfps.c_str();
			//al_draw_text(font ,orange, 50, 50, NULL, cfps);

			for (int i = 0; i < ScreenWidth; i+=10){
				for (int j = 0; j < ScreenHeight; j+=10){
					al_draw_line(i, j, i + Forces->getFieldX(i,j), j + Forces->getFieldY(i,j), al_map_rgb(200,200,200), 3.f);
				}
			}

		}	

		al_draw_text(font ,orange, 40,40, NULL, appName);
		al_draw_text(font ,orange, 40,49, NULL, "BETA VERSION");


		// Loading buffer to window
		al_flip_display();
		al_wait_for_vsync();
		
		//al_rest(0.001);

	}


	// Drawing scoreboard
	background = al_load_bitmap("images/scoreboards.png");
	al_draw_bitmap(background, 0, 0, NULL);

	vector<Player*> scores;

	//Sorting scores
	for (int i = 0; i < playerNumber; i++)
	{
		for (int j = i; j < playerNumber-i-1; j++)
		{
			if(players[j]->getScore() < players[j+1]->getScore()){
				Player* tmp = players[j];
				players[j] = players[j +1];
				players[j +1] = tmp;
			}
		}
	}





	ALLEGRO_FONT *scoreFont = al_load_ttf_font("fonts/chalk.ttf", 40, NULL);


	for (int i = 0; i < playerNumber; i++){
		ALLEGRO_COLOR c;
		if(i == 0)
			c = al_map_rgb(255,245,0);
		else
			c = al_map_rgb(255,255,255);

		string string = to_string(i + 1);
		char const *num = string.c_str();

		al_draw_text(scoreFont ,c, 700, 140 + 80*i, NULL, num);
		players[i]->SetX(760);
		players[i]->SetY(150 + i * 80);
		players[i]->DrawPlayer();

		string = to_string(players[i]->getScore());
		char const *score = string.c_str();

		al_draw_text(scoreFont ,c, 940, 140 + 80*i, NULL, score);

	}

	al_flip_display();

	al_rest(10);


	delete players, event;

	al_destroy_display(gameWindow);
	al_destroy_event_queue(event);

	return 0;
}

int main(){
	while(PlayTheGame() == 0);
}
