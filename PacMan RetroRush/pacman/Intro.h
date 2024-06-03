#pragma once
#include <raylib.h>
#include "Player.h"
#include "Enemys.h"
#include "TileMap.h"
#include "Object.h"
#include "Text.h"
class Intro 
{
public:
	Intro();
	~Intro();

	AppStatus Init();
	void Update();
	void Render();
	void Release();
	bool checking = false;
	

private:
	AppStatus LoadIntro();
	void CheckCollisions();

	int timming = 0;
	Jugador* pacman;
	ENEMIGO* blinky;
	ENEMIGO* pinky;
	ENEMIGO* clyde;
	ENEMIGO* inky;
	std::vector<OBJETOS*> pill1;
	int playerX, playerY;
	int pill1X, pill2Y;
	Camera2D camera;
	TileMap* introScene;
	bool turn = false;
	bool end = true;
	bool loop = false;

	bool isPinky = false;
	bool isInky = false;
	bool isBlinky = false;
	bool isClyde = false;
	bool isPill1 = true;

};