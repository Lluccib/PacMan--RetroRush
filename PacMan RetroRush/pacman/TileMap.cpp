#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
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
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;
	//nos sirve para saber que rectangulo de la imagen de tiles corresponde a cada tile
	const int Z = TILE_SIZE + 1;

	dict_rect[(int)Tile::AIR] = { 12*Z,  2*Z, n, n };

	dict_rect[(int)Tile::DOUBLECORNER_TR] = { 0,  0, n, n };
	dict_rect[(int)Tile::DOUBLECORNER_TL] = { Z,  0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_R] = { 2*Z, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_L] = { 3 * Z, 0, n, n };
	dict_rect[(int)Tile::DOUBLECORNER_BR] = { 4 * Z, 0, n, n };
	dict_rect[(int)Tile::DOUBLECORNER_BL] = { 5 * Z, 0, n, n };
	dict_rect[(int)Tile::CORNERWALL_BR] = { 6 * Z, 0, n, n };
	dict_rect[(int)Tile::CORNERWALL_BL] = { 7 * Z, 0, n, n };
	dict_rect[(int)Tile::CORNERWALL_TR] = { 8 * Z, 0, n, n };
	dict_rect[(int)Tile::CORNERWALL_TL] = { 9 * Z, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_TR] = { 10 * Z, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_TL] = { 11 * Z, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_BR] = { 12 * Z, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_BL] = { 13 * Z, 0, n, n };
	dict_rect[(int)Tile::WALL_BR] = { 14 * Z, 0, n, n };
	dict_rect[(int)Tile::WALL_BL] = { 15 * Z, 0, n, n };

	dict_rect[(int)Tile::LARGECORNER_BR] = { 0,  Z, n, n };
	dict_rect[(int)Tile::LARGECORNER_BL] = { Z,  Z, n, n };
	dict_rect[(int)Tile::LARGECORNER_TR] = { 2 * Z, Z, n, n };
	dict_rect[(int)Tile::LARGECORNER_TL] = { 3 * Z, Z, n, n };
	dict_rect[(int)Tile::WALL_TR] = { 4 * Z, Z, n, n };
	dict_rect[(int)Tile::WALL_TL] = { 5 * Z, Z, n, n };
	dict_rect[(int)Tile::CORNER_TR] = { 6 * Z, Z, n, n };
	dict_rect[(int)Tile::CORNER_TL] = { 7 * Z, Z, n, n };
	dict_rect[(int)Tile::WALL_R] = { 8 * Z, Z, n, n };
	dict_rect[(int)Tile::WALL_L] = { 9 * Z, Z, n, n };
	dict_rect[(int)Tile::CORNER_BR] = { 10 * Z, Z, n, n };
	dict_rect[(int)Tile::CORNER_BL] = { 11 * Z, Z, n, n };
	dict_rect[(int)Tile::SQUARECORNER_TR] = { 12 * Z, Z, n, n };
	dict_rect[(int)Tile::SQUARECORNER_TL] = { 13 * Z, Z, n, n };
	dict_rect[(int)Tile::SQUARECORNER_BR] = { 14 * Z, Z, n, n };
	dict_rect[(int)Tile::SQUARECORNER_BL] = { 15 * Z, Z, n, n };

	dict_rect[(int)Tile::SQUAREWALL_L] = { 0,  2*Z, n, n };
	dict_rect[(int)Tile::SQUAREWALL_R] = { Z,  2 * Z, n, n };
	dict_rect[(int)Tile::TOPCORNER_L] = { 2 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::TOPCORNER_R] = { 3 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::BOTTOMCORNER_L] = { 4 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::BOTTOMCORNER_R] = { 5 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::SMALLCORNER_TR] = { 6 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::SMALLCORNER_TL] = { 7 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::SMALLCORNER_BR] = { 8 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::SMALLCORNER_BL] = { 9 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::TOPCORNERWALL_L] = { 10 * Z, 2 * Z, n, n };
	dict_rect[(int)Tile::TOPCORNERWALL_R] = { 11 * Z, 2 * Z, n, n };

	dict_rect[(int)Tile::FRUIT_ICON_1] = { 0, 3 * Z, n*2, n*2 };
	dict_rect[(int)Tile::FRUIT_ICON_2] = { n*2, 3 * Z, n*2, n*2 };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "Resources/tilemap/Arcade - Pac-Man - Maze Parts-only_blue_tiles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	if (data.LoadTexture(Resource::IMG_TILES_WHITE, "Resources/tilemap/Arcade - Pac-Man - Maze Parts-only_white_tiles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles_white = data.GetTexture(Resource::IMG_TILES_WHITE);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
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
	int y = 0;
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
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
bool TileMap::TestCollisionWallLeft(const AABB& box, bool door) const
{
	if (!door) return CollisionX(box.pos, box.height, door);
	else return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box, bool door) const
{
	if (!door) return CollisionX(box.pos + Point(box.width - 1, 0), box.height, door);
	else CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionWallUp(const AABB& box, bool door) const
{
	if (!door) return CollisionY(box.pos, box.height, door);
	else CollisionY(box.pos, box.height);
}
bool TileMap::TestCollisionWallDown(const AABB& box, bool door) const
{
	if (!door) return CollisionY(box.pos + Point(0, box.height - 1), box.height, door);
	else CollisionY(box.pos + Point(0, box.height - 1), box.height);
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
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile))
			return true;
	}
	return false;
}
bool TileMap::CollisionX(const Point& p, int distance, bool door) const
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
		if (IsTileSolid(GetTileIndex(x, y))) return true;
		if (GetTileIndex(x, y) == Tile::GHOST_DOOR or GetTileIndex(x, y) == Tile::GHOST_DOOR2) return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance, bool door) const
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
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile)) return true;
		if (tile == Tile::GHOST_DOOR or tile == Tile::GHOST_DOOR2) return true;
	}
	return false;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	if (win) {
		if (white == 0) {
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					tile = map[i * width + j];
					if (tile != Tile::AIR)
					{
						pos.x = (float)j * TILE_SIZE;
						pos.y = (float)i * TILE_SIZE;
						rc = dict_rect[(int)tile];
						DrawTextureRec(*img_tiles_white, rc, pos, WHITE);

					}
				}
			}
			--delay;
			if (delay <= 0) {
				delay = 30;
				white = 4;
				--flash;
			}
		}
		else {
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					tile = map[i * width + j];
					if (tile != Tile::AIR)
					{
						pos.x = (float)j * TILE_SIZE;
						pos.y = (float)i * TILE_SIZE;
						rc = dict_rect[(int)tile];
						DrawTextureRec(*img_tiles, rc, pos, WHITE);

					}
				}
			}
			--delay;
			if (delay <= 0) {
				delay = 30;
				white = 0;
				--flash;
			}
		}
		if (flash == 0) {
			win = false;
			flash = 4;
		}

	}
	else {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				tile = map[i * width + j];
				if (tile != Tile::AIR)
				{
					pos.x = (float)j * TILE_SIZE;
					pos.y = (float)i * TILE_SIZE;
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);

				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_TILES);
	data.ReleaseTexture(Resource::IMG_TILES_WHITE);

	dict_rect.clear();
}