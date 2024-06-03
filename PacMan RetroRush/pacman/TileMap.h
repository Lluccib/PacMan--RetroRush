#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	EMPTY = -1,
	EXIT_LEFT = -3, EXIT_RIGHT = -2,
	AIR = 0,//ESPACIO VACÍO

	//0<50 SERAN ESTATICOS
	ESQUINALARGA_BR = 17, ESQUINALARGA_BL, ESQUINALARGA_TR, ESQUINALARGA_TL,//esquinas
	PARED_TR, PARED_TL, //paredes
	ESQUINA_TR, ESQUINA_TL, //esquinas
	PARED_R, PARED_L,//paredes
	ESQUINA_BR, ESQUINA_BL,//esquinas
	CUADRADO_TR, CUADRADO_TL, CUADRADO_BR, CUADRADO_BL,//cuadraditos
	
	ESQUINADOBLE_R = 1, ESQUINADOBLE_L, PAREDDOBLE_R, PAREDDOBLE_L, ESQUINADOBLE_BR, ESQUINADOBLE_BL,
	ESQUINAPARED_BR, ESQUINAPARED_BL, ESQUINAPARED_TR, ESQUINAPARED_TL, 
	DOBLEPARED_TR, DOBLEPARED_TL, DOBLEPARED_BR, DOBLEPARED_BL,
	PARED_BR, PARED_BL, 


	PAREDCUADRADA_L = 33, PAREDCUADRADA_R,
	ESQUINASUPERIOR_L, ESQUINASUPERIOR_R, ESQUINAINFERIOR_L, ESQUINAINFERIOR_R,
	PEQUEÑA_TR, PEQUEÑA_TL, PEQUEÑA_BR, PEQUEÑA_BL,
	TOPE_L, TOPE_R,
	// 50 <= id < 100: special tiles
	PILL1 = 50, PILL2, CEREZA, MONDONGO,

	GHOST_DOOR = 70, GHOST_DOOR2,

	//Intervalos
	STATIC_FIRST = ESQUINADOBLE_R,
	STATIC_LAST = TOPE_R,
	SOLID_FIRST = ESQUINADOBLE_R,
	SOLID_LAST = TOPE_R,
	SPECIAL_FIRST = PILL1,
	SPECIAL_LAST = CEREZA,

	ICONO1 = 200, ICONO2, ICONO3,
	//jugador & enemigos
	PLAYER = 100,
	BLINKY = 101, PINKY, INKY, CLYDE

	

};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	bool TestCollisionWallUp(const AABB& box) const;
	bool TestCollisionWallDown(const AABB& box) const;

	bool TestCollisionWallLeft(const AABB& box, bool door) const;
	bool TestCollisionWallRight(const AABB& box, bool door) const;
	bool TestCollisionWallUp(const AABB& box, bool door) const;
	bool TestCollisionWallDown(const AABB& box, bool door) const;
	bool ganar = false;
	bool perder = false;
	Tile TestSideExit(const AABB& box) const;
	bool SolidTest(const AABB& box) const;



private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	bool CollisionX(const Point& p, int distance, bool door) const;
	bool CollisionY(const Point& p, int distance, bool door) const;
	Tile* map;
	int size, width, height;
	int tiempos = 4;
	std::unordered_map<int, Rectangle> dict_rect;

	int blanco = 0;
	const Texture2D* tilesnormales;
	const Texture2D* tilesganar;

	int retraso = 30;
	
	
};