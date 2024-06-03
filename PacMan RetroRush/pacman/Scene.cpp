#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;

	blinky = nullptr;
	inky = nullptr;
	pinky = nullptr;
	clyde = nullptr;

    level = nullptr;

	livesUI = nullptr;

	font = nullptr;
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, 0 };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	ResourceManager& data = ResourceManager::Instance();
	sound_intro = data.GetSound(AudioResource::INTRO);

	sound_munch1 = data.GetSound(AudioResource::PUNCH1);
	sound_munch2 = data.GetSound(AudioResource::PUNCH2);

	sound_pellet = data.GetSound(AudioResource::AUD_PELLET);
	sound_fruit = data.GetSound(AudioResource::FRUITAUD);
	sound_eatghost = data.GetSound(AudioResource::EATGHOST);

	sirens[0] = data.GetSound(AudioResource::SIRENA1);
	sirens[1] = data.GetSound(AudioResource::SIRENA2);
	sirens[2] = data.GetSound(AudioResource::SIRENA3);
	sirens[3] = data.GetSound(AudioResource::SIRENA4);
	sirens[4] = data.GetSound(AudioResource::SIRENA5);

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	StopSound(sirens[siren]);
	if(IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);

	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (blinky != nullptr)
	{
		blinky->Release();
		delete blinky;
		blinky = nullptr;
	}
	if (inky != nullptr)
	{
		inky->Release();
		delete inky;
		inky = nullptr;
	}
	if (pinky != nullptr)
	{
		pinky->Release();
		delete pinky;
		pinky = nullptr;
	}
	if (clyde != nullptr)
	{
		clyde->Release();
		delete clyde;
		clyde = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	if (livesUI != nullptr) {
		livesUI->Release();
		delete livesUI;
		livesUI = nullptr;
	}
	if (font != nullptr) {
		delete font;
		font = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{

	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	inky = new ENEMIGO({0,0}, Estado::QUIETO, Mirada::UP, TipoEnemigo::ROJO);
	if (inky == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	blinky = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::LEFT, TipoEnemigo::AZULITO);
	if (blinky == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	pinky = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::UP, TipoEnemigo::ROSA);
	if (pinky == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	clyde = new ENEMIGO({ 0,0 }, Estado::QUIETO, Mirada::UP, TipoEnemigo::NARANJITA);
	if (clyde == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	livesUI = new HUD({10, (WINDOW_HEIGHT)});
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}
	if (inky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	if (blinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	if (pinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	if (clyde->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	if (livesUI->Initialise() != AppStatus::OK) {
		LOG("Failed to initialise lives UI");
		return AppStatus::ERROR;
	}
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	player->SetTileMap(level);
	inky->SetTileMap(level);
	blinky->SetTileMap(level);
	pinky->SetTileMap(level);
	clyde->SetTileMap(level);
	PlaySound(sound_intro);
	font = new Text();
	if (font == nullptr) {
		LOG("Failed to allocate memory for font");
		return AppStatus::ERROR;
	}
	if (font->Initialise(Resource::IMG_FONT, "Assets/sprites/fuente1.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point posicionamiento;
	int* map = nullptr;
	OBJETOS *objetito;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 0)
	{
		map = new int[size] {
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 2, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 44, 43, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 1,
			 4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 3,
			 4,  0, 40, 15, 15, 39,  0, 40, 15, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 15, 39,  0, 40, 15, 15, 39,  0, 3,
			 4,  0, 25,  0,  0, 26,  0, 25,  0,  0,  0, 26,  0, 25, 26,  0, 25,  0,  0,  0, 26,  0, 25,  0,  0, 26,  0, 3,
			 4,  0, 28, 21, 21, 27,  0, 28, 21, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 21, 27,  0, 28, 21, 21, 27,  0, 3,
			 4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 3,
			 4,  0, 40, 15, 15, 39,  0, 35, 36,  0, 40, 15, 15, 15, 15, 15, 15, 39,  0, 35, 36,  0, 40, 15, 15, 39,  0, 3,
			 4,  0, 28, 21, 21, 27,  0, 25, 26,  0, 28, 21, 21, 36, 35, 21, 21, 27,  0, 25, 26,  0, 28, 21, 21, 27,  0, 3,
			 4,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0, 3,
			 6, 13, 13, 13, 13, 23,  0, 25, 28, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 27, 26,  0, 24, 13, 13, 13, 13, 5,
			 0,  0,  0,  0,  0,  4,  0, 25, 40, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 39, 26,  0,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26,  0,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0, 30, 13, 34, 70, 71, 33, 13, 29,  0, 25, 26,  0,  3,  0,  0,  0,  0, 0,
			11, 11, 11, 11, 11, 27,  0, 37, 38,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 37, 38,  0, 28, 11, 11, 11, 11, 11,
			-3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0, -2,
			13, 13, 13, 13, 13, 23,  0, 35, 36,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 35, 36,  0, 24, 13, 13, 13, 13, 13,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0, 32, 11, 11, 11, 11, 11, 11, 31,  0, 25, 26,  0,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26, 0,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0, 40, 15, 15, 15, 15, 15, 15, 39,  0, 25, 26,  0,  3,  0,  0,  0,  0, 0,
			 2, 11, 11, 11, 11, 27,  0, 37, 38,  0, 28, 21, 21, 36, 35, 21, 21, 27,  0, 37, 38,  0, 28, 11, 11, 11, 11, 1,
			 4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 3,
			 4,  0, 40, 15, 15, 39,  0, 40, 15, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 15, 39,  0, 40, 15, 15, 39,  0, 3,
			 4,  0, 28, 21, 39, 26,  0, 28, 21, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 21, 27,  0, 25, 40, 21, 27,  0, 3,
			 4,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0, 3,
			 8, 15, 39,  0, 25, 26,  0, 35, 36,  0, 40, 15, 15, 15, 15, 15, 15, 39,  0, 35, 36,  0, 25, 26,  0, 40, 15, 7,
			10, 21, 27,  0, 37, 38,  0, 25, 26,  0, 28, 21, 21, 36, 35, 21, 21, 27,  0, 25, 26,  0, 37, 38,  0, 28, 21, 9,
			 4,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0, 3,
			 4,  0, 40, 15, 15, 15, 15, 27, 28, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 27, 28, 15, 15, 15, 15, 39,  0, 3,
			 4,  0, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27,  0, 3,
			 4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 3,
			 6, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 5,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0 
		};
	}
	else if (stage == 1 or stage == 2)
	{
		map = new int[size] {
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 2, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 44, 43, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 1,
			 4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
			 4, 50, 40, 15, 15, 39, 50, 40, 15, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 15, 39, 50, 40, 15, 15, 39, 50, 3,
			 4, 52, 25,  0,  0, 26, 50, 25,  0,  0,  0, 26, 50, 25, 26, 50, 25,  0,  0,  0, 26, 50, 25,  0,  0, 26, 52, 3,
			 4, 50, 28, 21, 21, 27, 50, 28, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 27, 50, 28, 21, 21, 27, 50, 3,
			 4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
			 4, 50, 40, 15, 15, 39, 50, 35, 36, 50, 40, 15, 15, 15, 15, 15, 15, 39, 50, 35, 36, 50, 40, 15, 15, 39, 50, 3,
			 4, 50, 28, 21, 21, 27, 50, 25, 26, 50, 28, 21, 21, 36, 35, 21, 21, 27, 50, 25, 26, 50, 28, 21, 21, 27, 50, 3,
			 4, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 3,
			 6, 13, 13, 13, 13, 23, 50, 25, 28, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 27, 26, 50, 24, 13, 13, 13, 13, 5,
			 0,  0,  0,  0,  0,  4, 50, 25, 40, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 39, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0,  0,  0,  0,  102,  0,  0,  0,  0,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0, 30, 13, 34, 70, 71, 33, 13, 29,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			11, 11, 11, 11, 11, 27, 50, 37, 38,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 37, 38, 50, 28, 11, 11, 11, 11, 11,
			-3,  0,  0,  0,  0,  0, 50,  0,  0,  0,  3,  104,  0, 101,  0,  103,  0,  4,  0,  0,  0, 50,  0,  0,  0,  0,  0, -2,
			13, 13, 13, 13, 13, 23, 50, 35, 36,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 35, 36, 50, 24, 13, 13, 13, 13, 13,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0, 32, 11, 11, 11, 11, 11, 11, 31,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0,  0,  0,  0, 53,  0,  0,  0,  0,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0, 40, 15, 15, 15, 15, 15, 15, 39,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 2, 11, 11, 11, 11, 27, 50, 37, 38,  0, 28, 21, 21, 36, 35, 21, 21, 27,  0, 37, 38, 50, 28, 11, 11, 11, 11, 1,
			 4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
			 4, 50, 40, 15, 15, 39, 50, 40, 15, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 15, 39, 50, 40, 15, 15, 39, 50, 3,
			 4, 50, 28, 21, 39, 26, 50, 28, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 27, 50, 25, 40, 21, 27, 50, 3,
			 4, 52, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 100,  0, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 52, 3,
			 8, 15, 39, 50, 25, 26, 50, 35, 36, 50, 40, 15, 15, 15, 15, 15, 15, 39, 50, 35, 36, 50, 25, 26, 50, 40, 15, 7,
			10, 21, 27, 50, 37, 38, 50, 25, 26, 50, 28, 21, 21, 36, 35, 21, 21, 27, 50, 25, 26, 50, 37, 38, 50, 28, 21, 9,
			 4, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 3,
			 4, 50, 40, 15, 15, 15, 15, 27, 28, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 27, 28, 15, 15, 15, 15, 39, 50, 3,
			 4, 50, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27, 50, 3,
			 4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
			 6, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 5,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  200, 0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0 
		};
		if(stage == 1) player->InitScore();
		if (stage == 2) levelintro = true;
		siren = 0;
		fruitcounter = FRUITTIME;
		collectPellet = false;
		pellet_timer = PELLETTIME;
		inky->EstablecerSalida();
		pinky->EstablecerSalida();
		clyde->EstablecerSalida();
		blinky->A_normal();
		inky->A_normal();
		pinky->A_normal();
		clyde->A_normal();
		if (IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);
	}
	else
	{
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}
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
				posicionamiento.x = x * TILE_SIZE + TILE_SIZE / 2;
				playerX = posicionamiento.x;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				playerY = posicionamiento.y;
				player->SetPos(posicionamiento);
				map[i] = 0;
			}
			else if (tile == Tile::INKY)
			{
				posicionamiento.x = x * TILE_SIZE + TILE_SIZE / 2;
				inkyX = posicionamiento.x;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				inkyY = posicionamiento.y;
				inky->SetPos(posicionamiento);
				map[i] = 0;
			}
			else if (tile == Tile::BLINKY)
			{
				posicionamiento.x = x * TILE_SIZE + TILE_SIZE / 2;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				blinkyX = posicionamiento.x;
				blinkyY = posicionamiento.y;
				blinky->SetPos(posicionamiento);
				blinky->EstablecerCasa(posicionamiento);
				pinky->EstablecerCasa(posicionamiento);
				clyde->EstablecerCasa(posicionamiento);
				inky->EstablecerCasa(posicionamiento);
				map[i] = 0;
			}
			else if (tile == Tile::PINKY)
			{
				posicionamiento.x = x * TILE_SIZE + TILE_SIZE / 2;
				pinkyX = posicionamiento.x;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				pinkyY = posicionamiento.y;
				pinky->SetPos(posicionamiento);
				map[i] = 0;
			}
			else if (tile == Tile::CLYDE)
			{
				posicionamiento.x = x * TILE_SIZE + TILE_SIZE / 2;
				clydeX = posicionamiento.x;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				clydeY = posicionamiento.y;
				clyde->SetPos(posicionamiento);
				map[i] = 0;
			}
			else if (tile == Tile::DOT)
			{
				posicionamiento.x = x * TILE_SIZE;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				objetito = new OBJETOS(posicionamiento, ObjectType::PILL1);
				objects.push_back(objetito);
				map[i] = 0;
			}
			else if (tile == Tile::PELLET)
			{
				posicionamiento.x = x * TILE_SIZE;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				objetito = new OBJETOS(posicionamiento, ObjectType::PILL2);
				objects.push_back(objetito);
				map[i] = 0;
			}
			else if (tile == Tile::FRUIT)
			{
				posicionamiento.x = x * TILE_SIZE + TILE_SIZE / 2;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				fruitX = posicionamiento.x;
				fruitY = posicionamiento.y;
				map[i] = 0;
			}
			else if (tile == Tile::FRUIT_ICON)
			{
				if(level_count == 1) map[i] = (int)Tile::FRUIT_ICON_1;
				if (level_count == 2) map[i] = (int)Tile::FRUIT_ICON_2;
			}
			else if (tile == Tile::GHOST_DOOR) {
				posicionamiento.x = x * TILE_SIZE;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				inky->EstablecerObjetivo(posicionamiento);
				pinky->EstablecerObjetivo(posicionamiento);
				clyde->EstablecerObjetivo(posicionamiento);
				blinky->EstablecerSalidaCasa(posicionamiento);
				inky->EstablecerSalidaCasa(posicionamiento);
				pinky->EstablecerSalidaCasa(posicionamiento);
				clyde->EstablecerSalidaCasa(posicionamiento);
			}
			++i;
		}
	}
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] map;
	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	if (IsKeyPressed(KEY_F2))       EndLevel = true;
	if (IsKeyPressed(KEY_F3))       lose = true;
	if (IsKeyPressed(KEY_F5)) {
		int objectX, objectY;
		bool checkTile = true;
		while (checkTile) {
			objectX = (int)GetRandomValue(7, LEVEL_WIDTH - 7);
			objectY = (int)GetRandomValue(7, LEVEL_HEIGHT - 7);
			AABB box({ objectX, objectY }, TILE_SIZE, TILE_SIZE);
			if (objectX <= 16 and objectX >= 11) {
				if (objectY >= 16 and objectY <= 18) {
					checkTile = true;
				}
				else {
					if (!level->SolidTest(box)) checkTile = false;
				}
			}
			else {
				if (!level->SolidTest(box)) checkTile = false;
			}
		}
		objectX = objectX * TILE_SIZE;
		objectY = objectY * TILE_SIZE + TILE_SIZE - 1;
		OBJETOS* obj = new OBJETOS({objectX, objectY}, level_count);
		objects.push_back(obj);
	}
	if (IsKeyPressed(KEY_F4)) {
		if (god_mode) god_mode = false;
		else god_mode = true;
	}
	if (IsKeyPressed(KEY_ONE)) {
		level_count = 1;
		LoadLevel(1);
	}
	if (IsKeyPressed(KEY_TWO)) {
		level_count = 2;
		LoadLevel(2);
	}
	if (EndLevel) {
		StopSound(sirens[siren]);
		collectPellet = false;
		pellet_timer = PELLETTIME;
		if(IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);

		level->win = true;
		player->Ganar();
		inky->WinLose();
		blinky->WinLose();
		pinky->WinLose();
		clyde->WinLose();
		win = true;

		LoadLevel(0);
		EndLevel = false;
	}
	if (fruitcounter == 0) {
		OBJETOS* obj = new OBJETOS({ fruitX, fruitY }, level_count);
		objects.push_back(obj);
	}
	fruitcounter--;
	if (intro or levelintro) 
	{
		if (intro_count <= 0) {
			if (intro) player->setLives(2);
			if(intro) intro = false;
			if (levelintro) levelintro = false;
			intro_count = 240;
		}
		else 
		{
			player->Intro(intro_count);
			inky->Intro(intro_count);
			blinky->Intro(intro_count);
			pinky->Intro(intro_count);
			clyde->Intro(intro_count);
			--intro_count;
		}
	}
	else if (win) {
		if (!level->win) {
			level_count++;
			win = false;
			if (level_count > (int)LEVELS) {
				EndGame = true;
			}
			else {
				LoadLevel(level_count);
			}
		}
	}
	else if (lose) {
		StopSound(sirens[siren]);
		player->PERDER();
		if (!player->lose) {
			lose = false;
			if (player->Getvidas() >= 0) {
				player->SetPos({ playerX, playerY });
				inky->SetPos({ inkyX, inkyY });
				blinky->SetPos({ blinkyX, blinkyY });
				pinky->SetPos({ pinkyX, pinkyY });
				clyde->SetPos({ clydeX, clydeY });
			}
			else {
				collectPellet = false;
				pellet_timer = PELLETTIME;
				if (IsSoundPlaying(sound_pellet)) StopSound(sound_pellet); StopSound(sound_pellet);
				EndGame = true;
			}
		}
	}
	else {
		if (!IsSoundPlaying(sirens[siren])) {
			if (siren != 0) {
				if (!IsSoundPlaying(sirens[siren - 1])) PlaySound(sirens[siren]);
			} else PlaySound(sirens[siren]);
		}

		if (collectPellet) {

			if (blinkyCaught and inkyCaught and clydeCaught and pinkyCaught) {
				blinkyCaught = false;
				inkyCaught = false;
				clydeCaught = false;
				pinkyCaught = false;
				collectPellet = false;
				ghost_points = 200;
				pellet_timer = PELLETTIME;
				StopSound(sound_pellet);
			}

			if (pellet_timer >= 0) {
				if (!IsSoundPlaying(sound_pellet)) PlaySound(sound_pellet);
				--pellet_timer;
			}
			else {
				blinkyCaught = false;
				inkyCaught = false;
				clydeCaught = false;
				pinkyCaught = false;
				collectPellet = false;
				ghost_points = 200;
				pellet_timer = PELLETTIME;
				StopSound(sound_pellet);
			}
			if(!blinkyCaught) blinky->Pellet(collectPellet, pellet_timer);
			if(!inkyCaught) inky->Pellet(collectPellet, pellet_timer);
			if(!clydeCaught) clyde->Pellet(collectPellet, pellet_timer);
			if(!pinkyCaught) pinky->Pellet(collectPellet, pellet_timer);
		}

		level->Update();
		player->Update();
		inky->Update(player->GetDirection(), player->GetPosition(), blinky->GetEnemyPos());
		blinky->Update(player->GetDirection(), player->GetPosition(), blinky->GetEnemyPos());
		pinky->Update(player->GetDirection(), player->GetPosition(), blinky->GetEnemyPos());
		clyde->Update(player->GetDirection(), player->GetPosition(), blinky->GetEnemyPos());
		CheckCollisions();
	}
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->DrawPlayer();
		inky->DrawPlayer();
		blinky->DrawPlayer();
		pinky->DrawPlayer();
		clyde->DrawPlayer();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		inky->DrawDebug(GREEN);
		blinky->DrawDebug(GREEN);
		pinky->DrawDebug(GREEN);
		clyde->DrawDebug(GREEN);
	}

	RenderGUI();
	EndMode2D();
}
void Scene::Release()
{
    level->Release();
	player->Release();

	inky->Release();
	blinky->Release();
	pinky->Release();
	clyde->Release();

	livesUI->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box;
	int count = 0;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		count++;
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player->IncrementarPuntuación((*it)->Puntos());
			if ((*it)->Sonidos() == (int)ObjectType::PILL1) 
			{
				if (munch1) 
				{
					PlaySound(sound_munch1);
					munch1 = false;
				}
				else 
				{
					PlaySound(sound_munch2);
					munch1 = true;
				}
			}
            else if ((*it)->Sonidos() == (int)ObjectType::PILL2) {
				collectPellet = true;
			}
			else if ((*it)->Sonidos() == (int)ObjectType::FRUTAS1 or (*it)->Sonidos() == (int)ObjectType::FRUTAS2) {
				PlaySound(sound_fruit);
			}
			delete* it;
			it = objects.erase(it);
		}
		else
		{
			++it;
		}
	}
	if (count == 0) {
		EndLevel = true;
		count = 0;
	}
	else if (count <= FRACTION5_ITEMS) siren = 4;
	else if (count <= FRACTION5_ITEMS * 2) siren = 3;
	else if (count <= FRACTION5_ITEMS * 3) siren = 2;
	else if (count <= FRACTION5_ITEMS * 4) siren = 1;


	enemy_box = inky->GetHitbox();
	if (player_box.TestAABB(enemy_box)) {
		if (!inky->TaMuerto() and !collectPellet and !god_mode) lose = true;
		else {
			if (!inkyCaught and collectPellet) {
				player->IncrementarPuntuación(ghost_points);
				ghost_points *= 2;
				inky->pillado = true;
				inkyCaught = true;
				PlaySound(sound_eatghost);
			}
		}
	}
	else {
		enemy_box = blinky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) {
			if (!blinky->TaMuerto() and !collectPellet and !god_mode) lose = true;
			else {
				if (!blinkyCaught and collectPellet) {
					player->IncrementarPuntuación(ghost_points);
					ghost_points *= 2;
					blinky->pillado = true;
					blinkyCaught = true;
					PlaySound(sound_eatghost);
				}
			}
		}
		else {
			enemy_box = pinky->GetHitbox();
			if (player_box.TestAABB(enemy_box)) {
				if (!pinky->TaMuerto() and !collectPellet and !god_mode) lose = true;
				else {
					if (!pinkyCaught and collectPellet) {
						player->IncrementarPuntuación(ghost_points);
						ghost_points *= 2;
						pinky->pillado = true;
						pinkyCaught = true;
						PlaySound(sound_eatghost);
					}
				}
			}
			else {
				enemy_box = clyde->GetHitbox();
				if (player_box.TestAABB(enemy_box)) {
					if (!clyde->TaMuerto() and !collectPellet and !god_mode) lose = true;
					else {
						if (!clydeCaught and collectPellet) {
							player->IncrementarPuntuación(ghost_points);
							ghost_points *= 2;
							clyde->pillado = true;
							clydeCaught = true;
							PlaySound(sound_eatghost);
						}
					}
				}
			}
		}
	}
	
}
void Scene::ClearLevel()
{
	for (OBJETOS* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (OBJETOS* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (OBJETOS* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	font->Draw(10, 5, TextFormat("1UP"));
	font->Draw(10, 13, TextFormat("%d", player->GetPuntos()));
	
	livesUI->RenderUI(player->Getvidas());
	livesUI->DrawPlayer();

	if (intro or levelintro) font->Draw((WINDOW_WIDTH / 2)-22, (WINDOW_HEIGHT / 2)+15, TextFormat("READY!"), YELLOW);
	if (intro and intro_count > 60) font->Draw((WINDOW_WIDTH / 2) - 40, (WINDOW_HEIGHT / 2) - 32, TextFormat("PLAYER ONE"), CYANBLUE);

	if(god_mode) font->Draw(WINDOW_WIDTH - 125, 5, TextFormat("modo god activo"));
}