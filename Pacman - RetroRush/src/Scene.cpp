#include "Scene.h"
#include <stdio.h>
#include "Globals.h"
#include "TileMap.h"


Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()//2820
{
	if (player != nullptr)
	{
		/*player->Release();*/
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	Object *obj;
	
	ClearLevel();//Aquesta funció neteja els components de l'escena.

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			 2,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  36,  37,   7,   7,   7,   7,   7,   7,	  7,   7,   7,   7,  7,  7,  1,
			 5,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  62,  62,	 62,  62,  62,  62, 62, 62,  6,
			 5,  62,  12,  14,  14,  11,  62,  12,  14,  14,  14,  11,  62,  16,  15,  62,  12,  14,  14,  14,  11,  62,  12,  14,  14, 11, 62,  6,
			 5,  63,  16,   0,   0,  15,  62,  16,   0,   0,   0,  15,  62,  16,  15,  62,  16,   0,   0,   0,  15,	 62,  16,   0,   0, 15, 63,  6,
			 5,  62,  10,  13,  13,   9,  62,  10,  13,  13,  13,   9,  62,  10,   9,  62,  10,  13,  13,  13,   9,	 62,  10,  13,  13,  9, 62,  6,
			 5,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,	 62,  62,  62,  62, 62, 62,  6,
			 5,  62,  12,  14,  14,  11,  62,  12,  11,  62,  12,  14,  14,  14,  14,  14,  14,  11,  62,  12,  11,	 62,  12,  14,  14, 11, 62,  6,
			 5,  62,  10,  13,  13,   9,  62,  16,  15,  62,  10,  13,  13,  30,  31,  13,  13,   9,  62,  16,  15,	 62,  10,  13,  13,  9, 62,  6,
			 5,  62,  62,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  16,  15,	 62,  62,  62,  62, 62, 62,  6,
			 3,   8,   8,   8,   8,  11,  62,  16,  23,  14,  14,  11,  62,  16,  15,  62,  12,  14,  14,  22,  15,  62,  12,   8,   8,  8,  8,  4,
			 0,   0,   0,   0,   0,   5,  62,  16,  31,  13,  13,   9,   0,  10,   9,   0,  10,  13,  13,  30,  15,	 62,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  15,	 62,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,  18,   8,   8,   0,   0,   8,   8,  17,   0,  16,  15,	 62,   6,   0,   0,  0,  0,  0,
			 7,   7,   7,   7,   7,   9,  62,  10,   9,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,  10,   9,	 62,  10,   7,   7,  7,  7,  7,
			 0,   0,   0,   0,   0,   0,  62,   0,   0,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,   0,   0,	 62,   0,   0,   0,  0,  0,  0,
			 8,   8,   8,   8,   8,  11,  62,  12,  11,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,  12,  11,	 62,  12,   8,   8,  8,  8,  8,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,  21,   7,   7,   7,   7,   7,   7,  20,   0,  16,  15,  62,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  15,  62,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,  12,  14,  14,  14,  14,  14,  14,  11,   0,  16,  15,  62,   6,   0,   0,  0,  0,  0,
			 2,   7,   7,   7,   7,   9,  62,  10,   9,   0,  10,  13,  13,  30,  31,  13,  13,   9,   0,  10,   9,  62,  10,   7,   7,  7,  7,  1,
			 5,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62, 62, 62,  6,
			 5,  62,  12,  14,  14,  11,  62,  12,  14,  14,  14,  11,  62,  16,  15,  62,  12,  14,  14,  14,  11,  62,  12,  14,  14, 11, 62,  6,
			 5,  62,  10,  13,  30,  15,  62,  10,  13,  13,  13,   9,  62,  10,   9,  62,  10,  13,  13,  13,   9,  62,  16,  31,  13,  9, 62,  6,
			 5,  63,  62,  62,  16,  15,  62,  62,  62,  62,  62,  62,  62, 100,   0,  62,  62,   0,  62,  62,  62,  62,  16,  15,  62, 62, 63,  6,
			34,  14,  11,  62,  16,  15,  62,  12,  11,  62,  12,  14,  14,  14,  14,  14,  14,  11,  62,  12,  11,  62,  16,  15,  62, 12, 14, 35,
			32,  13,   9,  62,  10,   9,  62,  16,  15,  62,  10,  13,  13,  30,  31,  13,  13,   9,  62,  16,  15,  62,  10,   9,  62, 10, 13, 33,
			 5,  62,  62,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62, 62, 62,  6,
			 5,  62,  12,  14,  14,  14,  14,  22,  23,  14,  14,  11,  62,  16,  15,  62,  12,  14,  14,  22,  23,  14,  14,  14,  14, 11, 62,  6,
			 5,  62,  10,  13,  13,  13,  13,  13,  13,  13,  13,   9,  62,  10,   9,  62,  10,  13,  13,  13,  13,  13,  13,  13,  13,  9, 62,  6,
			 5,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62, 62, 62,  6,
			 3,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  8,  8,  4

		};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			 2,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,  36,  37,   7,   7,   7,   7,   7,   7,	  7,   7,   7,   7,  7,  7,  1,
			 5,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  62,  62,	 62,  62,  62,  62, 62, 62,  6,
			 5,  62,  12,  14,  14,  11,  62,  12,  14,  14,  14,  11,  62,  16,  15,  62,  12,  14,  14,  14,  11,  62,  12,  14,  14, 11, 62,  6,
			 5,  63,  16,   0,   0,  15,  62,  16,   0,   0,   0,  15,  62,  16,  15,  62,  16,   0,   0,   0,  15,	 62,  16,   0,   0, 15, 63,  6,
			 5,  62,  10,  13,  13,   9,  62,  10,  13,  13,  13,   9,  62,  10,   9,  62,  10,  13,  13,  13,   9,	 62,  10,  13,  13,  9, 62,  6,
			 5,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,	 62,  62,  62,  62, 62, 62,  6,
			 5,  62,  12,  14,  14,  11,  62,  12,  11,  62,  12,  14,  14,  14,  14,  14,  14,  11,  62,  12,  11,	 62,  12,  14,  14, 11, 62,  6,
			 5,  62,  10,  13,  13,   9,  62,  16,  15,  62,  10,  13,  13,  30,  31,  13,  13,   9,  62,  16,  15,	 62,  10,  13,  13,  9, 62,  6,
			 5,  62,  62,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  16,  15,	 62,  62,  62,  62, 62, 62,  6,
			 3,   8,   8,   8,   8,  11,  62,  16,  23,  14,  14,  11,  62,  16,  15,  62,  12,  14,  14,  22,  15,  62,  12,   8,   8,  8,  8,  4,
			 0,   0,   0,   0,   0,   5,  62,  16,  31,  13,  13,   9,   0,  10,   9,   0,  10,  13,  13,  30,  15,	 62,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  15,	 62,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,  18,   8,   8,   0,   0,   8,   8,  17,   0,  16,  15,	 62,   6,   0,   0,  0,  0,  0,
			 7,   7,   7,   7,   7,   9,  62,  10,   9,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,  10,   9,	 62,  10,   7,   7,  7,  7,  7,
			 0,   0,   0,   0,   0,   0,  62,   0,   0,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,   0,   0,	 62,   0,   0,   0,  0,  0,  0,
			 8,   8,   8,   8,   8,  11,  62,  12,  11,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,  12,  11,	 62,  12,   8,   8,  8,  8,  8,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,  21,   7,   7,   7,   7,   7,   7,  20,   0,  16,  15,  62,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  15,  62,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,  62,  16,  15,   0,  12,  14,  14,  14,  14,  14,  14,  11,   0,  16,  15,  62,   6,   0,   0,  0,  0,  0,
			 2,   7,   7,   7,   7,   9,  62,  10,   9,   0,  10,  13,  13,  30,  31,  13,  13,   9,   0,  10,   9,  62,  10,   7,   7,  7,  7,  1,
			 5,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62, 62, 62,  6,
			 5,  62,  12,  14,  14,  11,  62,  12,  14,  14,  14,  11,  62,  16,  15,  62,  12,  14,  14,  14,  11,  62,  12,  14,  14, 11, 62,  6,
			 5,  62,  10,  13,  30,  15,  62,  10,  13,  13,  13,   9,  62,  10,   9,  62,  10,  13,  13,  13,   9,  62,  16,  31,  13,  9, 62,  6,
			 5,  63,  62,  62,  16,  15,  62,  62,  62,  62,  62,  62,  62, 100,   0,  62,  62,   0,  62,  62,  62,  62,  16,  15,  62, 62, 63,  6,
			34,  14,  11,  62,  16,  15,  62,  12,  11,  62,  12,  14,  14,  14,  14,  14,  14,  11,  62,  12,  11,  62,  16,  15,  62, 12, 14, 35,
			32,  13,   9,  62,  10,   9,  62,  16,  15,  62,  10,  13,  13,  30,  31,  13,  13,   9,  62,  16,  15,  62,  10,   9,  62, 10, 13, 33,
			 5,  62,  62,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62,  16,  15,  62,  62,  62,  62, 62, 62,  6,
			 5,  62,  12,  14,  14,  14,  14,  22,  23,  14,  14,  11,  62,  16,  15,  62,  12,  14,  14,  22,  23,  14,  14,  14,  14, 11, 62,  6,
			 5,  62,  10,  13,  13,  13,  13,  13,  13,  13,  13,   9,  62,  10,   9,  62,  10,  13,  13,  13,  13,  13,  13,  13,  13,  9, 62,  6,
			 5,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62,  62, 62, 62,  6,
			 3,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  8,  8,  4

		};
		player->InitScore();
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
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
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_APPLE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::APPLE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_CHILI)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CHILI);
				objects.push_back(obj);
				map[i] = 0;
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);

	}
	//Debug levels instantly
	/*if (IsKeyPressed(KEY_F5))		LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);*/

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
		Object* obj = new Object( { objectX, objectY }, level_count);
		objects.push_back(obj); 
	}

	level->Update();
	player->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);

	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());
			
			//Delete the object
			delete* it; 
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it); 
		}
		else
		{
			//Move to the next object
			++it; 
		}
	}
}
void Scene::ClearLevel()//el seu objectiu és eliminar els objectes per evitar sobrecàrregas
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 10, 8, LIGHTGRAY);
}