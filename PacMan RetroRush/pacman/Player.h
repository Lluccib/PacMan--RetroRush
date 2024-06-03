#pragma once
#include "Entity.h"
#include "TileMap.h"
#define PLAYER_FRAME_SIZE		16
#define PLAYER_PHYSICAL_WIDTH	8
#define PLAYER_PHYSICAL_HEIGHT	8
#define PLAYER_SPEED			1
enum class State { INTRO, QUIETO, ANDANDO, MURIENDO, CERRADO };
enum class Vista { DERECHA, IZQUIERDA, ARRIBA, ABAJO };
enum class AnimacionesJugador {
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN, MURIENDO,
	CERRADO, ESCONDIDO,
	NUM_ANIMATIONS
};

class Jugador : public Entity
{
public:
	Jugador(const Point& p, State s, Vista view);
	~Jugador();

	AppStatus Initialise();	
	
	bool perder = false;

	bool PilladoIntro = false;
	void SetTileMap(TileMap* tilemap);
	void InitScore();
	void IncrementarPuntuación(int n);
	int pillarpuntos();
	void establecervidas(int l);	
	void LoseLives();
	int Getvidas();
	void Ganar();
	void PERDER();
	void Intro(int count);
	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	Point GetDirection();
	Point GetPosition();

	bool IntroUpdate(bool turn);

private:
	AnimacionesJugador GetAnimation();
	bool MirandoDerecha() const;
	bool MirandoIzquierda() const;
	bool MirandoArrriba() const;
	bool MirandoAbajo() const;
	void Mover();
	void SetAnimation(int id);
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();
	
	
	void PARAR();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void StartDying();

	Vista look;
	
	int puntos;
	int vidas = 3;
	Vista turn = Vista::DERECHA;
	TileMap* mapa;
	int contador = 0;
	bool pill1 = false;
	State state;
	
	Sound sonidomuerte;
};