 
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
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
int Player::GetLives()
{
	return lives;
}
Point Player::GetDirection()
{
	return dir;
}
Point Player::GetPosition() {
	return pos;
}
void Player::setLives(int l)
{
	lives = l;
}
void Player::LoseLives()
{
	--lives;
}
void Player::Win()
{
	SetAnimation((int)PlayerAnim::CLOSED);
}
void Player::Lose() {
	lose = true;
	if (count == 0) {
		/*PlaySound(sound_death);*/
		SetAnimation((int)PlayerAnim::DYING);
		LoseLives();
	}

	count++;

	if (count < 150) {
		if (count < 48) {
			Sprite* sprite = dynamic_cast<Sprite*>(render);
			sprite->Update();
		}
		else SetAnimation((int)PlayerAnim::HIDDEN);
	}
	else {
		lose = false;
		count = 0;
		SetAnimation((int)PlayerAnim::CLOSED);
	}
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
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	SetDirection({ 0,0 });  //abans era "dir ="
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else if (IsLookingUp())  SetAnimation((int)PlayerAnim::IDLE_UP);
	else if (IsLookingDown())  SetAnimation((int)PlayerAnim::IDLE_DOWN);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartWalkingUp()
{
	
	state = State::WALKING;
	look = Look::UP;
	SetAnimation((int)PlayerAnim::WALKING_UP);
}
void Player::StartWalkingDown()
{
	
	state = State::WALKING;
	look = Look::DOWN;
	SetAnimation((int)PlayerAnim::WALKING_DOWN);
}
void Player::StartDying()
{
	state = State::DYING;

	SetAnimation((int)PlayerAnim::DYING);
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
	}
}
void Player::ChangeAnimUp()
{
	look = Look::UP;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_UP);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_UP); break;
	}
}
void Player::ChangeAnimDown()
{
	look = Look::DOWN;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_DOWN);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_DOWN); break;
	}
}
void Player::Update()
{
	//all movement in move
	Move();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::Move()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	//checks which way the player wants to turn next
	if (IsKeyPressed(KEY_UP) or IsKeyDown(KEY_UP))
	{
		turn = Look::UP;
		state = State::WALKING;
	}
	else if (IsKeyPressed(KEY_DOWN) or IsKeyDown(KEY_DOWN)) {
		turn = Look::DOWN;
		state = State::WALKING;
	}
	else if (IsKeyPressed(KEY_RIGHT) or IsKeyDown(KEY_RIGHT)) {
		turn = Look::RIGHT;
		state = State::WALKING;
	}
	else if (IsKeyPressed(KEY_LEFT) or IsKeyDown(KEY_LEFT)) {
		turn = Look::LEFT;
		state = State::WALKING;
	}

	//checks if the turn is possible
	if (turn != look) {
		switch (turn) {
		case Look::UP:
			pos.y -= PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallUp(box)) ChangeAnimUp();
			pos.y = prev_y;
			break;
		case Look::DOWN:
			pos.y += PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallDown(box)) ChangeAnimDown();
			pos.y = prev_y;
			break;
		case Look::LEFT:
			pos.x -= PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallLeft(box)) ChangeAnimLeft();
			pos.x = prev_x;
			break;
		case Look::RIGHT:
			pos.x += PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallRight(box)) ChangeAnimRight();
			pos.x = prev_x;
			break;
		}
	}

	if (look == Look::LEFT)
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE)StartWalkingLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
		if (pos.x == 0) {
			pos.x = WINDOW_WIDTH;
			ChangeAnimRight();
		}

	}
	else if (look == Look::RIGHT)
	{
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingRight();
		else
		{
			if (!IsLookingRight()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
		if (pos.x == WINDOW_WIDTH - 8) {
			pos.x = 0;
			ChangeAnimLeft();
		}

	}
	else if (look == Look::UP) {
		pos.y -= PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingUp();
		else
		{
			if (!IsLookingUp()) ChangeAnimUp();
		}

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
		}
	}
	else if (look == Look::DOWN) {
		pos.y += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingDown();
		else
		{
			if (!IsLookingDown()) ChangeAnimDown();
		}

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
		}
	}
}
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