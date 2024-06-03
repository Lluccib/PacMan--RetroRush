#include "Enemys.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include <cmath>

ENEMIGO::ENEMIGO(const Point& p, State_e s, Look_e view, EnemyType t) :
	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	type = t;
	if (type != EnemyType::AZULITO) useDoor = true;
	if (type != EnemyType::NARANJITA) mode = Mode_e::BUSCAR;
	else mode = Mode_e::ENCONTRAR;
	
}
ENEMIGO::~ENEMIGO()
{
}
AppStatus ENEMIGO::Initialise()
{
	const int n = ENEMY_FRAME_SIZE;
	int k;
	switch (type) {
	case EnemyType::AZULITO:
		k = n * 2;
		break;
	case EnemyType::ROSA:
		k = n;
		break;
	case EnemyType::ROJO:
		k = 0;
		break;
	case EnemyType::NARANJITA:
		k = n * 3;
		break;
	}

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMY, "Assets/sprites/fantasmasfull.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	sound_retreat = data.GetSound(AudioResource::AUD_RETREAT);

	render = new Sprite(data.GetTexture(Resource::IMG_ENEMY));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE, { n*4, (float)k, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::HIDDEN, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::HIDDEN, { n * 4, 5*n, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_RIGHT, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)i*n, (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { 2 * n + ((float)i*n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_UP, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_UP, { 4 * n + ((float)i * n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_DOWN, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_DOWN, { 6 * n + ((float)i * n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::PELLET, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::PELLET, { (float)i * n, 4*n , n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::PELLET_FLASH, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::PELLET_FLASH, { 2*n +((float)i * n), 4*n , n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::EYES_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::EYES_LEFT, { 0, 5 * n , n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::EYES_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::EYES_RIGHT, { n, 5 * n , n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::EYES_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::EYES_UP, { 2*n, 5 * n , n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::EYES_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::EYES_DOWN, { 3*n, 5 * n , n, n });

	return AppStatus::OK;
}

void ENEMIGO::IntroUpdate(bool turn) 
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (turn) {
		pos.x += ((ENEMY_SPEED*2) - 1);
		if (state != State_e::PELLET) {
			state = State_e::PELLET;
			SetAnimation((int)EnemyAnim::PELLET);
		}

		if (introCaught) {
			SetAnimation((int)EnemyAnim::HIDDEN);
			state = State_e::WALKING;
		}
		else {
			if (state == State_e::IDLE) StartWalkingRight();
			else {
				if (!IsLookingRight()) ChangeAnimRight();
			}
		}
	}
	else {
		pos.x += -(ENEMY_SPEED * 2);
		if (state == State_e::IDLE) StartWalkingLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}
	}
}

void ENEMIGO::UpdateLook(int anim_id)
{
	EnemyAnim anim = (EnemyAnim)anim_id;
	look = (anim == EnemyAnim::WALKING_LEFT) ? Look_e::LEFT : Look_e::RIGHT;
}

void ENEMIGO::Intro(int count) {
	if (count <= 60) SetAnimation((int)EnemyAnim::IDLE);
	else SetAnimation((int)EnemyAnim::HIDDEN);
}

Point ENEMIGO::GetEnemyPos() 
{
	return pos;
}

void ENEMIGO::SetNormal()
{
	state = State_e::IDLE;
	SetAnimation((int)EnemyAnim::IDLE);
}

void ENEMIGO::SetTarget(Point t) 
{
	target = t;
}

void ENEMIGO::SetTargetExit()
{
	useDoor = true;
	target = home_exit;
}

void ENEMIGO::SetHome(Point t)
{
	home = t;
}

void ENEMIGO::SetHomeExit(Point t)
{
	home_exit = t;
}

bool ENEMIGO::IsDead() {
	return(state == State_e::EYES);
}

void ENEMIGO::Pellet(bool ifPellet, int count) {
	if (!caught) {
		if (ifPellet) {
			state = State_e::PELLET;
			vulnearble = true;
			if (count < 300) {
				if (flash) SetAnimation((int)EnemyAnim::PELLET_FLASH);
				else SetAnimation((int)EnemyAnim::PELLET);
				--delay;
				if (delay <= 0) {
					delay = 30;
					if (flash) flash = false;
					else flash = true;
				}

			}
			else {
				SetAnimation((int)EnemyAnim::PELLET);
			}
		}
		else {
			state = State_e::WALKING;
			switch (look) {
			case Look_e::DOWN:
				ChangeAnimDown();
				break;
			case Look_e::UP:
				ChangeAnimUp();
				break;
			case Look_e::RIGHT:
				ChangeAnimRight();
				break;
			case Look_e::LEFT:
				ChangeAnimLeft();
				break;
			}
			vulnearble = false;
		}
	}
	else {
		if (state != State_e::EYES) {
			state = State_e::WALKING;
			switch (look) {
			case Look_e::DOWN:
				ChangeAnimDown();
				break;
			case Look_e::UP:
				ChangeAnimUp();
				break;
			case Look_e::RIGHT:
				ChangeAnimRight();
				break;
			case Look_e::LEFT:
				ChangeAnimLeft();
				break;
			}
			vulnearble = false;
		}
	}
}

void ENEMIGO::WinLose() {
	SetAnimation((int)EnemyAnim::HIDDEN);
}

void ENEMIGO::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}

bool ENEMIGO::IsLookingRight() const
{
	return look == Look_e::RIGHT;
}
bool ENEMIGO::IsLookingLeft() const
{
	return look == Look_e::LEFT;
}
bool ENEMIGO::IsLookingUp() const
{
	return look == Look_e::UP;
}
bool ENEMIGO::IsLookingDown() const
{
	return look == Look_e::DOWN;
}
void ENEMIGO::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
EnemyAnim ENEMIGO::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (EnemyAnim)sprite->GetAnimation();
}
void ENEMIGO::Stop()
{
	dir = { 0,0 };
	if (state != State_e::PELLET) {
		state = State_e::IDLE;
		SetAnimation((int)EnemyAnim::IDLE);
	}
}
void ENEMIGO::StartWalkingLeft()
{
	look = Look_e::LEFT;
	if (state != State_e::PELLET) {
		state = State_e::WALKING;
		SetAnimation((int)EnemyAnim::WALKING_LEFT);
	}
}
void ENEMIGO::StartWalkingRight()
{
	look = Look_e::RIGHT;
	if (state != State_e::PELLET) {
		state = State_e::WALKING;
		SetAnimation((int)EnemyAnim::WALKING_RIGHT);
	}
}
void ENEMIGO::StartWalkingUp()
{
	look = Look_e::UP;
	if (state != State_e::PELLET) {
		state = State_e::WALKING;
		SetAnimation((int)EnemyAnim::WALKING_UP);
	}
}
void ENEMIGO::StartWalkingDown()
{
	look = Look_e::DOWN;
	if (state != State_e::PELLET) {
		state = State_e::WALKING;
		SetAnimation((int)EnemyAnim::WALKING_DOWN);
	}
}
void ENEMIGO::StartDying()
{
	state = State_e::EYES;

	//SetAnimation((int)EnemyAnim::PELLET);
}
void ENEMIGO::ChangeAnimRight()
{
	look = Look_e::RIGHT;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_RIGHT); break;
	case State_e::PELLET: SetAnimation((int)EnemyAnim::PELLET); break;
	case State_e::EYES: SetAnimation((int)EnemyAnim::EYES_RIGHT); break;
	}
}
void ENEMIGO::ChangeAnimLeft()
{
	look = Look_e::LEFT;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_LEFT); break;
	case State_e::PELLET: SetAnimation((int)EnemyAnim::PELLET); break;
	case State_e::EYES: SetAnimation((int)EnemyAnim::EYES_LEFT); break;
	}
}
void ENEMIGO::ChangeAnimUp()
{
	look = Look_e::UP;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_UP); break;
	case State_e::PELLET: SetAnimation((int)EnemyAnim::PELLET); break;
	case State_e::EYES: SetAnimation((int)EnemyAnim::EYES_UP); break;
	}
}
void ENEMIGO::ChangeAnimDown()
{
	look = Look_e::DOWN;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_DOWN); break;
	case State_e::PELLET: SetAnimation((int)EnemyAnim::PELLET); break;
	case State_e::EYES: SetAnimation((int)EnemyAnim::EYES_DOWN); break;
	}
}
void ENEMIGO::Update(Point pacmanDir, Point pacmanPos, Point blinkypos)
{
	//all movement in move
	Move(pacmanDir, pacmanPos, blinkypos);

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
float ENEMIGO::GetTargetDistance(Point dir) 
{
	Point check = pos;
	check.x += (dir.x * ENEMY_SPEED);
	check.y += (dir.y * ENEMY_SPEED);
	return static_cast<float>(sqrt(pow(check.x - target.x, 2) + pow(check.y - target.y, 2)));
}
void ENEMIGO::Move(Point pacmanDir, Point pacmanPos, Point blinkypos)
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;
	int availableWays = 0;
	Point optimalWay;
	Look_e backwards;

	if (caught) {
		if (state != State_e::EYES) {
			state = State_e::EYES;
			switch (look) {
			case Look_e::LEFT:
				ChangeAnimLeft();
				break;
			case Look_e::RIGHT:
				ChangeAnimRight();
				break;
			case Look_e::UP:
				ChangeAnimUp();
				break;
			case Look_e::DOWN:
				ChangeAnimDown();
				break;
			}
		}
		if (home.x == pos.x and home.y == pos.y) {
			caught = false;
			if (IsSoundPlaying(sound_retreat)) StopSound(sound_retreat);
			useDoor = false;
			target = home_exit;
			state = State_e::WALKING;
			switch (look) {
			case Look_e::LEFT:
				ChangeAnimLeft();
				break;
			case Look_e::RIGHT:
				ChangeAnimRight();
				break;
			case Look_e::UP:
				ChangeAnimUp();
				break;
			case Look_e::DOWN:
				ChangeAnimDown();
				break;
			}
		}
		target = home;
		if (!IsSoundPlaying(sound_retreat)) PlaySound(sound_retreat);
	} 
	
	UpdateTarget(pacmanDir, pacmanPos, blinkypos);

	if (pacmanPos.x <= (pos.x + TILE_SIZE * 10) and pacmanPos.x >= (pos.x - TILE_SIZE * 10)) {
		if (pacmanPos.y <= (pos.y + TILE_SIZE * 10) and pacmanPos.y >= (pos.y - TILE_SIZE * 10)) mode = Mode_e::BUSCAR;
		else mode = Mode_e::ENCONTRAR;
	}
	else mode = Mode_e::ENCONTRAR;

	if (state != State_e::PELLET) {
		Point direction;
		Look_e checking;

		switch (look) {
		case Look_e::LEFT:
			optimalWay = { -1, 0 };
			backwards = Look_e::RIGHT;
			break;
		case Look_e::RIGHT:
			optimalWay = { 1, 0 };
			backwards = Look_e::LEFT;
			break;
		case Look_e::UP:
			optimalWay = { 0, -1 };
			backwards = Look_e::DOWN;
			break;
		case Look_e::DOWN:
			optimalWay = { 0, 1 };
			backwards = Look_e::UP;
			break;
		}

		for (int i = 0; i < 4; ++i) {
			if (i != (int)backwards) {
				checking = (Look_e)i;
				bool possible = false;
				switch (checking) {
				case Look_e::LEFT:
					pos.x += -ENEMY_SPEED;
					box = GetHitbox();
					if (!map->TestCollisionWallLeft(box)) {
						availableWays++;
						possible = true;
					}
					pos.x = prev_x;
					direction = { -1, 0 };
					break;
				case Look_e::RIGHT:
					pos.x += ENEMY_SPEED;
					box = GetHitbox();
					if (!map->TestCollisionWallRight(box)) {
						availableWays++;
						possible = true;
					}
					pos.x = prev_x;
					direction = { 1, 0 };
					break;
				case Look_e::UP:
					pos.y -= ENEMY_SPEED;
					box = GetHitbox();
					if (!map->TestCollisionWallUp(box, useDoor)) {
						availableWays++;
						possible = true;
					}
					pos.y = prev_y;
					direction = { 0, -1 };
					break;
				case Look_e::DOWN:
					pos.y += ENEMY_SPEED;
					box = GetHitbox();
					if (!map->TestCollisionWallDown(box, useDoor)) {
						availableWays++;
						possible = true;
					}
					pos.y = prev_y;
					direction = { 0, 1 };
					break;
				}
				if (possible) {
					if (GetTargetDistance(direction) < GetTargetDistance(optimalWay)) optimalWay = direction;
				}
			}
		}

		if (availableWays > 0) {
			if (optimalWay.x == 0) {
				if (optimalWay.y == 1) {
					pos.y += ENEMY_SPEED;
					if (state == State_e::IDLE) StartWalkingDown();
					else {
						if (!IsLookingDown()) ChangeAnimDown();
					}
				}
				else {
					pos.y -= ENEMY_SPEED;
					if (state == State_e::IDLE) StartWalkingUp();
					else {
						if (!IsLookingUp()) ChangeAnimUp();
					}
				}
			}
			else {
				if (optimalWay.x == 1) {
					pos.x += ENEMY_SPEED;
					if (state == State_e::IDLE) StartWalkingRight();
					else {
						if (!IsLookingRight()) ChangeAnimRight();
					}
					if (pos.x == WINDOW_WIDTH - 9) {
						pos.x = 1;
						ChangeAnimLeft();
					}
				}
				else {
					pos.x += -ENEMY_SPEED;
					if (state == State_e::IDLE) StartWalkingLeft();
					else {
						if (!IsLookingLeft()) ChangeAnimLeft();
					}
					if (pos.x == 1) {
						pos.x = WINDOW_WIDTH - 9;
						ChangeAnimRight();
					}
				}
			}
		}
		else {
			switch (backwards) {
			case Look_e::LEFT:
				pos.x += -ENEMY_SPEED;
				if (state == State_e::IDLE) StartWalkingLeft();
				else {
					if (!IsLookingLeft()) ChangeAnimLeft();
				}
				break;
			case Look_e::RIGHT:
				pos.x += ENEMY_SPEED;
				if (state == State_e::IDLE) StartWalkingRight();
				else {
					if (!IsLookingRight()) ChangeAnimRight();
				}
				break;
			case Look_e::UP:
				pos.y += -ENEMY_SPEED;
				if (state == State_e::IDLE) StartWalkingUp();
				else {
					if (!IsLookingUp()) ChangeAnimUp();
				}
				break;
			case Look_e::DOWN:
				pos.y += ENEMY_SPEED;
				if (state == State_e::IDLE) StartWalkingDown();
				else {
					if (!IsLookingDown()) ChangeAnimDown();
				}
				break;
			}
		}
	}
 else {
	 bool canMove = false;
	 bool notFound = false;
	 std::vector<Look_e> possible;
	 Look_e targetDir;
	 int random;
	 int i = 0;

	 switch (look) {
	 case Look_e::LEFT:
		 optimalWay = { -1, 0 };
		 backwards = Look_e::RIGHT;
		 break;
	 case Look_e::RIGHT:
		 optimalWay = { 1, 0 };
		 backwards = Look_e::LEFT;
		 break;
	 case Look_e::UP:
		 optimalWay = { 0, -1 };
		 backwards = Look_e::DOWN;
		 break;
	 case Look_e::DOWN:
		 optimalWay = { 0, 1 };
		 backwards = Look_e::UP;
		 break;
	 }

	 for (int i = 0; i < 4; ++i) {
		 if (i != (int)backwards) {
			 targetDir = (Look_e)i;
			 switch (targetDir) {
			 case Look_e::LEFT:
				 pos.x += -(ENEMY_SPEED + 1);
				 box = GetHitbox();
				 if (!map->TestCollisionWallLeft(box)) possible.push_back(targetDir);
				 pos.x = prev_x;
				 break;
			 case Look_e::RIGHT:
				 pos.x += (ENEMY_SPEED + 1);
				 box = GetHitbox();
				 if (!map->TestCollisionWallRight(box)) possible.push_back(targetDir);
				 pos.x = prev_x;
				 break;
			 case Look_e::UP:
				 pos.y += -(ENEMY_SPEED + 1);
				 box = GetHitbox();
				 if (!map->TestCollisionWallUp(box, useDoor)) possible.push_back(targetDir);
				 pos.y = prev_y;
				 break;
			 case Look_e::DOWN:
				 pos.y += (ENEMY_SPEED + 1);
				 box = GetHitbox();
				 if (!map->TestCollisionWallDown(box, useDoor)) possible.push_back(targetDir);
				 pos.y = prev_y;
				 break;
			 }
		 }
	 }

	 if (possible.size() > 0) {
		 random = GetRandomValue(0, possible.size() - 1);
		 targetDir = possible[random];
	 }
	 else targetDir = backwards;
	 
	 switch (targetDir) {
	 case Look_e::LEFT:
		 pos.x += -(ENEMY_SPEED + 1);
		 if (state == State_e::IDLE) StartWalkingLeft();
		 else {
			 if (!IsLookingLeft()) ChangeAnimLeft();
		 }
		 if (pos.x == 1) {
			 pos.x = WINDOW_WIDTH - 9;
			 ChangeAnimRight();
		 }
		 break;
	 case Look_e::RIGHT:
		 pos.x += (ENEMY_SPEED + 1);
		 if (state == State_e::IDLE) StartWalkingRight();
		 else {
			 if (!IsLookingRight()) ChangeAnimRight();
		 }
		 if (pos.x == WINDOW_WIDTH - 9) {
			 pos.x = 1;
			 ChangeAnimLeft();
		 }
		 break;
	 case Look_e::UP:
		 pos.y += -(ENEMY_SPEED + 1);
		 if (state == State_e::IDLE) StartWalkingUp();
		 else {
			 if (!IsLookingUp()) ChangeAnimUp();
		 }
		 break;
	 case Look_e::DOWN:
		 pos.y += (ENEMY_SPEED + 1);
		 if (state == State_e::IDLE) StartWalkingDown();
		 else {
			 if (!IsLookingDown()) ChangeAnimDown();
		 }
		 break;
	 }
}
 
}

