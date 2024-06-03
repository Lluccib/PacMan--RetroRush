#pragma once
#include "Entity.h"
#include "TileMap.h"
#define ENEMY_FRAME_SIZE		16
#define ENEMY_PHYSICAL_WIDTH	8
#define ENEMY_PHYSICAL_HEIGHT	8
#define VELOCIDAD_ENEMIGO			1

enum class Estado { IDLE, WALKING, PELLET, EYES };
enum class Mirada { RIGHT, LEFT, UP, DOWN };
enum class Modo { BUSCAR, ENCONTRAR };
enum class TipoEnemigo {AZULITO, ROSA, ROJO, NARANJITA};

enum class AnimacionesEnemigos {
	IDLE,
	VAIZQUIERDA, VADERECHA, VAARRIBA, VAABAJO, CEREZA, CEREZAS,
	 ESCONDIDO, IZQUIERDA, DERECHA, ARRIBA, ABAJO,
	NUM_ANIMATIONS
};

class ENEMIGO : public Entity
{
public:
	ENEMIGO(const Point& p, Estado s, Mirada view, TipoEnemigo t);
	~ENEMIGO();
	bool pillado = false;
	bool Valla = false;
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);
	void Intro(int count);
	void Pellet(bool ifPellet, int count);
	void EstablecerObjetivo(Point t);
	void EstablecerSalida();
	void EstablecerCasa(Point t);
	void EstablecerSalidaCasa(Point t);
	void A_normal();
	bool TaMuerto();
	
	Point GetEnemyPos();

	void Update(Point pacmanDir, Point pacmanPos, Point blinkypos);
	void Release();
	void DrawDebug(const Color& col) const;
	void WinLose();

	bool pilladointro = false;
	void UPDATEintro(bool turn);

private:	
	Point casa;
	Point salida_casa;
	Point objetivo;
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;
	float GetTargetDistance(Point dir);
	void SetAnimation(int id);
	AnimacionesEnemigos GetAnimation();
	void Parar();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void EmpiezaMorir();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();
	void Move(Point pacmanDir, Point pacmanPos, Point blinkypos);
	void UpdateTarget(Point pacmanDir, Point pacmanPos, Point blinkypos);
	void UpdateLook(int anim_id);
	int pasos;
	int fps;
	bool comible = false;
	bool lucecita = true;
	Estado state;
	Mirada vista;
	Modo mode;
	TipoEnemigo type;
	TileMap* mapa;
	int retardo = 30;
	Sound retratado;



	
	

};
