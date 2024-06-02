#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include <cstring>
#include "Scene.h"

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	laser = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (laser != nullptr)
	{
		laser->Release();
		delete laser;
		laser = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BLOQUE_SUP_R] = { 0,  0, n, n };//1
	dict_rect[(int)Tile::BLOQUE_SUP_L] = { n,  0, n, n };//2
	dict_rect[(int)Tile::BLOQUE_INF_L] = { 5 * n, 0, n, n };//3
	dict_rect[(int)Tile::BLOQUE_INF_R] = { 4 * n, 0, n, n };//4
	dict_rect[(int)Tile::PARED_L] = { 3 * n,  0, n, n };//5
	dict_rect[(int)Tile::PARED_R] = { 2 * n,  0, n, n };//6
	dict_rect[(int)Tile::PARED_SUP] = { 10 * n, 0, n, n };//7
	dict_rect[(int)Tile::PARED_INF] = { 12 * n, 0, n, n };//8
	dict_rect[(int)Tile::ESQUINA_INF_L] = { 9 * n, n, n, n };//9
	dict_rect[(int)Tile::ESQUINA_INF_R] = { 10 * n, n, n, n };//10
	dict_rect[(int)Tile::ESQUINA_SUP_L] = { 5 * n, n, n, n };//11
	dict_rect[(int)Tile::ESQUINA_SUP_R] = { 6 * n, n, n, n };//12
	dict_rect[(int)Tile::LINEA_FINA_UP] = { 3 * n, n, n, n };//13
	dict_rect[(int)Tile::LINEA_FINA_DOWN] = { 14 * n, 0, n, n };//14
	dict_rect[(int)Tile::LINEA_FINA_L] = { 7 * n, n, n, n };//15
	dict_rect[(int)Tile::LINEA_FINA_R] = { 8 * n, n, n, n };//16

	dict_rect[(int)Tile::CUADRADO_SUP_R] = { 11 * n, n, n, n };//17
	dict_rect[(int)Tile::CUADRADO_SUP_L] = { 12 * n, n, n, n };//18

	dict_rect[(int)Tile::CUADRADO_INF_R] = { 13 * n, n, n, n };//19
	dict_rect[(int)Tile::CUADRADO_INF_L] = { 14 * n, n, n, n };//21

	dict_rect[(int)Tile::PAREDRARA_INF_R] = { 15 * n, n, n, n };//22
	dict_rect[(int)Tile::PAREDRARA_INF_L] = { 0, n, n, n };//23
	dict_rect[(int)Tile::PAREDRARA_SUP_R] = { n, n, n, n };//30
	dict_rect[(int)Tile::PAREDRARA_SUP_L] = { 2* n, n, n, n };//31

	dict_rect[(int)Tile::PARED2_SUP_L] = { 9 * n, 0, n, n };//32
	dict_rect[(int)Tile::PARED2_SUP_R] = { 8 * n, 0, n, n };//33
	dict_rect[(int)Tile::PARED2_INF_L] = { 7 * n, 0, n, n };//34
	dict_rect[(int)Tile::PARED2_INF_R] = { 6 * n, 0, n, n };//35

	dict_rect[(int)Tile::PARED3_R] = { 11 * n, 0, n, n };//36
	dict_rect[(int)Tile::PARED3_L] = { 15 * n, 0, n, n };//37

	dict_rect[(int)Tile::LASER_L] = { 0, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_R] = { 4 * n, 6 * n, n, n };
	
	dict_rect[(int)Tile::LASER_FRAME0] = { 1 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME1] = { 2 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME2] = { 3 * n, 6 * n, n, n };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/Pacmantileset5.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	if (data.LoadTexture(Resource::IMG_ITEMS, "images/Items.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_items = data.GetTexture(Resource::IMG_ITEMS);

	laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	laser->Update();
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}



bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionWallUp(const AABB& box) const
{
	return CollisionY(box.pos, box.height);
}
bool TileMap::TestCollisionWallDown(const AABB& box) const
{
	return CollisionY(box.pos + Point(0, box.height - 1), box.height);
}

Tile TileMap::TestSideExit(const AABB& box) const
{
	return GetTileIndex(box.pos.x, box.pos.y);
}
bool TileMap::SolidTest(const AABB& box) const
{
	return IsTileSolid(GetTileIndex(box.pos.x, box.pos.y));
}

bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(GetTileIndex(x,y)))
			return true;
	}
	return false;
}


void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	/*Vector2 pos;*/
	Color c;
	c.a = 128;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::LASER)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);

					/*if (Scene().debug == DebugMode::SIZE) {
						
					}*/

					/*DrawHitbox(pos.x, pos.y, TILE_SIZE, TILE_SIZE, c);*/
					
					/*render->DrawBox(pos.x, pos.y, TILE_SIZE, TILE_SIZE, c);
					render->DrawCorners(pos.x, pos.y, TILE_SIZE, TILE_SIZE);*/
					
				}
				else
				{
					laser->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}
}

void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	laser->Release();

	dict_rect.clear();
}
