#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "animation.h"
#include "ModuleAudio.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:

	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void MapShape(); 
	void MapShape2(); 
	void BallReset(); 
	void FontV(); 
	void FontBlit(); 
	void FontBlit2();
	void FontBlitLastScore();
	void FontBlitHiScore();

public:

	// Lists of physics objects
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	PhysBody* Ball;
	PhysBody* Right;
	PhysBody* Right1;
	PhysBody* Left;
	PhysBody* Left1;
	b2RevoluteJointDef RevoluteJointDefRight;
	b2RevoluteJointDef RevoluteJointDefLeft;

	b2Body* BodyB1;
	b2Body* BodyB2;
	b2Body* BodyB3;

	SDL_Rect numbers[10];
	uint score = 00000;
	uint score2 = 00000;
	uint LastScore = 00000;
	uint HiScore = 00000;

	PhysBody* BallPush;
	PhysBody* BallPushS;
	b2PrismaticJointDef JointLauncher;

	bool MapShapeB = false;
	bool Swap = false;
	bool Swap2 = true;
	bool GroundLevel = true;
	bool Reset = false;
	bool Block = false;
	bool Square = false;
	bool Door = false;

	bool Pokeball = false;
	bool Flipper = false;
	bool Bumper = false;
	bool Shot = false;
	bool Pokemon = false;
	bool Points = false;
	bool Pikachu = false;
	bool Wailmer = false;


	int BallsNumber = 3;
	PhysBody* Sensor;
	PhysBody* Sensor1;
	PhysBody* SensorReset;

	PhysBody* PokeballSensor;
	PhysBody* WailmerSensor;
	PhysBody* PikachuSensor;

	PhysBody* PointsSensor1;
	PhysBody* PointsSensor2;
	PhysBody* PointsSensor3;
	PhysBody* PointsSensor4;
	PhysBody* PointsSensor5;
	PhysBody* PointsSensor6;

	PhysBody* BlockSensor;
	PhysBody* SquareSensor;
	PhysBody* DoorSensor;

	bool SensorUp;

	SDL_Texture* Background;
	SDL_Texture* Replay;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* Loop;
	SDL_Texture* FontTex;
	SDL_Texture* MapSprites;
	SDL_Texture* Win;

	uint BonusSFX;
	uint FlipperS;
	uint BumperS;
	uint ShootS;
	uint PokeballS;
	uint PokemonS;
	uint PointS;
	uint wailmerS;
	uint pikachuS;
	uint BGSound;

	p2Point<int> ray;
	bool ray_on;


	SDL_Texture* Sprites = nullptr;
	Animation PikachuA;
	Animation ZigzagoonA;
	Animation SpoinkUp;
	Animation SpoinkDown;
	Animation PelipperA;
	Animation SeedotA;
	Animation BallA;
	Animation FlipperRighA;
	Animation FlipperLeftA;
	Animation Shroomish1A;
	Animation Shroomish2A;
	Animation Shroomish3A;

	Animation PikachuDoorA;
	Animation PikachuDoorB;
	Animation SquareA;

	Animation WailmerA;
	Animation MinumA;
	Animation PlusleA;
	Animation LightA;

	Animation HouseA;
	Animation TvA;
	Animation EggA;
	Animation BallCount;

};

