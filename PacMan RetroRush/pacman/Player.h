#pragma once
#include "Entity.h"
#include "TileMap.h"

#define PLAYER_FRAME_SIZE		16

#define PLAYER_PHYSICAL_WIDTH	8
#define PLAYER_PHYSICAL_HEIGHT	8

#define PLAYER_SPEED			1

enum class State { INTRO, IDLE, WALKING, MURIENDO, CERRADO };
enum class Look { RIGHT, LEFT, UP, DOWN };

enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN, MURIENDO,
	CERRADO, ESCONDIDO,
	NUM_ANIMATIONS
};

class Player : public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrementarPuntuación(int n);
	int GetPuntos();

	void setLives(int l);
	void Ganar();
	void PERDER();
	void Intro(int count);
	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	void LoseLives();
	int Getvidas();
	Point GetDirection();
	Point GetPosition();


	bool lose = false;

	bool introCaught = false;
	bool IntroUpdate(bool turn);

private:
	bool MirandoDerecha() const;
	bool MirandoIzquierda() const;
	bool MirandoArrriba() const;
	bool MirandoAbajo() const;
	void Move();
	void SetAnimation(int id);
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();

	PlayerAnim GetAnimation();
	void PARAR();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void StartDying();
	
	State state;
	Look look;
	//new var turn to check which way the player wants to turn (initialized as right since its the starting direction)
	Look turn = Look::RIGHT;

	TileMap* map;

	int puntos;
	int vidas = 3;
	int count = 0;
	bool pellet = false;
	Sound sound_death;
};