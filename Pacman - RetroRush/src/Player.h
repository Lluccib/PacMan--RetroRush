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

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		10

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
//#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, DEAD };
enum class Look { RIGHT, LEFT, UP, DOWN };//NOU UP I DOWN

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN,//NOU UP I DOWN
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
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

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	void SetState(State s);
	State GetState() const;

	void SetLook(Look view);
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;


    //Animation management
	void SetAnimation(int id);
	/*PlayerAnim GetAnimation();*/

	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	TileMap* map;

	int prev_x = pos.x;
	int prev_y = pos.y;

	int score;

private:

	State state;
	Look look;
	int jump_delay;


	/*TileMap *map;

	int score;*/
};

