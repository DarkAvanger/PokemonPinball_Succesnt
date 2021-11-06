#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "animation.h"
#include <math.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	// Initialise all the internal class variables, at least to NULL pointer
	circle = box = rick = NULL;
	ray_on = false;
	SensorUp = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	// Set camera position
	App->renderer->camera.x = App->renderer->camera.y = 0;

	FlipperS = App->audio->LoadFx("pinball/Saver.wav");
	BumperS = App->audio->LoadFx("pinball/bonus.wav");
	ShootS = App->audio->LoadFx("pinball/Saver.wav");
	PokeballS = App->audio->LoadFx("pinball/Saver.wav");
	PokemonS = App->audio->LoadFx("pinball/Do.wav");
	PointS = App->audio->LoadFx("pinball/Pling.wav");
	BonusSFX = App->audio->LoadFx("pinball/bonus.wav");
	Replay = App->textures->Load("pinball/map_end.png");
	Background = App->textures->Load("pinball/map.png");
	FontTex = App->textures->Load("pinball/font.png");
	Sprites = App->textures->Load("pinball/pokemons.png");
	MapSprites = App->textures->Load("pinball/mapfix.png");
	wailmerS = App->audio->LoadFx("pinball/wailmer.wav");
	pikachuS = App->audio->LoadFx("pinball/pikachu.wav");
	BGSound = App->audio->LoadFx("pinball/BlueSong.wav");
	Win = App->textures->Load("pinball/map_Win.png");
	
	App->audio->PlayFx(BGSound, -1);

	FontV();

	SquareSensor = App->physics->CreateRectangleSensor(218, 170, 5, 5); //SensorPikaSquare
	WailmerSensor = App->physics->CreateRectangleSensor(187, 190, 5, 5); //Sensor Wailmer
	PikachuSensor = App->physics->CreateRectangleSensor(32, 383, 5, 5); //Sensor Pikachu

	Sensor = App->physics->CreateRectangleSensor(40, 220, 10, 10); //Loop
	Sensor1 = App->physics->CreateRectangleSensor(242, 320, 5, 5);

 //Need Position
	PointsSensor1 = App->physics->CreateRectangleSensor(153, 182, 6, 6); //Points
	PointsSensor2 = App->physics->CreateRectangleSensor(188, 313, 6, 6); //Points
	PointsSensor3 = App->physics->CreateRectangleSensor(52, 313, 6, 6); //Points
	PointsSensor4 = App->physics->CreateRectangleSensor(62, 218, 5, 5); //Points
	PointsSensor5 = App->physics->CreateRectangleSensor(94, 200, 5, 5); //Points
	PointsSensor6 = App->physics->CreateRectangleSensor(70, 195, 5, 5); //Points
	
	SensorReset = App->physics->CreateRectangleSensor(100, 450, 200, 20);

	MapShape();

	Ball = App->physics->CreateCircle(242, 350, 6); //Ball
	
	Right = App->physics->CreateRectangle(100, 400, 27, 6);

	Right1 = App->physics->CreateCircle(155, 400, 2);
	Right1->body->SetType(b2_staticBody);

	RevoluteJointDefRight.bodyA = Right->body;
	RevoluteJointDefRight.bodyB = Right1->body;
	RevoluteJointDefRight.referenceAngle = 0 * DEGTORAD;
	RevoluteJointDefRight.enableLimit = true;
	RevoluteJointDefRight.lowerAngle = -30 * DEGTORAD;
	RevoluteJointDefRight.upperAngle = 30 * DEGTORAD;
	RevoluteJointDefRight.localAnchorA.Set(PIXEL_TO_METERS(13), 0);
	RevoluteJointDefRight.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_right = (b2RevoluteJoint*)App->physics->World->CreateJoint(&RevoluteJointDefRight);

	Left = App->physics->CreateRectangle(100, 400, 27, 7);

	Left1 = App->physics->CreateCircle(90, 400, 2);
	Left1->body->SetType(b2_staticBody);

	RevoluteJointDefLeft.bodyA = Left->body;
	RevoluteJointDefLeft.bodyB = Left1->body;
	RevoluteJointDefLeft.referenceAngle = 0 * DEGTORAD;
	RevoluteJointDefLeft.enableLimit = true;
	RevoluteJointDefLeft.lowerAngle = -30 * DEGTORAD;
	RevoluteJointDefLeft.upperAngle = 30 * DEGTORAD;
	RevoluteJointDefLeft.localAnchorA.Set(PIXEL_TO_METERS(-13), 0);
	RevoluteJointDefLeft.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_left = (b2RevoluteJoint*)App->physics->World->CreateJoint(&RevoluteJointDefLeft);
	
	//1
	b2BodyDef bumper1;
	bumper1.type = b2_kinematicBody; 
	bumper1.position.Set(PIXEL_TO_METERS(130), PIXEL_TO_METERS(127)); 

	BodyB1 = App->physics->World->CreateBody(&bumper1);
	b2CircleShape Sbumper1;
	Sbumper1.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); 
	Sbumper1.m_radius = PIXEL_TO_METERS(5); 
	b2FixtureDef Fbumper1;
	Fbumper1.shape = &Sbumper1; 
	Fbumper1.restitution = 1, 1;
	BodyB1->CreateFixture(&Fbumper1); 

	//2
	b2BodyDef bumper2;
	bumper2.type = b2_kinematicBody;
	bumper2.position.Set(PIXEL_TO_METERS(160), PIXEL_TO_METERS(127)); 

	BodyB2 = App->physics->World->CreateBody(&bumper2);
	b2CircleShape Sbumper2;
	Sbumper2.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); 
	Sbumper2.m_radius = PIXEL_TO_METERS(5); 
	b2FixtureDef Fbumper2;
	Fbumper2.shape = &Sbumper2;
	Fbumper2.restitution = 1, 1;
	BodyB2->CreateFixture(&Fbumper2);



	 //3
	b2BodyDef bumper3;
	bumper3.type = b2_kinematicBody; 
	bumper3.position.Set(PIXEL_TO_METERS(145), PIXEL_TO_METERS(152)); 

	BodyB3 = App->physics->World->CreateBody(&bumper3);
	b2CircleShape Sbumper3;
	Sbumper3.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); 
	Sbumper3.m_radius = PIXEL_TO_METERS(5); 
	b2FixtureDef Fbumper3;
	Fbumper3.shape = &Sbumper3;
	Fbumper3.restitution = 1, 1;
	BodyB3->CreateFixture(&Fbumper3); 

	

	//Shoot1 
	
	b2BodyDef Shoot1;
	Shoot1.type = b2_staticBody; 
	Shoot1.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));

	b2Body* BodyShoot1 = App->physics->World->CreateBody(&Shoot1);
	b2Vec2 Vertice1[3];
	Vertice1[0].Set(PIXEL_TO_METERS(1), PIXEL_TO_METERS(20));
	Vertice1[1].Set(PIXEL_TO_METERS(1), PIXEL_TO_METERS(43));
	Vertice1[2].Set(PIXEL_TO_METERS(-22), PIXEL_TO_METERS(57));

	
	b2PolygonShape ShapeShoot1;
	ShapeShoot1.Set(Vertice1, 3); 
	b2FixtureDef FShoot1;
	FShoot1.shape = &ShapeShoot1; 
	Shoot1.position.Set(PIXEL_TO_METERS(173), PIXEL_TO_METERS(318));
	b2Body* dynamicBody2_l = App->physics->World->CreateBody(&Shoot1);
	FShoot1.restitution = 1.5f;
	dynamicBody2_l->CreateFixture(&FShoot1); 
	
	//Shoot2
	
	b2BodyDef Shoot2;
	Shoot2.type = b2_staticBody; 
	Shoot2.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); 

	b2Body* BodyShoot2 = App->physics->World->CreateBody(&Shoot2);
	b2Vec2 Vertice2[3];
	Vertice2[0].Set(PIXEL_TO_METERS(-1), PIXEL_TO_METERS(20));
	Vertice2[1].Set(PIXEL_TO_METERS(-1), PIXEL_TO_METERS(43));
	Vertice2[2].Set(PIXEL_TO_METERS(22), PIXEL_TO_METERS(57));


	b2PolygonShape ShapeShoot2;
	ShapeShoot2.Set(Vertice2, 3); 
	b2FixtureDef FShoot2;
	FShoot2.shape = &ShapeShoot2; 
	Shoot2.position.Set(PIXEL_TO_METERS(66), PIXEL_TO_METERS(318)); 
	b2Body* DBody2 = App->physics->World->CreateBody(&Shoot2);
	FShoot2.restitution = 1.5f;
	DBody2->CreateFixture(&FShoot2); 
	
	BallPush = App->physics->CreateRectangle(243, 220, 15, 5); //before 400 now 200 idk but it works
	BallPushS = App->physics->CreateRectangle(243, 404, 15, 5);
	BallPushS->body->SetType(b2_staticBody);

	JointLauncher.bodyB = BallPush->body;
	JointLauncher.bodyA = BallPushS->body;
	JointLauncher.collideConnected = false;
	JointLauncher.enableLimit = true;
	JointLauncher.lowerTranslation = PIXEL_TO_METERS(25);
	JointLauncher.upperTranslation = PIXEL_TO_METERS(40);
	JointLauncher.localAnchorA.Set(0, 0);
	JointLauncher.localAnchorB.Set(0, 0);
	JointLauncher.localAxisA.Set(0, -1);

	b2PrismaticJoint* joint_launcher = (b2PrismaticJoint*)App->physics->World->CreateJoint(&JointLauncher);
	

	//Animations

	PikachuA.PushBack({ 46,21,25,24 });
	PikachuA.PushBack({ 72,21,26,28 });
	PikachuA.speed = 0.04f;

	BallA.PushBack({ 106,119,16,16 });
	
	SeedotA.PushBack({ 148,83,21,42 });
	SeedotA.PushBack({ 173,82,21,44 });
	SeedotA.speed = 0.04f;

	ZigzagoonA.PushBack({ 160,15,42,34 });
	ZigzagoonA.PushBack({ 206,15,42,37 });
	ZigzagoonA.speed = 0.01f;

	SpoinkUp.PushBack({ 5,57,22,42 });
	SpoinkUp.PushBack({ 33,58,22,42 });
	SpoinkUp.speed = 0.04f;

	SpoinkDown.PushBack({ 58,64,21,36 });
	SpoinkDown.PushBack({ 83,64,21,36 });
	SpoinkDown.PushBack({ 108,64,21,36 });
	SpoinkDown.PushBack({ 83,64,21,36 });
	SpoinkDown.speed = 0.2f;

	PelipperA.PushBack({ 1,2,42,45 });
	PelipperA.PushBack({ 200,78,40,48 });
	PelipperA.speed = 0.01f;

	PikachuDoorA.PushBack({ 118,147,17,18 });
	PikachuDoorA.PushBack({ 138,147,17,18 });
	PikachuDoorA.PushBack({ 162,147,17,18 });
	PikachuDoorA.PushBack({ 138,147,17,18 });
	PikachuDoorA.speed = 0.1;

	PikachuDoorB.PushBack({ 118,147,17,18 });

	Shroomish1A.PushBack({ 100,20,26,26 });
	Shroomish1A.PushBack({ 130,20,26,26 });
	Shroomish1A.speed = 0.04f;
	
	Shroomish2A.PushBack({ 100,20,26,26 });
	Shroomish2A.PushBack({ 130,20,26,26 });
	Shroomish2A.speed = 0.04f;
	
	Shroomish3A.PushBack({ 100,20,26,26 });
	Shroomish3A.PushBack({ 130,20,26,26 });
	Shroomish3A.speed = 0.04f;

	FlipperRighA.PushBack({ 54,113,34,26 });
	FlipperLeftA.PushBack({ 10,113,34,26 });

	WailmerA.PushBack({ 10,150,40,38 });
	WailmerA.PushBack({ 55,150,40,38 });
	WailmerA.speed = 0.01f;

	PlusleA.PushBack({ 190,130,30,45 });
	PlusleA.PushBack({ 220,130,30,45 });
	PlusleA.speed = 0.01f;

	MinumA.PushBack({ 190,175,30,45 });
	MinumA.PushBack({ 220,175,30,45 });
    MinumA.speed = 0.01f;

	LightA.PushBack({ 95,170,17,40 });
	LightA.PushBack({ 115,170,17,40 });
	LightA.PushBack({ 135,170,17,26 });
	LightA.speed = 0.04f;

	EggA.PushBack({ 22,120,20,20 });
	EggA.PushBack({ 40,120,20,20 });
	EggA.speed = 0.01f;

	HouseA.PushBack({ 0,0,100,100 });

	TvA.PushBack({ 100,0,70,100 });

	BallCount.PushBack({ 20,190,20,20 });

	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::Update()
{
	static int PikachuX = 20;

	if (BallsNumber > 0) {

		//CONTROLS
		App->renderer->Blit(Background, 0, 0);

		static int Power = 0;
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			Power += 30;
			if (Power > 300)
				Power = 300;

		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
			BallPush->body->ApplyForceToCenter(b2Vec2(0, -Power), 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			b2Vec2 force = b2Vec2(0, -200);
			Right->body->ApplyForceToCenter(force, 1);
			RevoluteJointDefRight.lowerAngle = 30 * DEGTORAD;
			PikachuX = 198;
			PikachuSensor->body->SetTransform(b2Vec2(PIXEL_TO_METERS(209), PIXEL_TO_METERS(383)), 0);

		}

		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
			Ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())), 0);
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			b2Vec2 force = b2Vec2(0, -200);
			Left->body->ApplyForceToCenter(force, 1);
			RevoluteJointDefLeft.lowerAngle = 30 * DEGTORAD;
			PikachuX = 20;
			PikachuSensor->body->SetTransform(b2Vec2(PIXEL_TO_METERS(32), PIXEL_TO_METERS(383)), 0);
		}


		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
			static bool temp = false;
			if (temp == true)
				MapShape();

			else
				MapShape2();
			temp = !temp;
		}
		if (score2 >= 10000) {
			BallsNumber++;
			score2 = 0;
		}

		
	}

	//Map Sensor
	
	if (Swap == true) {
		if (Ball->body->GetLinearVelocity().y < 1 || Ball->body->GetLinearVelocity().y > 1.9) {
			if (MapShapeB == true) {
				MapShape();
				MapShapeB = !MapShapeB;
				Swap = !Swap;
				GroundLevel = false;
			}
			else {
				MapShape2();
				MapShapeB = !MapShapeB;
				Swap = !Swap;
				GroundLevel = true;
			}
		}
	}
	static int PikachuC = 0;
	//Reset
	if (Reset == true) {
		BallReset();
		Reset = false;
		Block = false;
		PikachuC = 0;
		if (Swap2 == true) {
			MapShape();
			MapShapeB = !MapShapeB;
			Swap2 = !Swap2;
			GroundLevel = false;
		}
	}
	if (BallsNumber >= 3) {
		App->renderer->Blit(Sprites, 92, 338.5f, &(BallCount.GetCurrentFrame()), 0.01f);
		App->renderer->Blit(Sprites, 108, 338.5f, &(BallCount.GetCurrentFrame()), 0.01f);
		App->renderer->Blit(Sprites, 125, 338.5f, &(BallCount.GetCurrentFrame()), 0.01f);
	}
	else if (BallsNumber == 2) {
		App->renderer->Blit(Sprites, 92, 338.5f, &(BallCount.GetCurrentFrame()), 0.01f);
		App->renderer->Blit(Sprites, 108, 338.5f, &(BallCount.GetCurrentFrame()), 0.01f);
	}
	else if (BallsNumber == 1) {
		App->renderer->Blit(Sprites, 92, 338.5f, &(BallCount.GetCurrentFrame()), 0.01f);
	}
	//Sound


	if (Points == true) {
		Points = false;
		App->audio->PlayFx(PointS);
		score += 300;
		score2 += 300;
	}
	if (Pokemon == true) {
		Pokemon = false;
		App->audio->PlayFx(PokemonS);
		score += 100;
		score2 += 100;
	}
	if (Flipper == true) {
		Flipper = false;
		App->audio->PlayFx(FlipperS);
	}

	if (Bumper == true) {
		Bumper = false;
		App->audio->PlayFx(BumperS);
		score += 100;
		score2 += 100;
	}
	
	if (Pokeball == true) {
		Pokeball = false;
		App->audio->PlayFx(PokeballS);

	}
	//Pokemon

	static int  PikachuT = 0;
	
	if (PikachuC < 2) {
		if (Pikachu == true) {
			b2Vec2 vel = Ball->body->GetLinearVelocity();
			if (PikachuT < 200) {

				Ball->body->SetLinearVelocity(b2Vec2(0, 0));
			}
			if (PikachuT > 200) {
				Ball->body->SetLinearVelocity(b2Vec2(0, -100));
				Pikachu = false;
				PikachuT = 0;
				PikachuC++;
				App->audio-> PlayFx(pikachuS);
				score += 1000;
				score2 += 1000;
			}
			PikachuT++;
		}
	}

	static int WailmerT = 0;
	if (Wailmer == true) {
		if (WailmerT < 200) {
			Ball->body->SetTransform(b2Vec2(500, 500), 0);
		}
		if (WailmerT > 200) {
			Ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(190), PIXEL_TO_METERS(200)), 0);
			Ball->body->SetLinearVelocity(b2Vec2(0, 10));
			WailmerT = 0;
			Wailmer = false;
			score += 1000;
			score2 += 1000;
			App->audio->PlayFx(wailmerS);
		}
		WailmerT++;
	}

	
	//BUMPER
	static uint time = 0;
	time++;
	float posx = cos((time + 0) / 45.0) * 20;
	float posy = sin((time + 0) / 45.0) * 20;
	b2Vec2 pos = b2Vec2(PIXEL_TO_METERS(posx + PIXEL_TO_METERS(103)), PIXEL_TO_METERS(posy + PIXEL_TO_METERS(130)));

	BodyB1->SetTransform(pos, 0);
	App->renderer->Blit(Sprites, posx + 103, posy + 133, &Shroomish1A.GetCurrentFrame(), 0.01f);

	posx = cos((time + 90) / 45.0) * 20;
	posy = sin((time + 90) / 45.0) * 20;
	pos = b2Vec2(PIXEL_TO_METERS(posx + PIXEL_TO_METERS(103)), PIXEL_TO_METERS(posy + PIXEL_TO_METERS(130)));

	BodyB2->SetTransform(pos, 0);
	App->renderer->Blit(Sprites, posx + 103, posy + 133, &Shroomish2A.GetCurrentFrame(), 0.01f);
	
	posx = cos((time + 180) / 45.0) * 20;
	posy = sin((time + 180) / 45.0) * 20;
	pos = b2Vec2(PIXEL_TO_METERS(posx + PIXEL_TO_METERS(103)), PIXEL_TO_METERS(posy + PIXEL_TO_METERS(130)));
	
	BodyB3->SetTransform(pos, 0);
	App->renderer->Blit(Sprites, posx + 103, posy + 133, &Shroomish3A.GetCurrentFrame(), 0.01f);
	
	//SpriteBlit

	App->renderer->Blit(Sprites, Ball->body->GetPosition().x * 25 - 7, Ball->body->GetPosition().y * 25 - 7, &(BallA.GetCurrentFrame()));

	App->renderer->Blit(Sprites, PikachuX, 380, &(PikachuA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(Sprites, 192, 285, &(ZigzagoonA.GetCurrentFrame()), 0.01f);;
	App->renderer->Blit(Sprites, 155, 115, &(PelipperA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(Sprites, 20, 275, &(SeedotA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(Sprites, 168, 163, &(WailmerA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(MapSprites, -19, 88, &(TvA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(Sprites, 68, 147, &(MinumA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(Sprites, 35, 163, &(PlusleA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(Sprites, 62, 172, &(LightA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(MapSprites, 158, 15, &(HouseA.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(MapSprites, 199, 35, &(EggA.GetCurrentFrame()), 0.01f);

	static int temp = 0;
	if (temp < 170 && Square == true)
	{
		App->renderer->Blit(Sprites, 210, 160, &(PikachuDoorA.GetCurrentFrame()), 0.01f);
	}

	if (Square == false)
	{
		App->renderer->Blit(Sprites, 210, 160, &(PikachuDoorB.GetCurrentFrame()), 0.00f);
	}

	if (temp > 170)
	{
		Square = false;
		temp = 0;
	}
	temp++;

	// Prepare for raycast ------------------------------------------------------
	
	// The target point of the raycast is the mouse current position (will change over game time)
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// Total distance of the raycast reference segment
	int ray_hit = ray.DistanceTo(mouse);

	// Declare a vector. We will draw the normal to the hit surface (if we hit something)
	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if (ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if (hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	// Raycasts -----------------
	if(ray_on == true)
	{
		// Compute the vector from the raycast origin up to the contact point (if we're hitting anything; otherwise this is the reference length)
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		// Draw a line from origin to the hit point (or reference length if we are not hitting anything)
		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		// If we are hitting something with the raycast, draw the normal vector to the contact point
		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	//Pokemon

	if (App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT)
	{
		App->renderer->Blit(Sprites, 232, 375, &(SpoinkUp.GetCurrentFrame()), 0.01f);
	}
	else
	{
		App->renderer->Blit(Sprites, 232, 381, &(SpoinkDown.GetCurrentFrame()), 0.01f);
	}
	
	//Font Blit

	FontBlit();
	//Blit right and left;
	float32 RightAngle = Right->body->GetAngle();
	float32 LeftAngle = Left->body->GetAngle();

	App->renderer->Blit(Sprites, 126, 394, &FlipperRighA.GetCurrentFrame(), 0.00f, RADTODEG * (RightAngle) + 30, 55, 10);
	App->renderer->Blit(Sprites, 83, 394, &FlipperLeftA.GetCurrentFrame(), 0.00f, RADTODEG * (LeftAngle) - 30, 5, 10);

	if (BallsNumber <= 0) {
		if (HiScore < score) {
			HiScore = score;

			
		}
		else {
			HiScore = HiScore;
			
		
		}

		App->renderer->Blit(Replay, 0, 0);
		FontBlit2();
		FontBlitLastScore();
		FontBlitHiScore();
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
			BallsNumber = 3;
		
			LastScore = score;

			score = 0;
			
		}
	}
	
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		score += 5000;
	}

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		BallsNumber -= 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN && BallsNumber <3) {
		BallsNumber += 1;
	}

	if (score >= 20000) {

		App->renderer->Blit(Win, 0, 0);
		App->scene_intro->BallReset();
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
			BallsNumber = 3;

			LastScore = score;

			score = 0;

		}
	}

	// Keep playing
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(BonusSFX);
	static bool temp = true;
}

void ModuleSceneIntro::MapShape()
{
	p2List_item<PhysBody*>* temp = ricks.getFirst();
	while (temp != NULL) {
		b2Body* body = temp->data->body;
		temp->data->body->GetWorld()->DestroyBody(body);
		temp = temp->next;
	}
	ricks.clear();
	//ShootLVL

	int map[104] = {
	60, 264,
	43, 243,
	26, 212,
	20, 193,
	15, 178,
	11, 148,
	15, 117,
	20, 97,
	27, 84,
	44, 66,
	55, 58,
	73, 49,
	86, 45,
	95, 41,
	111, 35,
	141, 28,
	167, 25,
	222, 26,
	232, 30,
	239, 34,
	244, 41,
	248, 48,
	249, 59,
	249, 103,
	251, 417,
	235, 417,
	235, 220,
	235, 169,
	233, 159,
	232, 137,
	232, 102,
	232, 64,
	230, 54,
	222, 48,
	213, 47,
	169, 43,
	144, 45,
	125, 50,
	107, 59,
	96, 62,
	80, 65,
	69, 69,
	60, 73,
	48, 83,
	40, 93,
	34, 107,
	31, 128,
	30, 149,
	30, 166,
	36, 187,
	55, 227,
	79, 242
	};

	ricks.add(App->physics->CreateChain(0, 0, map, 104));
}

void ModuleSceneIntro::MapShape2() //Should be fixed, needs test
{
	p2List_item<PhysBody*>* asd = ricks.getFirst();
	while (asd != NULL) {
		b2Body* body = asd->data->body;
		asd->data->body->GetWorld()->DestroyBody(body);
		asd = asd->next;
	}
	ricks.clear();

	int map[104] = {
	100, 471,
	100, 432,
	100, 427,
	40, 394,
	30, 390,
	23, 387,
	23, 339,
	26, 331,
	31, 329,
	42, 328,
	46, 324,
	48, 294,
	39, 284,
	30, 271,
	22, 253,
	17, 239,
	13, 224,
	12, 206,
	12, 190,
	13, 174,
	18, 149,
	30, 128,
	46, 106,
	71, 91,
	94, 83,
	120, 80,
	144, 83,
	168, 90,
	194, 107,
	209, 123,
	221, 147,
	226, 165,
	228, 211,
	226, 230,
	221, 246,
	214, 263,
	205, 278,
	199, 286,
	192, 292,
	193, 323,
	195, 327,
	200, 329,
	208, 329,
	213, 331,
	217, 338,
	216, 387,
	208, 390,
	200, 393,
	140, 427,
	140, 432,
	140, 472,
	106, 472
	};

	int obstacle1[58] = {//TV Obstacle
	52, 221,
	45, 212,
	42, 205,
	38, 196,
	25, 210,
	33, 175,
	34, 163,
	39, 152,
	47, 140,
	58, 125,
	71, 111,
	81, 106,
	81, 130,
	76, 133,
	72, 139,
	74, 145,
	81, 160,
	87, 173,
	96, 184,
	96, 196,
	87, 202,
	66, 184,
	61, 182,
	55, 183,
	51, 187,
	55, 194,
	64, 202,
	64, 214,
	52, 222
	};
	
	int obstacle2[108] = { //Yellow Island
	157, 184,
	160, 183,
	163, 183,
	164, 188,
	163, 192,
	162, 197,
	158, 199,
	155, 201,
	154, 218,
	157, 219,
	161, 217,
	163, 214,
	163, 210,
	167, 207,
	171, 199,
	176, 185,
	177, 180,
	182, 177,
	183, 173,
	190, 171,
	198, 172,
	202, 176,
	206, 183,
	205, 190,
	199, 194,
	193, 199,
	191, 204,
	186, 208,
	184, 217,
	189, 219,
	204, 218,
	210, 212,
	212, 205,
	212, 193,
	211, 176,
	209, 165,
	203, 155,
	195, 139,
	187, 123,
	174, 111,
	164, 106,
	144, 102,
	139, 102,
	137, 111,
	139, 117,
	142, 120,
	148, 121,
	161, 126,
	163, 133,
	162, 142,
	148, 157,
	141, 167,
	142, 182,
	147, 178
	};

	int obstacle3[18] = { //greyLeftObstacle
	95, 102,
	102, 102,
	102, 120,
	100, 121,
	100, 124,
	97, 124,
	97, 121,
	95, 120,
	95, 106
	};

	int obstacle20[6] = { //BAJA MEN QUE TE PARTO
	44, 247,
    27, 210,
	34,240 
	};

	int obstacle4[18] = { 
	116, 100,
	123, 100,
	123, 118,
	122, 119,
	122, 122,
	118, 122,
	118, 120,
	116, 118,
	116, 105
	};

	int obstacle7[20] = { //blueObstacleLeft
	44, 342,
	48, 342,
	48, 368,
	50, 373,
	55, 376,
	86, 395,
	81, 400,
	46, 379,
	44, 375,
	44, 345
	};

	int obstacle8[18] = {//blueObstacleRight
	191, 342,
	196, 342,
	196, 374,
	157, 401,
	153, 396,
	185, 375,
	189, 372,
	191, 368,
	191, 345
	};

	ricks.add(App->physics->CreateChain(0, 0, map, 104));
	ricks.add(App->physics->CreateChain(0, 0, obstacle1, 58));
	ricks.add(App->physics->CreateChain(0, 0, obstacle2, 108));
	ricks.add(App->physics->CreateChain(0, 0, obstacle3, 18));
	ricks.add(App->physics->CreateChain(0, 0, obstacle4, 18));
	ricks.add(App->physics->CreateChain(0, 0, obstacle7, 20));
	ricks.add(App->physics->CreateChain(0, 0, obstacle8, 18));
	ricks.add(App->physics->CreateChain(0, 0, obstacle20, 6));

}

void ModuleSceneIntro::BallReset()
{
	Ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(243), PIXEL_TO_METERS(350)), 0);
	Ball->body->SetAngularVelocity(0);
	Ball->body->SetLinearVelocity(b2Vec2(0, 0));
}

void ModuleSceneIntro::FontV()
{
	for (int x = 0; x < 10; x++) {
		numbers[x] = { x * 13 + 3, 3, 13,24 };
	}
}
// Move text X positon
void ModuleSceneIntro::FontBlit()
{
	int score_temp = score;
	for (int i = 6; i >= 0; i--) {

		int temp = score_temp % 10;
		App->renderer->Blit(FontTex, i * 13 + 164, 1, &numbers[temp]);
		score_temp = score_temp / 10;
	}

}

void ModuleSceneIntro::FontBlit2()
{
	int score_temp = score;
	for (int i = 6; i >= 0; i--) {

		int temp = score_temp % 10;
		App->renderer->Blit(FontTex, i * 13 + 115, 155, &numbers[temp]);
		score_temp = score_temp / 10;
	}
}

void ModuleSceneIntro::FontBlitLastScore()
{

	int score_temp = LastScore;
	for (int i = 6; i >= 0; i--) {

		int temp = score_temp % 10;
		App->renderer->Blit(FontTex, i * 13 + 115, 186, &numbers[temp]);
		score_temp = score_temp / 10;
	}
}

void ModuleSceneIntro::FontBlitHiScore()
{
	int score_temp = HiScore;
	for (int i = 6; i >= 0; i--) {

		int temp = score_temp % 10;
		App->renderer->Blit(FontTex, i * 13 + 115, 217, &numbers[temp]);
		score_temp = score_temp / 10;
	}
}