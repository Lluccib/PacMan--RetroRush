#include "Enemys.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include <cmath>

ENEMIGO::ENEMIGO(const Point& p, Estado s, Mirada view, TipoEnemigo t) :
	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	vista = view;
	mapa = nullptr;
	type = t;
	if (type != TipoEnemigo::AZULITO) Valla = true;
	if (type != TipoEnemigo::NARANJITA) mode = Modo::BUSCAR;
	else mode = Modo::ENCONTRAR;
	
}
ENEMIGO::~ENEMIGO()
{
}
AppStatus ENEMIGO::Initialise()
{
	const int n = ENEMY_FRAME_SIZE;
	int k;
	switch (type) {
	case TipoEnemigo::AZULITO:
		k = n * 2;
		break;
	case TipoEnemigo::ROSA:
		k = n;
		break;
	case TipoEnemigo::ROJO:
		k = 0;
		break;
	case TipoEnemigo::NARANJITA:
		k = n * 3;
		break;
	}

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMY, "Assets/sprites/fantasmasfull.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	retratado = data.GetSound(AudioResource::AUD_RETREAT);

	render = new Sprite(data.GetTexture(Resource::IMG_ENEMY));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)AnimacionesEnemigos::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::IDLE, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::IDLE, { n*4, (float)k, n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::ESCONDIDO, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::ESCONDIDO, { n * 4, 5*n, n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::VADERECHA, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::VADERECHA, { (float)i*n, (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::VAIZQUIERDA, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::VAIZQUIERDA, { 2 * n + ((float)i*n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::VAARRIBA, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::VAARRIBA, { 4 * n + ((float)i * n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::VAABAJO, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::VAABAJO, { 6 * n + ((float)i * n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::CEREZA, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::CEREZA, { (float)i * n, 4*n , n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::CEREZAS, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::CEREZAS, { 2*n +((float)i * n), 4*n , n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::IZQUIERDA, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::IZQUIERDA, { 0, 5 * n , n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::DERECHA, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::DERECHA, { n, 5 * n , n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::ARRIBA, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::ARRIBA, { 2*n, 5 * n , n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::ABAJO, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::ABAJO, { 3*n, 5 * n , n, n });

	return AppStatus::OK;
}

void ENEMIGO::UPDATEintro(bool turn) 
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (turn) {
		pos.x += ((VELOCIDAD_ENEMIGO*2) - 1);
		if (state != Estado::PELLET) {
			state = Estado::PELLET;
			SetAnimation((int)AnimacionesEnemigos::CEREZA);
		}

		if (pilladointro) {
			SetAnimation((int)AnimacionesEnemigos::ESCONDIDO);
			state = Estado::WALKING;
		}
		else {
			if (state == Estado::IDLE) StartWalkingRight();
			else {
				if (!IsLookingRight()) ChangeAnimRight();
			}
		}
	}
	else {
		pos.x += -(VELOCIDAD_ENEMIGO * 2);
		if (state == Estado::IDLE) StartWalkingLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}
	}
}

void ENEMIGO::UpdateLook(int anim_id)
{
	AnimacionesEnemigos anim = (AnimacionesEnemigos)anim_id;
	vista = (anim == AnimacionesEnemigos::VAIZQUIERDA) ? Mirada::LEFT : Mirada::RIGHT;
}

void ENEMIGO::Intro(int count) {
	if (count <= 60) SetAnimation((int)AnimacionesEnemigos::IDLE);
	else SetAnimation((int)AnimacionesEnemigos::ESCONDIDO);
}

Point ENEMIGO::GetEnemyPos() 
{
	return pos;
}

void ENEMIGO::A_normal()
{
	state = Estado::IDLE;
	SetAnimation((int)AnimacionesEnemigos::IDLE);
}

void ENEMIGO::EstablecerObjetivo(Point t) 
{
	objetivo = t;
}

void ENEMIGO::EstablecerSalida()
{
	Valla = true;
	objetivo = salida_casa;
}

void ENEMIGO::EstablecerCasa(Point t)
{
	casa = t;
}

void ENEMIGO::EstablecerSalidaCasa(Point t)
{
	salida_casa = t;
}

bool ENEMIGO::TaMuerto() {
	return(state == Estado::EYES);
}

void ENEMIGO::Pellet(bool ifPellet, int count) {
	if (!pillado) {
		if (ifPellet) {
			state = Estado::PELLET;
			comible = true;
			if (count < 300) {
				if (lucecita) SetAnimation((int)AnimacionesEnemigos::CEREZAS);
				else SetAnimation((int)AnimacionesEnemigos::CEREZA);
				--retardo;
				if (retardo <= 0) {
					retardo = 30;
					if (lucecita) lucecita = false;
					else lucecita = true;
				}

			}
			else {
				SetAnimation((int)AnimacionesEnemigos::CEREZA);
			}
		}
		else {
			state = Estado::WALKING;
			switch (vista) {
			case Mirada::DOWN:
				ChangeAnimDown();
				break;
			case Mirada::UP:
				ChangeAnimUp();
				break;
			case Mirada::RIGHT:
				ChangeAnimRight();
				break;
			case Mirada::LEFT:
				ChangeAnimLeft();
				break;
			}
			comible = false;
		}
	}
	else {
		if (state != Estado::EYES) {
			state = Estado::WALKING;
			switch (vista) {
			case Mirada::DOWN:
				ChangeAnimDown();
				break;
			case Mirada::UP:
				ChangeAnimUp();
				break;
			case Mirada::RIGHT:
				ChangeAnimRight();
				break;
			case Mirada::LEFT:
				ChangeAnimLeft();
				break;
			}
			comible = false;
		}
	}
}

void ENEMIGO::WinLose() {
	SetAnimation((int)AnimacionesEnemigos::ESCONDIDO);
}

void ENEMIGO::SetTileMap(TileMap* tilemap)
{
	mapa = tilemap;
}

bool ENEMIGO::IsLookingRight() const
{
	return vista == Mirada::RIGHT;
}
bool ENEMIGO::IsLookingLeft() const
{
	return vista == Mirada::LEFT;
}
bool ENEMIGO::IsLookingUp() const
{
	return vista == Mirada::UP;
}
bool ENEMIGO::IsLookingDown() const
{
	return vista == Mirada::DOWN;
}
void ENEMIGO::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
AnimacionesEnemigos ENEMIGO::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (AnimacionesEnemigos)sprite->GetAnimation();
}
void ENEMIGO::Parar()
{
	dir = { 0,0 };
	if (state != Estado::PELLET) {
		state = Estado::IDLE;
		SetAnimation((int)AnimacionesEnemigos::IDLE);
	}
}
void ENEMIGO::StartWalkingLeft()
{
	vista = Mirada::LEFT;
	if (state != Estado::PELLET) {
		state = Estado::WALKING;
		SetAnimation((int)AnimacionesEnemigos::VAIZQUIERDA);
	}
}
void ENEMIGO::StartWalkingRight()
{
	vista = Mirada::RIGHT;
	if (state != Estado::PELLET) {
		state = Estado::WALKING;
		SetAnimation((int)AnimacionesEnemigos::VADERECHA);
	}
}
void ENEMIGO::StartWalkingUp()
{
	vista = Mirada::UP;
	if (state != Estado::PELLET) {
		state = Estado::WALKING;
		SetAnimation((int)AnimacionesEnemigos::VAARRIBA);
	}
}
void ENEMIGO::StartWalkingDown()
{
	vista = Mirada::DOWN;
	if (state != Estado::PELLET) {
		state = Estado::WALKING;
		SetAnimation((int)AnimacionesEnemigos::VAABAJO);
	}
}
void ENEMIGO::EmpiezaMorir()
{
	state = Estado::EYES;
}

void ENEMIGO::ChangeAnimUp()
{
	vista = Mirada::UP;
	switch (state)
	{
	case Estado::IDLE:	 SetAnimation((int)AnimacionesEnemigos::IDLE);    break;
	case Estado::WALKING: SetAnimation((int)AnimacionesEnemigos::VAARRIBA); break;
	case Estado::PELLET: SetAnimation((int)AnimacionesEnemigos::CEREZA); break;
	case Estado::EYES: SetAnimation((int)AnimacionesEnemigos::ARRIBA); break;
	}
}
void ENEMIGO::ChangeAnimDown()
{
	vista = Mirada::DOWN;
	switch (state)
	{
	case Estado::IDLE:	 SetAnimation((int)AnimacionesEnemigos::IDLE);    break;
	case Estado::WALKING: SetAnimation((int)AnimacionesEnemigos::VAABAJO); break;
	case Estado::PELLET: SetAnimation((int)AnimacionesEnemigos::CEREZA); break;
	case Estado::EYES: SetAnimation((int)AnimacionesEnemigos::ABAJO); break;
	}
}
void ENEMIGO::ChangeAnimRight()
{
	vista = Mirada::RIGHT;
	switch (state)
	{
	case Estado::IDLE:	 SetAnimation((int)AnimacionesEnemigos::IDLE);    break;
	case Estado::WALKING: SetAnimation((int)AnimacionesEnemigos::VADERECHA); break;
	case Estado::PELLET: SetAnimation((int)AnimacionesEnemigos::CEREZA); break;
	case Estado::EYES: SetAnimation((int)AnimacionesEnemigos::DERECHA); break;
	}
}
void ENEMIGO::ChangeAnimLeft()
{
	vista = Mirada::LEFT;
	switch (state)
	{
	case Estado::IDLE:	 SetAnimation((int)AnimacionesEnemigos::IDLE);    break;
	case Estado::WALKING: SetAnimation((int)AnimacionesEnemigos::VAIZQUIERDA); break;
	case Estado::PELLET: SetAnimation((int)AnimacionesEnemigos::CEREZA); break;
	case Estado::EYES: SetAnimation((int)AnimacionesEnemigos::IZQUIERDA); break;
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
	check.x += (dir.x * VELOCIDAD_ENEMIGO);
	check.y += (dir.y * VELOCIDAD_ENEMIGO);
	return static_cast<float>(sqrt(pow(check.x - objetivo.x, 2) + pow(check.y - objetivo.y, 2)));
}
void ENEMIGO::Move(Point pacmanDir, Point pacmanPos, Point blinkypos)
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;
	int availableWays = 0;
	Point optimalWay;
	Mirada backwards;

	if (pillado) {
		if (state != Estado::EYES) {
			state = Estado::EYES;
			switch (vista) {
			case Mirada::LEFT:
				ChangeAnimLeft();
				break;
			case Mirada::RIGHT:
				ChangeAnimRight();
				break;
			case Mirada::UP:
				ChangeAnimUp();
				break;
			case Mirada::DOWN:
				ChangeAnimDown();
				break;
			}
		}
		if (casa.x == pos.x and casa.y == pos.y) {
			pillado = false;
			if (IsSoundPlaying(retratado)) StopSound(retratado);
			Valla = false;
			objetivo = salida_casa;
			state = Estado::WALKING;
			switch (vista) {
			case Mirada::LEFT:
				ChangeAnimLeft();
				break;
			case Mirada::RIGHT:
				ChangeAnimRight();
				break;
			case Mirada::UP:
				ChangeAnimUp();
				break;
			case Mirada::DOWN:
				ChangeAnimDown();
				break;
			}
		}
		objetivo = casa;
		if (!IsSoundPlaying(retratado)) PlaySound(retratado);
	} 
	
	UpdateTarget(pacmanDir, pacmanPos, blinkypos);

	if (pacmanPos.x <= (pos.x + TILE_SIZE * 10) and pacmanPos.x >= (pos.x - TILE_SIZE * 10)) {
		if (pacmanPos.y <= (pos.y + TILE_SIZE * 10) and pacmanPos.y >= (pos.y - TILE_SIZE * 10)) mode = Modo::BUSCAR;
		else mode = Modo::ENCONTRAR;
	}
	else mode = Modo::ENCONTRAR;

	if (state != Estado::PELLET) {
		Point direction;
		Mirada checking;

		switch (vista) {
		case Mirada::LEFT:
			optimalWay = { -1, 0 };
			backwards = Mirada::RIGHT;
			break;
		case Mirada::RIGHT:
			optimalWay = { 1, 0 };
			backwards = Mirada::LEFT;
			break;
		case Mirada::UP:
			optimalWay = { 0, -1 };
			backwards = Mirada::DOWN;
			break;
		case Mirada::DOWN:
			optimalWay = { 0, 1 };
			backwards = Mirada::UP;
			break;
		}

		for (int i = 0; i < 4; ++i) {
			if (i != (int)backwards) {
				checking = (Mirada)i;
				bool possible = false;
				switch (checking) {
				case Mirada::LEFT:
					pos.x += -VELOCIDAD_ENEMIGO;
					box = GetHitbox();
					if (!mapa->TestCollisionWallLeft(box)) {
						availableWays++;
						possible = true;
					}
					pos.x = prev_x;
					direction = { -1, 0 };
					break;
				case Mirada::RIGHT:
					pos.x += VELOCIDAD_ENEMIGO;
					box = GetHitbox();
					if (!mapa->TestCollisionWallRight(box)) {
						availableWays++;
						possible = true;
					}
					pos.x = prev_x;
					direction = { 1, 0 };
					break;
				case Mirada::UP:
					pos.y -= VELOCIDAD_ENEMIGO;
					box = GetHitbox();
					if (!mapa->TestCollisionWallUp(box, Valla)) {
						availableWays++;
						possible = true;
					}
					pos.y = prev_y;
					direction = { 0, -1 };
					break;
				case Mirada::DOWN:
					pos.y += VELOCIDAD_ENEMIGO;
					box = GetHitbox();
					if (!mapa->TestCollisionWallDown(box, Valla)) {
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
					pos.y += VELOCIDAD_ENEMIGO;
					if (state == Estado::IDLE) StartWalkingDown();
					else {
						if (!IsLookingDown()) ChangeAnimDown();
					}
				}
				else {
					pos.y -= VELOCIDAD_ENEMIGO;
					if (state == Estado::IDLE) StartWalkingUp();
					else {
						if (!IsLookingUp()) ChangeAnimUp();
					}
				}
			}
			else {
				if (optimalWay.x == 1) {
					pos.x += VELOCIDAD_ENEMIGO;
					if (state == Estado::IDLE) StartWalkingRight();
					else {
						if (!IsLookingRight()) ChangeAnimRight();
					}
					if (pos.x == WINDOW_WIDTH - 9) {
						pos.x = 1;
						ChangeAnimLeft();
					}
				}
				else {
					pos.x += -VELOCIDAD_ENEMIGO;
					if (state == Estado::IDLE) StartWalkingLeft();
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
			case Mirada::LEFT:
				pos.x += -VELOCIDAD_ENEMIGO;
				if (state == Estado::IDLE) StartWalkingLeft();
				else {
					if (!IsLookingLeft()) ChangeAnimLeft();
				}
				break;
			case Mirada::RIGHT:
				pos.x += VELOCIDAD_ENEMIGO;
				if (state == Estado::IDLE) StartWalkingRight();
				else {
					if (!IsLookingRight()) ChangeAnimRight();
				}
				break;
			case Mirada::UP:
				pos.y += -VELOCIDAD_ENEMIGO;
				if (state == Estado::IDLE) StartWalkingUp();
				else {
					if (!IsLookingUp()) ChangeAnimUp();
				}
				break;
			case Mirada::DOWN:
				pos.y += VELOCIDAD_ENEMIGO;
				if (state == Estado::IDLE) StartWalkingDown();
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
	 std::vector<Mirada> possible;
	 Mirada targetDir;
	 int random;
	 int i = 0;

	 switch (vista) {
	 case Mirada::LEFT:
		 optimalWay = { -1, 0 };
		 backwards = Mirada::RIGHT;
		 break;
	 case Mirada::RIGHT:
		 optimalWay = { 1, 0 };
		 backwards = Mirada::LEFT;
		 break;
	 case Mirada::UP:
		 optimalWay = { 0, -1 };
		 backwards = Mirada::DOWN;
		 break;
	 case Mirada::DOWN:
		 optimalWay = { 0, 1 };
		 backwards = Mirada::UP;
		 break;
	 }

	 for (int i = 0; i < 4; ++i) {
		 if (i != (int)backwards) {
			 targetDir = (Mirada)i;
			 switch (targetDir) {
			 case Mirada::LEFT:
				 pos.x += -(VELOCIDAD_ENEMIGO + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallLeft(box)) possible.push_back(targetDir);
				 pos.x = prev_x;
				 break;
			 case Mirada::RIGHT:
				 pos.x += (VELOCIDAD_ENEMIGO + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallRight(box)) possible.push_back(targetDir);
				 pos.x = prev_x;
				 break;
			 case Mirada::UP:
				 pos.y += -(VELOCIDAD_ENEMIGO + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallUp(box, Valla)) possible.push_back(targetDir);
				 pos.y = prev_y;
				 break;
			 case Mirada::DOWN:
				 pos.y += (VELOCIDAD_ENEMIGO + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallDown(box, Valla)) possible.push_back(targetDir);
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
	 case Mirada::LEFT:
		 pos.x += -(VELOCIDAD_ENEMIGO + 1);
		 if (state == Estado::IDLE) StartWalkingLeft();
		 else {
			 if (!IsLookingLeft()) ChangeAnimLeft();
		 }
		 if (pos.x == 1) {
			 pos.x = WINDOW_WIDTH - 9;
			 ChangeAnimRight();
		 }
		 break;
	 case Mirada::RIGHT:
		 pos.x += (VELOCIDAD_ENEMIGO + 1);
		 if (state == Estado::IDLE) StartWalkingRight();
		 else {
			 if (!IsLookingRight()) ChangeAnimRight();
		 }
		 if (pos.x == WINDOW_WIDTH - 9) {
			 pos.x = 1;
			 ChangeAnimLeft();
		 }
		 break;
	 case Mirada::UP:
		 pos.y += -(VELOCIDAD_ENEMIGO + 1);
		 if (state == Estado::IDLE) StartWalkingUp();
		 else {
			 if (!IsLookingUp()) ChangeAnimUp();
		 }
		 break;
	 case Mirada::DOWN:
		 pos.y += (VELOCIDAD_ENEMIGO + 1);
		 if (state == Estado::IDLE) StartWalkingDown();
		 else {
			 if (!IsLookingDown()) ChangeAnimDown();
		 }
		 break;
	 }
}
 
}

void ENEMIGO::UpdateTarget(Point pacmanDir, Point pacmanPos, Point blinkypos) 
{
	if (Valla) {
		if (pos.x == objetivo.x and pos.y == objetivo.y-1) Valla = false;
		else if (casa.x == objetivo.x and casa.y == objetivo.y-1) {
			state = Estado::IDLE;
			pillado = false;
			objetivo = salida_casa;
		}
	}
	else if(state != Estado::EYES){
		if (mode == Modo::ENCONTRAR) {
			switch (type) {
			case TipoEnemigo::AZULITO:
				objetivo = { TILE_SIZE * (LEVEL_WIDTH - 1), 0 };
				break;
			case TipoEnemigo::ROSA:
				objetivo = { 0,0 };
				break;
			case TipoEnemigo::ROJO:
				objetivo = { TILE_SIZE * (LEVEL_WIDTH - 1), TILE_SIZE * (LEVEL_HEIGHT - 1) };
				break;
			case TipoEnemigo::NARANJITA:
				objetivo = { 0, TILE_SIZE * (LEVEL_HEIGHT - 1) };
				break;
			}
		}
		else {
			switch (type) {
			case TipoEnemigo::AZULITO:
				objetivo = pacmanPos;
				break;
			case TipoEnemigo::ROSA:
				objetivo = pacmanPos + (pacmanDir * TILE_SIZE * 4);
				break;
			case TipoEnemigo::ROJO:
				objetivo = pacmanPos + (pacmanDir * TILE_SIZE * 2);
				objetivo.x += objetivo.x - blinkypos.x;
				objetivo.y += objetivo.y - blinkypos.y;
				break;
			case TipoEnemigo::NARANJITA:
				if (pacmanPos.x <= (pos.x + TILE_SIZE * 4) and pacmanPos.x >= (pos.x - TILE_SIZE * 4)) {
					if (pacmanPos.y <= (pos.y + TILE_SIZE * 4) and pacmanPos.y >= (pos.y - TILE_SIZE * 4)) objetivo = { 0, TILE_SIZE * (LEVEL_HEIGHT - 1) };
					else objetivo = pacmanPos;
				}
				else objetivo = pacmanPos;
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
	if (IsSoundPlaying(retratado)) StopSound(retratado);
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ENEMY);

	render->Release();
}