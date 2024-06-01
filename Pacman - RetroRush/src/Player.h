#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		8

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	8
#define PLAYER_PHYSICAL_HEIGHT	8

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1


//Gravity affects jumping velocity when jump_delay is 0
//#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, DEAD, DYING, CLOSED};
enum class Look { RIGHT, LEFT, UP, DOWN };//NOU UP I DOWN

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN, DYING,
	CLOSED, HIDDEN,//NOU UP I DOWN
	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void LoseLives();
	int GetLives();
	Point GetDirection();
	Point GetPosition();
	void setLives(int l);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	void Win();
	void Lose();

	bool lose = false;
	

private:
	
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	void Move();

    //Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();

	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void StartDying();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();
	TileMap* map;

	int prev_x = pos.x;
	int prev_y = pos.y;
	int score;
	int lives = 3;
	int count = 0;
	bool pellet = false;

	State state;
	Look look;
	Look turn = Look::RIGHT;
	int jump_delay;

};

