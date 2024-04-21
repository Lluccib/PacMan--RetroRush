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
Scene::~Scene()
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
	player = new Player({ 0,0 }, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, State::IDLE, Look::RIGHT);
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
			 2,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,	  7,   7,   7,   7,  7,  7,  1,
			 5,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,  0,  0,  6,
			 5,   0,  12,  14,  14,  11,   0,  12,  14,  14,  14,  11,   0,   0,   0,   0,  12,  14,  14,  14,  11,   0,  12,  14,  14, 11,  0,  6,
			 5, 100,  16,   0,   0,  15,   0,  16,   0,   0,   0,  15,   0,   0,   0,   0,  16,   0,   0,   0,  15,	  0,  16,   0,   0, 15,  0,  6,
			 5,   0,  10,  13,  13,   9,   0,  10,  13,  13,  13,   9,   0,   0,   0,   0,  10,  13,  13,  13,   9,	  0,  10,  13,  13,  9,  0,  6,
			 5,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,  0,  0,  6,
			 5,   0,  12,  14,  14,  11,   0,  12,  11,   0,  12,  14,  14,  14,  14,  14,  14,  11,   0,  12,  11,	  0,  12,  14,  14, 11,  0,  6,
			 5,   0,  10,  13,  13,   9,   0,  16,  15,   0,  10,  13,  13,   0,   0,  13,  13,   9,   0,  16,  15,	  0,  10,  13,  13,  9,  0,  6,
			 5,   0,   0,   0,   0,   0,   0,  16,  15,   0,   0,   0,   0,  16,  15,   0,   0,   0,   0,  16,  15,	  0,   0,   0,   0,  0,  0,  6,
			 3,   8,   8,   8,   8,  11,   0,  16,   0,  14,  14,  11,   0,  16,  15,   0,  12,  14,  14,   0,  15,   0,  12,   8,   8,  8,  8,  4,
			 0,   0,   0,   0,   0,   5,   0,  16,   0,  13,  13,   9,   0,  10,   9,   0,  10,  13,  13,   0,  15,	  0,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,   0,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  15,	  0,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,   0,  16,  15,   0,  18,   8,   8,   0,   0,   8,   8,  17,   0,  16,  15,	  0,   6,   0,   0,  0,  0,  0,
			 7,   7,   7,   7,   7,   9,   0,  10,   9,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,  10,   9,	  0,  10,   7,   7,  7,  7,  7,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,   0,   0,	  0,   0,   0,   0,  0,  0,  0,
			 8,   8,   8,   8,   8,  11,   0,  12,  11,   0,   6,   0,   0,   0,   0,   0,   0,   5,   0,  12,  11,	  0,  12,   8,   8,  8,  8,  8,
			 0,   0,   0,   0,   0,   5,   0,  16,  15,   0,  21,   7,   7,   7,   7,   7,   7,  20,   0,  16,  15,   0,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,   0,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  15,   0,   6,   0,   0,  0,  0,  0,
			 0,   0,   0,   0,   0,   5,   0,  16,  15,   0,  12,  14,  14,  14,  14,  14,  14,  11,   0,  16,  15,   0,   6,   0,   0,  0,  0,  0,
			 2,   7,   7,   7,   7,   9,   0,  10,   9,   0,  10,  13,  13,   0,   0,  13,  13,   9,   0,  10,   9,   0,  10,   7,   7,  7,  7,  1,
			 5,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  6,
			 5,   0,  12,  14,  14,  11,   0,  12,  14,  14,  14,  11,   0,  16,  15,   0,  12,  14,  14,  14,  11,   0,  12,  14,  14, 11,  0,  6,
			 5,   0,  10,  13,   0,  15,   0,  10,  13,  13,  13,   9,   0,  10,   9,   0,  10,  13,  13,  13,   9,   0,  16,   0,  13,  9,  0,  6,
			 5,   0,   0,   0,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  16,  15,   0,  0,  0,  6,
			 0,  14,  11,   0,  16,  15,   0,  12,  11,   0,  12,  14,  14,  14,  14,  14,  14,  11,   0,  12,  11,   0,  16,  15,   0, 12, 14,  0,
			 0,  13,   9,   0,  10,   9,   0,  16,  15,   0,  10,  13,  13,  30,  31,  13,  13,   9,   0,  16,  15,   0,  10,   9,   0, 10, 13,  0,
			 5,   0,   0,   0,   0,   0,   0,  16,  15,   0,   0,   0,   0,  16,  15,   0,   0,   0,   0,  16,  15,   0,   0,   0,   0,  0,  0,  6,
			 5,   0,  12,  14,  14,  14,  14,  22,  23,  14,  14,  11,   0,  16,  15,   0,  12,  14,  14,  22,  23,  14,  14,  14,  14, 11,  0,  6,
			 5,   0,  10,  13,  13,  13,  13,  13,  13,  13,  13,   9,   0,  10,   9,   0,  10,  13,  13,  13,  13,  13,  13,  13,  13,  9,  0,  6,
			 5,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  6,
			 3,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,  8,  8,  4

		};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			13,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  13,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,  13,
			 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   9,
			10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,  63,   0,   0,  10,
			 9,   0,   0,   0,  62,   0,   0,   0,   0,   0,   0,   0,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,
			10,   0,   0,  62,   0,   0,   0,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,
			 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,  22,  23,  13,  17,  18,  17,  18,  17,  18,  13,   0,   9,
			10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  20,  21,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,
			 9,   0,  13,  17,  18,  17,  18,  17,  18,  13,  22,  23,  13,  20,  21,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,
			10,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  21,   0,  20,  21,   0,   0,   0,   0,   0,   0,   0,   0,  13,  10,
			 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  21,   0,  20,  21,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   9,
			10,  13,   0,   0,   0,  62,  62,   0,   0,   0,  20,  21,   0,  20,  21,   0,   0,   0,  63,   0,   0,   0,   0,   0,  10,
			 9,   0,   0,   0,   0,   0,   0,   0,   0,   0,  20,  21,   0,  20,  21,   0,   0,  63,   0,  63,   0,   0,   0,  13,   9,
			10,   0,   0,  -1,   0,   0,   0,   0,   0,   0,  20,  21,   0,  20,  21,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,
			 9,  13,   0, 100,   0,   0,   0,   0,   0,   0,  20,  21,   0,  20,  21,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9,
			10,  17,  18,  13,   0,   0,   0,   0,  13,  17,  18,  17,  18,  17,  18,  17,  13,   0,   0,   0,   0,	 13,  17,  18,  10,
			 9,   1,   2,  40,  70,  70,  70,  70,  41,   9,   5,   6,   9,   5,   6,   9,  40,  70,  70,  70,  70,  41,   1,   2,   9,
			10,   3,   4,  13,  14,  15,  16,  11,  12,  10,   7,   8,  10,   7,   8,  10,  13,  14,  15,  16,  11,  12,   3,   4,   10
		};
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
void Scene::HandleInputPlayer()
{
	bool moving = true;//Cambiat

	if (IsKeyPressed(KEY_F1))
	{
		/*TileMap().Render();*/
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
		
	}

	switch(player->GetState())
	{
	case State::IDLE:
		if (IsKeyDown(KEY_LEFT))		player->StartWalkingLeft();
		if (IsKeyDown(KEY_RIGHT))		player->StartWalkingRight();
		if (IsKeyDown(KEY_UP))			player->StartWalkingUp();
		if (IsKeyDown(KEY_DOWN))		player->StartWalkingDown();
		break;

	case State::WALKING:
		if (player->IsLookingRight())
		{
			if (IsKeyDown(KEY_RIGHT))
			{
				//continue walking right
				moving = true;

			}
			else if (IsKeyDown(KEY_LEFT))
			{
				player->StartWalkingLeft();
				moving = true;
			}
			else if (IsKeyDown(KEY_UP))
			{
				player->StartWalkingUp();
				moving = true;
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				player->StartWalkingDown();
				moving = true;
			}
		}
		if (player->IsLookingLeft())
		{
			if (IsKeyDown(KEY_RIGHT))
			{
				player->StartWalkingRight();
				moving = true;
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				moving = true;
			}
			else if (IsKeyDown(KEY_UP))
			{
				player->StartWalkingUp();
				moving = true;
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				player->StartWalkingDown();
				moving = true;
			}
		}
		if (player->IsLookingUp())
		{
			if (IsKeyDown(KEY_RIGHT))
			{
				player->StartWalkingRight();
				moving = true;
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				player->StartWalkingLeft();
				moving = true;
			}
			else if (IsKeyDown(KEY_UP))
			{
				moving = true;
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				player->StartWalkingDown();
				moving = true;
			}
		}
		if (player->IsLookingDown())
		{
			if (IsKeyDown(KEY_RIGHT))
			{
				player->StartWalkingRight();
				moving = true;
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				player->StartWalkingLeft();
				moving = true;
			}
			else if (IsKeyDown(KEY_UP))
			{
				player->StartWalkingUp();
				moving = true;
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				moving = true;
			}
		}
		if (!moving)	player->Stop();
		break;
	}
}
void Scene::Update()
{
	Point p1, p2;


	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);

	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))		LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);

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