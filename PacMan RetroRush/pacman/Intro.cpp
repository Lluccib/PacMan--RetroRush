#include "Intro.h"
#include <stdio.h>
#include "Globals.h"

Intro::Intro() 
{
	PacMan = nullptr;
	Blinky = nullptr;
	Pinky = nullptr;
	Inky = nullptr;
	Clyde = nullptr;
	introScene = nullptr;

	camera.target = { 0, 0 };				
	camera.offset = { 0, 0 };	
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
}

Intro::~Intro()
{
	if (PacMan != nullptr) {
		PacMan->Release();
		delete PacMan;
		PacMan = nullptr;
	}
	if (Blinky != nullptr) {
		Blinky->Release();
		delete Blinky;
		Blinky = nullptr;
	}
	if (Inky != nullptr) {
		Inky->Release();
		delete Inky;
		Inky = nullptr;
	}
	if (Pinky != nullptr) {
		Pinky->Release();
		delete Pinky;
		Pinky = nullptr;
	}
	if (Clyde != nullptr) {
		Clyde->Release();
		delete Clyde;
		Clyde = nullptr;
	}
	for (Entity* obj : pellet)
	{
		delete obj;
	}
	pellet.clear();
	if (introScene != nullptr) {
		introScene->Release();
		delete introScene;
		introScene = nullptr;
	}
}

AppStatus Intro::Init() 
{
	PacMan = new Jugador({ 0,0 }, State::QUIETO, Vista::IZQUIERDA);
	if (PacMan == nullptr)
	{
		LOG("Failed to allocate memory for intro PacMan");
		return AppStatus::ERROR;
	}
	Inky = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::ROJO);
	if (Inky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	Blinky = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::AZULITO);
	if (Blinky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	Pinky = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::ROSA);
	if (Pinky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	Clyde = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::NARANJITA);
	if (Clyde == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}

	if(PacMan->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro pacman");
		return AppStatus::ERROR;
	}
	if (Inky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (Blinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (Clyde->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (Pinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}

	introScene = new TileMap();
	if (introScene == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	if (introScene->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	if (LoadIntro() != AppStatus::OK)
	{
		LOG("Failed to load intro");
		return AppStatus::ERROR;
	}
	
	PacMan->SetTileMap(introScene);

	return AppStatus::OK;
}

AppStatus Intro::LoadIntro() 
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	OBJETOS* obj;
	int* map = nullptr;
	size = LEVEL_WIDTH * LEVEL_HEIGHT;

	map = new int[size] {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE / 2;
				playerX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				playerY = pos.y;
				PacMan->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::PELLET)
			{
				pos.x = x * TILE_SIZE;
				dotX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				dotY = pos.y;
				obj = new OBJETOS(pos, ObjectType::PILL2);
				pellet.push_back(obj);
				map[i] = 0;
			}
			++i;
		}
	}
	
	introScene->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] map;
	return AppStatus::OK;
}

void Intro::Update() 
{

	if (end) {
		turn = false;
		PacMan->SetPos({ playerX, playerY });
		OBJETOS* obj = new OBJETOS({ dotX, dotY }, ObjectType::PILL2);
		pellet.push_back(obj);
		isDot = true;

		Blinky->pilladointro = false;
		isBlinky = false;
		Pinky->pilladointro = false;
		isPinky = false;
		Inky->pilladointro = false;
		isInky = false;
		Clyde->pilladointro = false;
		isClyde = false;

		end = false;
		timer = 0;

		if (loop) {
			loop = false;
			loopCheck = true;
		}
	}
	else {
		Point p1, p2;
		AABB box;

		if (timer == 10) {
			Blinky->SetPos({ playerX, playerY });
			isBlinky = true;
		}
		if (timer == 20) {
			Pinky->SetPos({ playerX, playerY });
			isPinky = true;
		}
		if (timer == 30) {
			Inky->SetPos({ playerX, playerY });
			isInky = true;
		}
		if (timer == 40) {
			Clyde->SetPos({ playerX, playerY });
			isClyde = true;
		}

		end = PacMan->IntroUpdate(turn);
		if (end) loop = true;

		if (isBlinky) Blinky->UPDATEintro(turn);
		if (isInky) Inky->UPDATEintro(turn);
		if (isPinky) Pinky->UPDATEintro(turn);
		if (isClyde) Clyde->UPDATEintro(turn);

		CheckCollisions();
		timer++;
	}
}

void Intro::Render() 
{
	BeginMode2D(camera);
	introScene->Render();
	PacMan->DrawPlayer();
	Blinky->DrawPlayer();
	Inky->DrawPlayer();
	Pinky->DrawPlayer();
	Clyde->DrawPlayer();
	for (OBJETOS* obj : pellet)
	{
		obj->Draw();
	}
	EndMode2D();
}

void Intro::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box;

	player_box = PacMan->GetHitbox();

	if (isDot) {
		auto it = pellet.begin();
		while (it != pellet.end()) {
			obj_box = (*it)->GetHitbox();
			if (player_box.TestAABB(obj_box)) {
				delete* it;
				it = pellet.erase(it);
				turn = true;
				isDot = false;
			}
			else {
				++it;
			}
		}
	}

	if (isClyde) {
		enemy_box = Clyde->GetHitbox();
		if (player_box.TestAABB(enemy_box)) Clyde->pilladointro = true;
	}

	if (isInky) {
		enemy_box = Inky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) Inky->pilladointro = true;
	}

	if (isPinky) {
		enemy_box = Pinky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) Pinky->pilladointro = true;
	}
	
	if (isBlinky) {
		enemy_box = Blinky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) Blinky->pilladointro = true;
	}
}

void Intro::Release() 
{
	PacMan->Release();
	Inky->Release();
	Blinky->Release();
	Pinky->Release();
	Clyde->Release();
	introScene->Release();
	for (Entity* obj : pellet)
	{
		delete obj;
	}
	pellet.clear();
}