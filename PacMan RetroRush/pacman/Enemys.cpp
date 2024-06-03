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
	int G;
	switch (type) {
	case TipoEnemigo::AZULITO:
		G = n * 2;
		break;
	case TipoEnemigo::ROSA:
		G = n;
		break;
	case TipoEnemigo::ROJO:
		G = 0;
		break;
	case TipoEnemigo::NARANJITA:
		G = n * 3;
		break;
	}

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMY, "Assets/sprites/fantasmasfull.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	retratado = data.GetSound(AudioResource::RETRATADO);
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMY));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)AnimacionesEnemigos::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::IDLE, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::IDLE, { n*4, (float)G, n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::ESCONDIDO, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::ESCONDIDO, { n * 4, 5*n, n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::IZQUIERDA, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::IZQUIERDA, { 0, 5 * n , n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::DERECHA, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::DERECHA, { n, 5 * n , n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::ARRIBA, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::ARRIBA, { 2*n, 5 * n , n, n });

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::ABAJO, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesEnemigos::ABAJO, { 3*n, 5 * n , n, n });
	sprite->SetAnimationDelay((int)AnimacionesEnemigos::VADERECHA, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::VADERECHA, { (float)i*n, (float)G, n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::VAIZQUIERDA, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::VAIZQUIERDA, { 2 * n + ((float)i*n), (float)G, n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::VAARRIBA, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::VAARRIBA, { 4 * n + ((float)i * n), (float)G, n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::VAABAJO, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::VAABAJO, { 6 * n + ((float)i * n), (float)G, n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::CEREZA, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::CEREZA, { (float)i * n, 4*n , n, n });
	}

	sprite->SetAnimationDelay((int)AnimacionesEnemigos::CEREZAS, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)AnimacionesEnemigos::CEREZAS, { 2*n +((float)i * n), 4*n , n, n });
	}

	

	return AppStatus::OK;
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
void ENEMIGO::UPDATEintro(bool turn) 
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (turn) {
		position.x += ((VELOCIDAD_ENEMIGO*2) - 1);
		if (state != Estado::PILL1) {
			state = Estado::PILL1;
			SetAnimation((int)AnimacionesEnemigos::CEREZA);
		}

		if (pilladointro) {
			SetAnimation((int)AnimacionesEnemigos::ESCONDIDO);
			state = Estado::ANDANDO;
		}
		else {
			if (state == Estado::QUIETO) StartWalkingRight();
			else {
				if (!IsLookingRight()) ChangeAnimRight();
			}
		}
	}
	else {
		position.x += -(VELOCIDAD_ENEMIGO * 2);
		if (state == Estado::QUIETO) StartWalkingLeft();
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
	return position;
}

void ENEMIGO::A_normal()
{
	state = Estado::QUIETO;
	SetAnimation((int)AnimacionesEnemigos::IDLE);
}

bool ENEMIGO::TaMuerto() {
	return(state == Estado::OJOS);
}

void ENEMIGO::Pellet(bool ifPellet, int count) {
	if (!pillado) {
		if (ifPellet) {
			state = Estado::PILL1;
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
			state = Estado::ANDANDO;
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
		if (state != Estado::OJOS) {
			state = Estado::ANDANDO;
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
	direction = { 0,0 };
	if (state != Estado::PILL1) {
		state = Estado::QUIETO;
		SetAnimation((int)AnimacionesEnemigos::IDLE);
	}
}
void ENEMIGO::StartWalkingLeft()
{
	vista = Mirada::LEFT;
	if (state != Estado::PILL1) {
		state = Estado::ANDANDO;
		SetAnimation((int)AnimacionesEnemigos::VAIZQUIERDA);
	}
}
void ENEMIGO::StartWalkingRight()
{
	vista = Mirada::RIGHT;
	if (state != Estado::PILL1) {
		state = Estado::ANDANDO;
		SetAnimation((int)AnimacionesEnemigos::VADERECHA);
	}
}
void ENEMIGO::StartWalkingUp()
{
	vista = Mirada::UP;
	if (state != Estado::PILL1) {
		state = Estado::ANDANDO;
		SetAnimation((int)AnimacionesEnemigos::VAARRIBA);
	}
}
void ENEMIGO::StartWalkingDown()
{
	vista = Mirada::DOWN;
	if (state != Estado::PILL1) {
		state = Estado::ANDANDO;
		SetAnimation((int)AnimacionesEnemigos::VAABAJO);
	}
}
void ENEMIGO::EmpiezaMorir()
{
	state = Estado::OJOS;
}

void ENEMIGO::ChangeAnimUp()
{
	vista = Mirada::UP;
	switch (state)
	{
	case Estado::QUIETO:	 SetAnimation((int)AnimacionesEnemigos::IDLE);    break;
	case Estado::ANDANDO: SetAnimation((int)AnimacionesEnemigos::VAARRIBA); break;
	case Estado::PILL1: SetAnimation((int)AnimacionesEnemigos::CEREZA); break;
	case Estado::OJOS: SetAnimation((int)AnimacionesEnemigos::ARRIBA); break;
	}
}
void ENEMIGO::ChangeAnimDown()
{
	vista = Mirada::DOWN;
	switch (state)
	{
	case Estado::QUIETO:	 SetAnimation((int)AnimacionesEnemigos::IDLE);    break;
	case Estado::ANDANDO: SetAnimation((int)AnimacionesEnemigos::VAABAJO); break;
	case Estado::PILL1: SetAnimation((int)AnimacionesEnemigos::CEREZA); break;
	case Estado::OJOS: SetAnimation((int)AnimacionesEnemigos::ABAJO); break;
	}
}
void ENEMIGO::ChangeAnimRight()
{
	vista = Mirada::RIGHT;
	switch (state)
	{
	case Estado::QUIETO:	 SetAnimation((int)AnimacionesEnemigos::IDLE);    break;
	case Estado::ANDANDO: SetAnimation((int)AnimacionesEnemigos::VADERECHA); break;
	case Estado::PILL1: SetAnimation((int)AnimacionesEnemigos::CEREZA); break;
	case Estado::OJOS: SetAnimation((int)AnimacionesEnemigos::DERECHA); break;
	}
}
void ENEMIGO::ChangeAnimLeft()
{
	vista = Mirada::LEFT;
	switch (state)
	{
	case Estado::QUIETO:	 SetAnimation((int)AnimacionesEnemigos::IDLE);    break;
	case Estado::ANDANDO: SetAnimation((int)AnimacionesEnemigos::VAIZQUIERDA); break;
	case Estado::PILL1: SetAnimation((int)AnimacionesEnemigos::CEREZA); break;
	case Estado::OJOS: SetAnimation((int)AnimacionesEnemigos::IZQUIERDA); break;
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
	Point check = position;
	check.x += (dir.x * VELOCIDAD_ENEMIGO);
	check.y += (dir.y * VELOCIDAD_ENEMIGO);
	return static_cast<float>(sqrt(pow(check.x - objetivo.x, 2) + pow(check.y - objetivo.y, 2)));
}
void ENEMIGO::Move(Point pacmanDir, Point pacmanPos, Point blinkypos)
{
	AABB box;
	Mirada atras;
	int prev_x = position.x;
	int prev_y = position.y;
	int alternativas = 0;
	Point alternativarapida;
	

	if (pillado) {
		if (state != Estado::OJOS) {
			state = Estado::OJOS;
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
		if (casa.x == position.x and casa.y == position.y) {
			pillado = false;
			if (IsSoundPlaying(retratado)) StopSound(retratado);
			Valla = false;
			objetivo = salida_casa;
			state = Estado::ANDANDO;
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

	if (pacmanPos.x <= (position.x + TILE_SIZE * 10) and pacmanPos.x >= (position.x - TILE_SIZE * 10)) {
		if (pacmanPos.y <= (position.y + TILE_SIZE * 10) and pacmanPos.y >= (position.y - TILE_SIZE * 10)) mode = Modo::BUSCAR;
		else mode = Modo::ENCONTRAR;
	}
	else mode = Modo::ENCONTRAR;

	if (state != Estado::PILL1) {
		Point dir;
		Mirada comprobando;

		switch (vista) {
		case Mirada::LEFT:
			alternativarapida = { -1, 0 };
			atras = Mirada::RIGHT;
			break;
		case Mirada::RIGHT:
			alternativarapida = { 1, 0 };
			atras = Mirada::LEFT;
			break;
		case Mirada::UP:
			alternativarapida = { 0, -1 };
			atras = Mirada::DOWN;
			break;
		case Mirada::DOWN:
			alternativarapida = { 0, 1 };
			atras = Mirada::UP;
			break;
		}



		for (int i = 0; i < 4; ++i) {
			if (i != (int)atras) {
				comprobando = (Mirada)i;
				bool possible = false;
				switch (comprobando) {
				case Mirada::LEFT:
					position.x += -VELOCIDAD_ENEMIGO;
					box = GetHitbox();
					if (!mapa->TestCollisionWallLeft(box)) {
						alternativas++;
						possible = true;
					}
					position.x = prev_x;
					dir = { -1, 0 };
					break;
				case Mirada::RIGHT:
					position.x += VELOCIDAD_ENEMIGO;
					box = GetHitbox();
					if (!mapa->TestCollisionWallRight(box)) {
						alternativas++;
						possible = true;
					}
					position.x = prev_x;
					dir = { 1, 0 };
					break;
				case Mirada::UP:
					position.y -= VELOCIDAD_ENEMIGO;
					box = GetHitbox();
					if (!mapa->TestCollisionWallUp(box, Valla)) {
						alternativas++;
						possible = true;
					}
					position.y = prev_y;
					dir = { 0, -1 };
					break;
				case Mirada::DOWN:
					position.y += VELOCIDAD_ENEMIGO;
					box = GetHitbox();
					if (!mapa->TestCollisionWallDown(box, Valla)) {
						alternativas++;
						possible = true;
					}
					position.y = prev_y;
					dir = { 0, 1 };
					break;
				}
				if (possible) {
					if (GetTargetDistance(dir) < GetTargetDistance(alternativarapida)) alternativarapida = dir;
				}
			}
		}

		if (alternativas > 0) {
			if (alternativarapida.x == 0) {
				if (alternativarapida.y == 1) {
					position.y += VELOCIDAD_ENEMIGO;
					if (state == Estado::QUIETO) StartWalkingDown();
					else {
						if (!IsLookingDown()) ChangeAnimDown();
					}
				}
				else {
					position.y -= VELOCIDAD_ENEMIGO;
					if (state == Estado::QUIETO) StartWalkingUp();
					else {
						if (!IsLookingUp()) ChangeAnimUp();
					}
				}
			}
			else {
				if (alternativarapida.x == 1) {
					position.x += VELOCIDAD_ENEMIGO;
					if (state == Estado::QUIETO) StartWalkingRight();
					else {
						if (!IsLookingRight()) ChangeAnimRight();
					}
					if (position.x == WINDOW_WIDTH - 9) {
						position.x = 1;
						ChangeAnimLeft();
					}
				}
				else {
					position.x += -VELOCIDAD_ENEMIGO;
					if (state == Estado::QUIETO) StartWalkingLeft();
					else {
						if (!IsLookingLeft()) ChangeAnimLeft();
					}
					if (position.x == 1) {
						position.x = WINDOW_WIDTH - 9;
						ChangeAnimRight();
					}
				}
			}
		}
		else {
			switch (atras) {
			case Mirada::LEFT:
				position.x += -VELOCIDAD_ENEMIGO;
				if (state == Estado::QUIETO) StartWalkingLeft();
				else {
					if (!IsLookingLeft()) ChangeAnimLeft();
				}
				break;
			case Mirada::RIGHT:
				position.x += VELOCIDAD_ENEMIGO;
				if (state == Estado::QUIETO) StartWalkingRight();
				else {
					if (!IsLookingRight()) ChangeAnimRight();
				}
				break;
			case Mirada::UP:
				position.y += -VELOCIDAD_ENEMIGO;
				if (state == Estado::QUIETO) StartWalkingUp();
				else {
					if (!IsLookingUp()) ChangeAnimUp();
				}
				break;
			case Mirada::DOWN:
				position.y += VELOCIDAD_ENEMIGO;
				if (state == Estado::QUIETO) StartWalkingDown();
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
		 alternativarapida = { -1, 0 };
		 atras = Mirada::RIGHT;
		 break;
	 case Mirada::RIGHT:
		 alternativarapida = { 1, 0 };
		 atras = Mirada::LEFT;
		 break;
	 case Mirada::UP:
		 alternativarapida = { 0, -1 };
		 atras = Mirada::DOWN;
		 break;
	 case Mirada::DOWN:
		 alternativarapida = { 0, 1 };
		 atras = Mirada::UP;
		 break;
	 }

	 for (int i = 0; i < 4; ++i) {
		 if (i != (int)atras) {
			 targetDir = (Mirada)i;
			 switch (targetDir) {
			 case Mirada::LEFT:
				 position.x += -(VELOCIDAD_ENEMIGO + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallLeft(box)) possible.push_back(targetDir);
				 position.x = prev_x;
				 break;
			 case Mirada::RIGHT:
				 position.x += (VELOCIDAD_ENEMIGO + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallRight(box)) possible.push_back(targetDir);
				 position.x = prev_x;
				 break;
			 case Mirada::UP:
				 position.y += -(VELOCIDAD_ENEMIGO + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallUp(box, Valla)) possible.push_back(targetDir);
				 position.y = prev_y;
				 break;
			 case Mirada::DOWN:
				 position.y += (VELOCIDAD_ENEMIGO + 1);
				 box = GetHitbox();
				 if (!mapa->TestCollisionWallDown(box, Valla)) possible.push_back(targetDir);
				 position.y = prev_y;
				 break;
			 }
		 }
	 }

	 if (possible.size() > 0) {
		 random = GetRandomValue(0, possible.size() - 1);
		 targetDir = possible[random];
	 }
	 else targetDir = atras;
	 
	 switch (targetDir) {
	 case Mirada::LEFT:
		 position.x += -(VELOCIDAD_ENEMIGO + 1);
		 if (state == Estado::QUIETO) StartWalkingLeft();
		 else {
			 if (!IsLookingLeft()) ChangeAnimLeft();
		 }
		 if (position.x == 1) {
			 position.x = WINDOW_WIDTH - 9;
			 ChangeAnimRight();
		 }
		 break;
	 case Mirada::RIGHT:
		 position.x += (VELOCIDAD_ENEMIGO + 1);
		 if (state == Estado::QUIETO) StartWalkingRight();
		 else {
			 if (!IsLookingRight()) ChangeAnimRight();
		 }
		 if (position.x == WINDOW_WIDTH - 9) {
			 position.x = 1;
			 ChangeAnimLeft();
		 }
		 break;
	 case Mirada::UP:
		 position.y += -(VELOCIDAD_ENEMIGO + 1);
		 if (state == Estado::QUIETO) StartWalkingUp();
		 else {
			 if (!IsLookingUp()) ChangeAnimUp();
		 }
		 break;
	 case Mirada::DOWN:
		 position.y += (VELOCIDAD_ENEMIGO + 1);
		 if (state == Estado::QUIETO) StartWalkingDown();
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
		if (position.x == objetivo.x and position.y == objetivo.y-1) Valla = false;
		else if (casa.x == objetivo.x and casa.y == objetivo.y-1) {
			state = Estado::QUIETO;
			pillado = false;
			objetivo = salida_casa;
		}
	}
	else if(state != Estado::OJOS){
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
				if (pacmanPos.x <= (position.x + TILE_SIZE * 4) and pacmanPos.x >= (position.x - TILE_SIZE * 4)) {
					if (pacmanPos.y <= (position.y + TILE_SIZE * 4) and pacmanPos.y >= (position.y - TILE_SIZE * 4)) objetivo = { 0, TILE_SIZE * (LEVEL_HEIGHT - 1) };
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
	Entity::DrawHitbox(position.x, position.y, width, height, col);
}


void ENEMIGO::Release()
{
	if (IsSoundPlaying(retratado)) StopSound(retratado);
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ENEMY);

	render->Release();
}