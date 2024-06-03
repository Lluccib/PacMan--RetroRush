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

    nivel = nullptr;
	fuente = nullptr;
	vidasHUD = nullptr;

	
	
	camera.target = { 0, 0 };				
	camera.offset = { 0, 0 };	
	camera.rotation = 0.0f;					
	camera.zoom = 1.0f;		

	ResourceManager& data = ResourceManager::Instance();
	intro_sonido = data.GetSound(AudioResource::INTRO);

	sirenas[0] = data.GetSound(AudioResource::SIRENA1);
	sirenas[1] = data.GetSound(AudioResource::SIRENA2);
	sirenas[2] = data.GetSound(AudioResource::SIRENA3);
	sirenas[3] = data.GetSound(AudioResource::SIRENA4);
	sirenas[4] = data.GetSound(AudioResource::SIRENA5);

	fruta_sonido = data.GetSound(AudioResource::FRUITAUD);
	comerenemigo_sonido = data.GetSound(AudioResource::EATGHOST);
	punch1_sonido = data.GetSound(AudioResource::PUNCH1);
	punch2_sonido = data.GetSound(AudioResource::PUNCH2);

	pill_sonido = data.GetSound(AudioResource::AUD_PELLET);

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	StopSound(sirenas[sirena]);
	if(IsSoundPlaying(pill_sonido)) StopSound(pill_sonido);

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
    if (nivel != nullptr)
    {
		nivel->Release();
        delete nivel;
        nivel = nullptr;
    }
	if (vidasHUD != nullptr) {
		vidasHUD->Release();
		delete vidasHUD;
		vidasHUD = nullptr;
	}
	if (fuente != nullptr) {
		delete fuente;
		fuente = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{

	player = new Jugador({ 0,0 }, State::QUIETO, Vista::DERECHA);
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
	vidasHUD = new HUD({10, (WINDOW_HEIGHT)});
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
	if (vidasHUD->Initialise() != AppStatus::OK) {
		LOG("Failed to initialise lives UI");
		return AppStatus::ERROR;
	}
    nivel = new TileMap();
    if (nivel == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	if (nivel->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	player->SetTileMap(nivel);
	inky->SetTileMap(nivel);
	blinky->SetTileMap(nivel);
	pinky->SetTileMap(nivel);
	clyde->SetTileMap(nivel);
	PlaySound(intro_sonido);
	fuente = new Text();
	if (fuente == nullptr) {
		LOG("Failed to allocate memory for font");
		return AppStatus::ERROR;
	}
	if (fuente->Initialise(Resource::IMG_FONT, "Assets/sprites/fuente1.png", ' ', 8) != AppStatus::OK)
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
	//dos niveles iguales. Misma lógica. Cambian Items
	if (stage == 1 or stage == 2)
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
		sirena = 0;
		cuentafrutas = FRUITTIME;
		pillarcereza = false;
		cerezaTimepo = PELLETTIME;
		inky->EstablecerSalida();
		pinky->EstablecerSalida();
		clyde->EstablecerSalida();
		pinky->A_normal();
		clyde->A_normal();
		blinky->A_normal();
		inky->A_normal();
		
		if (IsSoundPlaying(pill_sonido)) StopSound(pill_sonido);
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
			else if (tile == Tile::PILL1)
			{
				posicionamiento.x = x * TILE_SIZE;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				objetito = new OBJETOS(posicionamiento, ObjectType::PILL1);
				objects.push_back(objetito);
				map[i] = 0;
			}
			else if (tile == Tile::CEREZA)
			{
				posicionamiento.x = x * TILE_SIZE;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				objetito = new OBJETOS(posicionamiento, ObjectType::PILL2);
				objects.push_back(objetito);
				map[i] = 0;
			}
			else if (tile == Tile::MONDONGO)
			{
				posicionamiento.x = x * TILE_SIZE + TILE_SIZE / 2;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				fruitX = posicionamiento.x;
				fruitY = posicionamiento.y;
				map[i] = 0;
			}
			else if (tile == Tile::ICONO1)
			{
				if(level_count == 1) map[i] = (int)Tile::ICONO2;
				if (level_count == 2) map[i] = (int)Tile::ICONO3;
			}
			else if (tile == Tile::GHOST_DOOR) {
				posicionamiento.x = x * TILE_SIZE;
				posicionamiento.y = y * TILE_SIZE + TILE_SIZE - 1;
				inky->EstablecerObjetivo(posicionamiento);
				pinky->EstablecerObjetivo(posicionamiento);
				clyde->EstablecerObjetivo(posicionamiento);
				inky->EstablecerSalidaCasa(posicionamiento);
				pinky->EstablecerSalidaCasa(posicionamiento);
				blinky->EstablecerSalidaCasa(posicionamiento);
				
				clyde->EstablecerSalidaCasa(posicionamiento);
			}
			++i;
		}
	}
	nivel->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
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
	if (IsKeyPressed(KEY_F3))       perder = true;
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
					if (!nivel->SolidTest(box)) checkTile = false;
				}
			}
			else {
				if (!nivel->SolidTest(box)) checkTile = false;
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
		StopSound(sirenas[sirena]);
		pillarcereza = false;
		cerezaTimepo = PELLETTIME;
		if(IsSoundPlaying(pill_sonido)) StopSound(pill_sonido);

		nivel->ganar = true;
		player->Ganar();
		inky->WinLose();
		blinky->WinLose();
		pinky->WinLose();
		clyde->WinLose();
		ganar = true;

		LoadLevel(0);
		EndLevel = false;
	}
	if (cuentafrutas == 0) {
		OBJETOS* obj = new OBJETOS({ fruitX, fruitY }, level_count);
		objects.push_back(obj);
	}
	cuentafrutas--;
	if (intro or levelintro) 
	{
		if (intro_count <= 0) {
			if (intro) player->establecervidas(2);
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
	else if (ganar) {
		if (!nivel->ganar) {
			level_count++;
			ganar = false;
			if (level_count > (int)LEVELS) {
				fin = true;
			}
			else {
				LoadLevel(level_count);
			}
		}
	}
	else if (perder) {
		StopSound(sirenas[sirena]);
		player->PERDER();
		if (!player->perder) {
			perder = false;
			if (player->Getvidas() >= 0) {
				player->SetPos({ playerX, playerY });
				inky->SetPos({ inkyX, inkyY });
				blinky->SetPos({ blinkyX, blinkyY });
				pinky->SetPos({ pinkyX, pinkyY });
				clyde->SetPos({ clydeX, clydeY });
			}
			else {
				pillarcereza = false;
				cerezaTimepo = PELLETTIME;
				if (IsSoundPlaying(pill_sonido)) StopSound(pill_sonido); StopSound(pill_sonido);
				fin = true;
			}
		}
	}
	else {
		if (!IsSoundPlaying(sirenas[sirena])) {
			if (sirena != 0) {
				if (!IsSoundPlaying(sirenas[sirena - 1])) PlaySound(sirenas[sirena]);
			} else PlaySound(sirenas[sirena]);
		}

		if (pillarcereza) {

			if (blinkypillado and inkypillado and clycepillado and pinkypillado) {
				blinkypillado = false;
				inkypillado = false;
				clycepillado = false;
				pinkypillado = false;
				pillarcereza = false;
				ghost_points = 200;
				cerezaTimepo = PELLETTIME;
				StopSound(pill_sonido);
			}

			if (cerezaTimepo >= 0) {
				if (!IsSoundPlaying(pill_sonido)) PlaySound(pill_sonido);
				--cerezaTimepo;
			}
			else {
				blinkypillado = false;
				inkypillado = false;
				clycepillado = false;
				pinkypillado = false;
				pillarcereza = false;
				ghost_points = 200;
				cerezaTimepo = PELLETTIME;
				StopSound(pill_sonido);
			}
			if(!blinkypillado) blinky->Pellet(pillarcereza, cerezaTimepo);
			if(!inkypillado) inky->Pellet(pillarcereza, cerezaTimepo);
			if(!clycepillado) clyde->Pellet(pillarcereza, cerezaTimepo);
			if(!pinkypillado) pinky->Pellet(pillarcereza, cerezaTimepo);
		}

		nivel->Update();
		player->Update();
		blinky->Update(player->GetDirection(), player->GetPosition(), blinky->GetEnemyPos());
		pinky->Update(player->GetDirection(), player->GetPosition(), blinky->GetEnemyPos());
		clyde->Update(player->GetDirection(), player->GetPosition(), blinky->GetEnemyPos());
		inky->Update(player->GetDirection(), player->GetPosition(), blinky->GetEnemyPos());
		
		CheckCollisions();
	}
}
void Scene::Render()
{
	BeginMode2D(camera);

    nivel->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->DrawPlayer();
		blinky->DrawPlayer();
		pinky->DrawPlayer();
		clyde->DrawPlayer();
		inky->DrawPlayer();
		
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
    nivel->Release();
	player->Release();

	inky->Release();
	blinky->Release();
	pinky->Release();
	clyde->Release();

	vidasHUD->Release();
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
				if (punch1) 
				{
					PlaySound(punch1_sonido);
					punch1 = false;
				}
				else 
				{
					PlaySound(punch2_sonido);
					punch1 = true;
				}
			}
            else if ((*it)->Sonidos() == (int)ObjectType::PILL2) {
				pillarcereza = true;
			}
			else if ((*it)->Sonidos() == (int)ObjectType::FRUTAS1 or (*it)->Sonidos() == (int)ObjectType::FRUTAS2) {
				PlaySound(fruta_sonido);
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
	else if (count <= PARTICIONES) sirena = 4;
	else if (count <= PARTICIONES * 2) sirena = 3;
	else if (count <= PARTICIONES * 3) sirena = 2;
	else if (count <= PARTICIONES * 4) sirena = 1;


	enemy_box = inky->GetHitbox();
	if (player_box.TestAABB(enemy_box)) {
		if (!inky->TaMuerto() and !pillarcereza and !god_mode) perder = true;
		else {
			if (!inkypillado and pillarcereza) {
				player->IncrementarPuntuación(ghost_points);
				ghost_points *= 2;
				inky->pillado = true;
				inkypillado = true;
				PlaySound(comerenemigo_sonido);
			}
		}
	}
	else {
		enemy_box = blinky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) {
			if (!blinky->TaMuerto() and !pillarcereza and !god_mode) perder = true;
			else {
				if (!blinkypillado and pillarcereza) {
					player->IncrementarPuntuación(ghost_points);
					ghost_points *= 2;
					blinky->pillado = true;
					blinkypillado = true;
					PlaySound(comerenemigo_sonido);
				}
			}
		}
		else {
			enemy_box = pinky->GetHitbox();
			if (player_box.TestAABB(enemy_box)) {
				if (!pinky->TaMuerto() and !pillarcereza and !god_mode) perder = true;
				else {
					if (!pinkypillado and pillarcereza) {
						player->IncrementarPuntuación(ghost_points);
						ghost_points *= 2;
						pinky->pillado = true;
						pinkypillado = true;
						PlaySound(comerenemigo_sonido);
					}
				}
			}
			else {
				enemy_box = clyde->GetHitbox();
				if (player_box.TestAABB(enemy_box)) {
					if (!clyde->TaMuerto() and !pillarcereza and !god_mode) perder = true;
					else {
						if (!clycepillado and pillarcereza) {
							player->IncrementarPuntuación(ghost_points);
							ghost_points *= 2;
							clyde->pillado = true;
							clycepillado = true;
							PlaySound(comerenemigo_sonido);
						}
					}
				}
			}
		}
	}
	
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
	fuente->Draw(10, 5, TextFormat("1UP"));
	fuente->Draw(10, 13, TextFormat("%d", player->pillarpuntos()));
	
	vidasHUD->RenderUI(player->Getvidas());
	vidasHUD->DrawPlayer();

	if (intro or levelintro) fuente->Draw((WINDOW_WIDTH / 2)-22, (WINDOW_HEIGHT / 2)+15, TextFormat("READY!"), YELLOW);
	if (intro and intro_count > 60) fuente->Draw((WINDOW_WIDTH / 2) - 40, (WINDOW_HEIGHT / 2) - 32, TextFormat("PLAYER ONE"), CYANBLUE);

	if(god_mode) fuente->Draw(WINDOW_WIDTH - 125, 5, TextFormat("modo god activo"));
}
void Scene::ClearLevel()
{
	for (OBJETOS* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}