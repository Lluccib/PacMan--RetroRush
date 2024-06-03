#include "Intro.h"
#include <stdio.h>
#include "Globals.h"
Intro::Intro() 
{
	pacman = nullptr;
	blinky = nullptr;
	pinky = nullptr;
	inky = nullptr;
	clyde = nullptr;
	introScene = nullptr;

	camera.target = { 0, 0 };				
	camera.offset = { 0, 0 };	
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
}

Intro::~Intro()
{
	if (pacman != nullptr) {
		pacman->Release();
		delete pacman;
		pacman = nullptr;
	}
	if (blinky != nullptr) {
		blinky->Release();
		delete blinky;
		blinky = nullptr;
	}
	if (clyde != nullptr) {
		clyde->Release();
		delete clyde;
		clyde = nullptr;
	}
	
	if (pinky != nullptr) {
		pinky->Release();
		delete pinky;
		pinky = nullptr;
	}
	if (inky != nullptr) {
		inky->Release();
		delete inky;
		inky = nullptr;
	}
	
	for (Entity* obj : pill1)
	{
		delete obj;
	}
	pill1.clear();
	if (introScene != nullptr) {
		introScene->Release();
		delete introScene;
		introScene = nullptr;
	}
}

AppStatus Intro::Init() 
{
	pacman = new Jugador({ 0,0 }, State::QUIETO, Vista::IZQUIERDA);
	if (pacman == nullptr)
	{
		LOG("Failed to allocate memory for intro PacMan");
		return AppStatus::ERROR;
	}
	inky = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::ROJO);
	if (inky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	blinky = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::AZULITO);
	if (blinky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	pinky = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::ROSA);
	if (pinky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	clyde = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::NARANJITA);
	if (clyde == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}

	if(pacman->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro pacman");
		return AppStatus::ERROR;
	}
	if (inky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (blinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (clyde->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (pinky->Initialise() != AppStatus::OK)
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
	
	pacman->SetTileMap(introScene);

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
				pacman->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::PELLET)
			{
				pos.x = x * TILE_SIZE;
				pill1X = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				pill2Y = pos.y;
				obj = new OBJETOS(pos, ObjectType::PILL2);
				pill1.push_back(obj);
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
		pacman->SetPos({ playerX, playerY });
		OBJETOS* obj = new OBJETOS({ pill1X, pill2Y }, ObjectType::PILL2);
		pill1.push_back(obj);
		isPill1 = true;

		blinky->pilladointro = false;
		isBlinky = false;
		pinky->pilladointro = false;
		isPinky = false;
		inky->pilladointro = false;
		isInky = false;
		clyde->pilladointro = false;
		isClyde = false;

		end = false;
		timming = 0;

		if (loop) {
			loop = false;
			checking = true;
		}
	}
	else {
		Point p1, p2;
		AABB box;

		if (timming == 10) {
			blinky->SetPos({ playerX, playerY });
			isBlinky = true;
		}
		if (timming == 20) {
			pinky->SetPos({ playerX, playerY });
			isPinky = true;
		}
		if (timming == 30) {
			inky->SetPos({ playerX, playerY });
			isInky = true;
		}
		if (timming == 40) {
			clyde->SetPos({ playerX, playerY });
			isClyde = true;
		}

		end = pacman->IntroUpdate(turn);
		if (end) loop = true;

		if (isBlinky) blinky->UPDATEintro(turn);
		if (isInky) inky->UPDATEintro(turn);
		if (isPinky) pinky->UPDATEintro(turn);
		if (isClyde) clyde->UPDATEintro(turn);

		CheckCollisions();
		timming++;
	}
}

void Intro::Render() 
{
	BeginMode2D(camera);
	introScene->Render();
	pacman->DrawPlayer();
	blinky->DrawPlayer();
	inky->DrawPlayer();
	pinky->DrawPlayer();
	clyde->DrawPlayer();
	for (OBJETOS* obj : pill1)
	{
		obj->Draw();
	}
	EndMode2D();
}

void Intro::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box;

	player_box = pacman->GetHitbox();

	if (isPill1) {
		auto it = pill1.begin();
		while (it != pill1.end()) {
			obj_box = (*it)->GetHitbox();
			if (player_box.TestAABB(obj_box)) {
				delete* it;
				it = pill1.erase(it);
				turn = true;
				isPill1 = false;
			}
			else {
				++it;
			}
		}
	}

	if (isClyde) {
		enemy_box = clyde->GetHitbox();
		if (player_box.TestAABB(enemy_box)) clyde->pilladointro = true;
	}

	if (isInky) {
		enemy_box = inky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) inky->pilladointro = true;
	}

	if (isPinky) {
		enemy_box = pinky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) pinky->pilladointro = true;
	}
	
	if (isBlinky) {
		enemy_box = blinky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) blinky->pilladointro = true;
	}
}

void Intro::Release() 
{
	pacman->Release();
	inky->Release();
	blinky->Release();
	pinky->Release();
	clyde->Release();
	introScene->Release();
	for (Entity* obj : pill1)
	{
		delete obj;
	}
	pill1.clear();
}