void ENEMIGO::UpdateTarget(Point pacmanDir, Point pacmanPos, Point blinkypos) 
{
	if (useDoor) {
		if (pos.x == target.x and pos.y == target.y-1) useDoor = false;
		else if (home.x == target.x and home.y == target.y-1) {
			state = State_e::IDLE;
			caught = false;
			target = home_exit;
		}
	}
	else if(state != State_e::EYES){
		if (mode == Mode_e::ENCONTRAR) {
			switch (type) {
			case EnemyType::AZULITO:
				target = { TILE_SIZE * (LEVEL_WIDTH - 1), 0 };
				break;
			case EnemyType::ROSA:
				target = { 0,0 };
				break;
			case EnemyType::ROJO:
				target = { TILE_SIZE * (LEVEL_WIDTH - 1), TILE_SIZE * (LEVEL_HEIGHT - 1) };
				break;
			case EnemyType::NARANJITA:
				target = { 0, TILE_SIZE * (LEVEL_HEIGHT - 1) };
				break;
			}
		}
		else {
			switch (type) {
			case EnemyType::AZULITO:
				target = pacmanPos;
				break;
			case EnemyType::ROSA:
				target = pacmanPos + (pacmanDir * TILE_SIZE * 4);
				break;
			case EnemyType::ROJO:
				target = pacmanPos + (pacmanDir * TILE_SIZE * 2);
				target.x += target.x - blinkypos.x;
				target.y += target.y - blinkypos.y;
				break;
			case EnemyType::NARANJITA:
				if (pacmanPos.x <= (pos.x + TILE_SIZE * 4) and pacmanPos.x >= (pos.x - TILE_SIZE * 4)) {
					if (pacmanPos.y <= (pos.y + TILE_SIZE * 4) and pacmanPos.y >= (pos.y - TILE_SIZE * 4)) target = { 0, TILE_SIZE * (LEVEL_HEIGHT - 1) };
					else target = pacmanPos;
				}
				else target = pacmanPos;
				break;
			}
		}
	}
}

void ENEMIGO::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}


void ENEMIGO::Release()
{
	if (IsSoundPlaying(sound_retreat)) StopSound(sound_retreat);
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ENEMY);

	render->Release();
}