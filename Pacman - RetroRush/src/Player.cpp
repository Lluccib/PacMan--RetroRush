
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, int width, int height, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	score = 0;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/Pacmanspritesheet.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	 
	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);
	//IDLE
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { n, n, n, n });
	//NOU
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_UP, { n, 2 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_DOWN, { 0, 0, n, n });
	//
	//WALKING
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i * n, 0, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, 0,-n, n });
	//NOU
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_UP, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_UP, { (float)i * n, 3 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_DOWN, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_DOWN, { (float)i * n, 2 * n, n, n });
	//
	/*sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 2*n, 5*n, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 3*n, 5*n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 2*n, 5*n, -n, n });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 3*n, 5*n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 0, 5*n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 0, 5*n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { n, 5*n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { n, 5*n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING, ANIM_LADDER_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING, { (float)i * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_PRE_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_PRE_TOP, { 4 * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_TOP, { 5 * n, 6 * n, n, n });*/
		
	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Player::SetState(State s)
{
	state = s;
}
State Player::GetState() const
{
	return state;
}
void Player::SetLook(Look view)
{
	look = view;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsLookingUp() const
{
	return look == Look::UP;
}
bool Player::IsLookingDown() const
{
	return look == Look::DOWN;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Player::Stop()
{
	SetDirection({ 0,0 });  //abans era "dir ="
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	if (IsLookingLeft())	SetAnimation((int)PlayerAnim::IDLE_LEFT);
	if (IsLookingUp())		SetAnimation((int)PlayerAnim::IDLE_UP);
	if (IsLookingDown())	SetAnimation((int)PlayerAnim::IDLE_DOWN);
}
void Player::StartWalkingLeft()
{
	
	SetDirection({ -PLAYER_SPEED,0 });
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	
	SetDirection({ PLAYER_SPEED,0 });
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartWalkingUp()
{
	
	SetDirection({ 0,-PLAYER_SPEED });
	state = State::WALKING;
	look = Look::UP;
	SetAnimation((int)PlayerAnim::WALKING_UP);
}
void Player::StartWalkingDown()
{
	
	SetDirection({ 0,PLAYER_SPEED });
	state = State::WALKING;
	look = Look::DOWN;
	SetAnimation((int)PlayerAnim::WALKING_DOWN);
}

void Player::Update()
{
	
	if (pos.x == 216 && pos.y == 119) {
		pos.x = 0;
		pos.y = 119;
	}

	if (pos.x == 0 && pos.y == 119) {
		pos.x = 216;
		pos.y = 119;
	}

	AABB box;

	box = GetHitbox();
	//IflookingUp
	if (map->TestCollisionWallRight(box))
	{
		printf("%d %d\n", pos.x, pos.y);
		printf("papi");
		pos.x = pos.x - 1;
		printf("%d %d\n", pos.x, pos.y);
		/*if (state == State::WALKING);*/
	}

	/*box = GetHitbox();*/
	if (map->TestCollisionWallLeft(box))
	{
		printf("%d %d\n", pos.x, pos.y);
		pos.x = pos.x + 1;
		printf("%d %d\n", pos.x, pos.y);
		/*if (state == State::WALKING);*/
	}

	/*box = GetHitbox();*/
	if (map->TestCollisionWallUp(box))
	{
		printf("%d %d\n", pos.x, pos.y);
		pos.y = pos.y + 1;
		printf("%d %d\n", pos.x, pos.y);
		/*if (state == State::WALKING);*/
	}
	/*box = GetHitbox();*/

	if (map->TestCollisionWallDown(box))
	{
		printf("%d %d\n", pos.x, pos.y);
		pos.y = pos.y - 1;
		printf("%d %d\n", pos.x, pos.y);
		/*if (state == State::WALKING);*/
	}

	//IflookingDown
	
	//IflookingRight
	//IflookingLeft

	Entity::Update();
	
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();



}

//void Player::MoveX()
//{
//	AABB box;
//	int prev_x = pos.x;
//
//	//We can only go up and down while climbing
//	if (state == State::CLIMBING)	return;
//
//	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
//	{
//		pos.x += -PLAYER_SPEED;
//		if (state == State::IDLE) StartWalkingLeft();
//		else
//		{
//			if (IsLookingRight()) ChangeAnimLeft();
//		}
//
//		box = GetHitbox();
//		if (map->TestCollisionWallLeft(box))
//		{
//			pos.x = prev_x;
//			if (state == State::WALKING) Stop();
//		}
//	}
//	else if (IsKeyDown(KEY_RIGHT))
//	{
//		pos.x += PLAYER_SPEED;
//		if (state == State::IDLE) StartWalkingRight();
//		else
//		{
//			if (IsLookingLeft()) ChangeAnimRight();
//		}
//
		/*box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}*/
//	}
//	else
//	{
//		if (state == State::WALKING) Stop();
//	}
//}
//void Player::MoveY()
//{
//	AABB box;
//
//	if (state == State::JUMPING)
//	{
//		LogicJumping();
//	}
//	else if (state == State::CLIMBING)
//	{
//		LogicClimbing();
//	}
//	else //idle, walking, falling
//	{
//		pos.y += PLAYER_SPEED;
//		box = GetHitbox();
//		if (map->TestCollisionGround(box, &pos.y))
//		{
//			if (state == State::FALLING) Stop();
//
//			if (IsKeyDown(KEY_UP))
//			{
//				box = GetHitbox();
//				if (map->TestOnLadder(box, &pos.x))
//					StartClimbingUp();
//			}
//			else if (IsKeyDown(KEY_DOWN))
//			{
//				//To climb up the ladder, we need to check the control point (x, y)
//				//To climb down the ladder, we need to check pixel below (x, y+1) instead
//				box = GetHitbox();
//				box.pos.y++;
//				if (map->TestOnLadderTop(box, &pos.x))
//				{
//					StartClimbingDown();
//					pos.y += PLAYER_LADDER_SPEED;
//				}
//					
//			}
//			else if (IsKeyPressed(KEY_SPACE))
//			{
//				StartJumping();
//			}
//		}
//		else
//		{
//			if (state != State::FALLING) StartFalling();
//		}
//	}
//}
//void Player::LogicJumping()
//{
//	AABB box, prev_box;
//	int prev_y;
//
//	jump_delay--;
//	if (jump_delay == 0)
//	{
//		prev_y = pos.y;
//		prev_box = GetHitbox();
//
//		pos.y += dir.y;
//		dir.y += GRAVITY_FORCE;
//		jump_delay = PLAYER_JUMP_DELAY;
//
//		//Is the jump finished?
//		if (dir.y > PLAYER_JUMP_FORCE)
//		{
//			dir.y = PLAYER_SPEED;
//			StartFalling();
//		}
//		else
//		{
//			//Jumping is represented with 3 different states
//			if (IsAscending())
//			{
//				if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
//				else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
//			}
//			else if (IsLevitating())
//			{
//				if (IsLookingRight())	SetAnimation((int)PlayerAnim::LEVITATING_RIGHT);
//				else					SetAnimation((int)PlayerAnim::LEVITATING_LEFT);
//			}
//			else if (IsDescending())
//			{
//				if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
//				else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
//			}
//		}
//		//We check ground collision when jumping down
//		if (dir.y >= 0)
//		{
//			box = GetHitbox();
//
//			//A ground collision occurs if we were not in a collision state previously.
//			//This prevents scenarios where, after levitating due to a previous jump, we found
//			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
//			//crossing it.
//			if (!map->TestCollisionGround(prev_box, &prev_y) &&
//				map->TestCollisionGround(box, &pos.y))
//			{
//				Stop();
//			}
//		}
//	}
//}
//void Player::LogicClimbing()
//{
//	AABB box;
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	int tmp;
//
//	if (IsKeyDown(KEY_UP))
//	{
//		pos.y -= PLAYER_LADDER_SPEED;
//		sprite->NextFrame();
//	}
//	else if (IsKeyDown(KEY_DOWN))
//	{
//		pos.y += PLAYER_LADDER_SPEED;
//		sprite->PrevFrame();
//	}
//
//	//It is important to first check LadderTop due to its condition as a collision ground.
//	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
//	box = GetHitbox();
//	if (map->TestOnLadderTop(box, &tmp))
//	{
//		if (IsInSecondHalfTile())		SetAnimation((int)PlayerAnim::CLIMBING_PRE_TOP);
//		else if (IsInFirstHalfTile())	SetAnimation((int)PlayerAnim::CLIMBING_TOP);
//		else					LOG("Internal error, tile should be a LADDER_TOP, coord: (%d,%d)", box.pos.x, box.pos.y);
//	}
//	else if (map->TestCollisionGround(box, &pos.y))
//	{
//		//Case leaving the ladder descending.
//		Stop();
//		sprite->SetAutomaticMode();
//	}
//	else if (!map->TestOnLadder(box, &tmp))
//	{
//		//Case leaving the ladder ascending.
//		//If we are not in a LadderTop, colliding ground or in the Ladder it means we are leaving
//		//ther ladder ascending.
//		Stop();
//		sprite->SetAutomaticMode();
//	}
//	else
//	{
//		if (GetAnimation() != PlayerAnim::CLIMBING)	SetAnimation((int)PlayerAnim::CLIMBING);
//	}
//}
void Player::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18*16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}