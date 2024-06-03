#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Jugador::Jugador(const Point& p, State s, Vista view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	mapa = nullptr;
	puntos = 0;
	state = s;
	look = view;
	
}
Jugador::~Jugador()
{
}
AppStatus Jugador::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "Assets/sprites/pacmananimations.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	sonidomuerte = data.GetSound(AudioResource::MUERTE);
	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)AnimacionesJugador::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)AnimacionesJugador::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesJugador::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)AnimacionesJugador::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesJugador::IDLE_LEFT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)AnimacionesJugador::IDLE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesJugador::IDLE_UP, { 0, 2*n, n, n });
	sprite->SetAnimationDelay((int)AnimacionesJugador::IDLE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesJugador::IDLE_DOWN, { 0, 3 * n, n, n });

	sprite->SetAnimationDelay((int)AnimacionesJugador::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)AnimacionesJugador::WALKING_RIGHT, { (float)i * n,  0, n, n });
	};
	sprite->SetAnimationDelay((int)AnimacionesJugador::MURIENDO, ANIM_DELAY);
	for (i = 0; i < 12; ++i){
		sprite->AddKeyFrame((int)AnimacionesJugador::MURIENDO, { (float)i * n, 4*n, n, n });
    };
	sprite->SetAnimationDelay((int)AnimacionesJugador::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)AnimacionesJugador::WALKING_LEFT, { (float)i * n, n, n, n });
	};

	sprite->SetAnimationDelay((int)AnimacionesJugador::WALKING_UP, ANIM_DELAY);
	for (i = 0; i < 3; ++i){
		sprite->AddKeyFrame((int)AnimacionesJugador::WALKING_UP, { (float)i * n, 2*n, n, n });
	};

	sprite->SetAnimationDelay((int)AnimacionesJugador::WALKING_DOWN, ANIM_DELAY);
	for (i = 0; i < 3; ++i){
		sprite->AddKeyFrame((int)AnimacionesJugador::WALKING_DOWN, { (float)i * n, 3 * n, n, n });
    };
	sprite->SetAnimationDelay((int)AnimacionesJugador::CERRADO, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesJugador::CERRADO, { 0, 4 * n, n, n });
	sprite->SetAnimationDelay((int)AnimacionesJugador::ESCONDIDO, ANIM_DELAY);
	sprite->AddKeyFrame((int)AnimacionesJugador::ESCONDIDO, { 4*n, 0, n, n });
	sprite->SetAnimation((int)AnimacionesJugador::ESCONDIDO);
	return AppStatus::OK;
}
bool Jugador::IntroUpdate(bool turn) 
{
	bool end = false;
	if (turn) {
		pos.x += PLAYER_SPEED;
		if (state == State::QUIETO) StartWalkingRight();
		else {
			if (!MirandoDerecha()) ChangeAnimRight();
		}
	}
	else {
		pos.x += -PLAYER_SPEED;
		if (state == State::QUIETO) StartWalkingLeft();
		else {
			if (!MirandoIzquierda()) ChangeAnimLeft();
		}
	}

	if (pos.x == WINDOW_WIDTH) end = true;
	return end;
}
void Jugador::InitScore()
{
	puntos = 0;
}
void Jugador::IncrementarPuntuación(int n)
{
	puntos += n;
}
int Jugador::pillarpuntos()
{
	return puntos;
}
int Jugador::Getvidas() 
{
	return vidas;
}
Point Jugador::GetDirection() 
{
	return dir;
}
Point Jugador::GetPosition() {
	return pos;
}
void Jugador::establecervidas(int l) 
{
	vidas = l;
}
void Jugador::LoseLives() 
{
	--vidas;
}
void Jugador::SetTileMap(TileMap* tilemap)
{
	mapa = tilemap;
}
void Jugador::Ganar() 
{
	SetAnimation((int)AnimacionesJugador::CERRADO);
}
void Jugador::PERDER() {
	perder = true;
	if (contador == 0) {
		PlaySound(sonidomuerte);
		SetAnimation((int)AnimacionesJugador::MURIENDO);
		LoseLives();
	}

	contador++;

	if (contador < 150) {
		if (contador < 48) {
			Sprite* sprite = dynamic_cast<Sprite*>(render);
			sprite->Update();
		} else SetAnimation((int)AnimacionesJugador::ESCONDIDO);
	}
	else {
		perder = false;
		contador = 0;
		SetAnimation((int)AnimacionesJugador::CERRADO);
	}
}
void Jugador::Intro(int count) {
	if(count <= 60) SetAnimation((int)AnimacionesJugador::CERRADO);
	else SetAnimation((int)AnimacionesJugador::ESCONDIDO);
}
void Jugador::ChangeAnimRight()
{
	look = Vista::DERECHA;
	switch (state)
	{
	case State::QUIETO:	 SetAnimation((int)AnimacionesJugador::IDLE_RIGHT);    break;
	case State::ANDANDO: SetAnimation((int)AnimacionesJugador::WALKING_RIGHT); break;
	}
}
void Jugador::ChangeAnimLeft()
{
	look = Vista::IZQUIERDA;
	switch (state)
	{
	case State::QUIETO:	 SetAnimation((int)AnimacionesJugador::IDLE_LEFT);    break;
	case State::ANDANDO: SetAnimation((int)AnimacionesJugador::WALKING_LEFT); break;
	}
}
void Jugador::ChangeAnimUp()
{
	look = Vista::ARRIBA;
	switch (state)
	{
	case State::QUIETO:	 SetAnimation((int)AnimacionesJugador::IDLE_UP);    break;
	case State::ANDANDO: SetAnimation((int)AnimacionesJugador::WALKING_UP); break;
	}
}
void Jugador::ChangeAnimDown()
{
	look = Vista::ABAJO;
	switch (state)
	{
	case State::QUIETO:	 SetAnimation((int)AnimacionesJugador::IDLE_DOWN);    break;
	case State::ANDANDO: SetAnimation((int)AnimacionesJugador::WALKING_DOWN); break;
	}
}
bool Jugador::MirandoDerecha() const
{
	return look == Vista::DERECHA;
}
bool Jugador::MirandoIzquierda() const
{
	return look == Vista::IZQUIERDA;
}
bool Jugador::MirandoArrriba() const
{
	return look == Vista::ARRIBA;
}
bool Jugador::MirandoAbajo() const
{
	return look == Vista::ABAJO;
}
void Jugador::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
AnimacionesJugador Jugador::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (AnimacionesJugador)sprite->GetAnimation();
}
void Jugador::PARAR()
{
	dir = { 0,0 };
	state = State::QUIETO;
	if (MirandoDerecha())	SetAnimation((int)AnimacionesJugador::IDLE_RIGHT);
	else if (MirandoArrriba())  SetAnimation((int)AnimacionesJugador::IDLE_UP);
	else if (MirandoAbajo())  SetAnimation((int)AnimacionesJugador::IDLE_DOWN);
	else					SetAnimation((int)AnimacionesJugador::IDLE_LEFT);
}
void Jugador::StartWalkingLeft()
{
	state = State::ANDANDO;
	look = Vista::IZQUIERDA;
	SetAnimation((int)AnimacionesJugador::WALKING_LEFT);
}
void Jugador::StartWalkingRight()
{
	state = State::ANDANDO;
	look = Vista::DERECHA;
	SetAnimation((int)AnimacionesJugador::WALKING_RIGHT);
}
void Jugador::StartWalkingUp()
{
	state = State::ANDANDO;
	look = Vista::ARRIBA;
	SetAnimation((int)AnimacionesJugador::WALKING_UP);
}
void Jugador::StartWalkingDown()
{
	state = State::ANDANDO;
	look = Vista::ABAJO;
	SetAnimation((int)AnimacionesJugador::WALKING_DOWN);
}
void Jugador::StartDying()
{
	state = State::MURIENDO;
	
	SetAnimation((int)AnimacionesJugador::MURIENDO);
}

void Jugador::Update()
{
	//all movement in move
	Mover();   

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Jugador::Mover()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	//checks which way the player wants to turn next
	if (IsKeyPressed(KEY_UP) or IsKeyDown(KEY_UP))
	{
		turn = Vista::ARRIBA;
		state = State::ANDANDO;
	}
	else if (IsKeyPressed(KEY_DOWN) or IsKeyDown(KEY_DOWN)) {
		turn = Vista::ABAJO;
		state = State::ANDANDO;
	}
	else if (IsKeyPressed(KEY_RIGHT) or IsKeyDown(KEY_RIGHT)) {
		turn = Vista::DERECHA;
		state = State::ANDANDO;
	}
	else if (IsKeyPressed(KEY_LEFT) or IsKeyDown(KEY_LEFT)) {
		turn = Vista::IZQUIERDA;
		state = State::ANDANDO;
	}

	//checks if the turn is possible
	if (turn != look) {
		switch (turn) {
		case Vista::ARRIBA:
			pos.y -= PLAYER_SPEED;
			box = GetHitbox();
			if (!mapa->TestCollisionWallUp(box)) ChangeAnimUp();
			pos.y = prev_y;
			break;
		case Vista::ABAJO:
			pos.y += PLAYER_SPEED;
			box = GetHitbox();
			if (!mapa->TestCollisionWallDown(box)) ChangeAnimDown();
			pos.y = prev_y;
			break;
		case Vista::IZQUIERDA:
			pos.x -= PLAYER_SPEED;
			box = GetHitbox();
			if (!mapa->TestCollisionWallLeft(box)) ChangeAnimLeft();
			pos.x = prev_x;
			break;
		case Vista::DERECHA:
			pos.x += PLAYER_SPEED;
			box = GetHitbox();
			if (!mapa->TestCollisionWallRight(box)) ChangeAnimRight();
			pos.x = prev_x;
			break;
		}
	}

	if (look == Vista::IZQUIERDA)
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::QUIETO)StartWalkingLeft();
		else {
			if (!MirandoIzquierda()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (mapa->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::ANDANDO) PARAR();
		}
		if (pos.x == 0) {
			pos.x = WINDOW_WIDTH - 8;
			ChangeAnimRight();
		}
		
	}
	else if (look == Vista::DERECHA)
	{
		pos.x += PLAYER_SPEED;
		if (state == State::QUIETO) StartWalkingRight();
		else
		{
			if (!MirandoDerecha()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (mapa->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::ANDANDO) PARAR();
		}
		if (pos.x == WINDOW_WIDTH - 8) {
			pos.x = 0;
			ChangeAnimLeft();
		}
		
	}
	else if (look == Vista::ARRIBA) {
		pos.y -= PLAYER_SPEED;
		if (state == State::QUIETO) StartWalkingUp();
		else
		{
			if (!MirandoArrriba()) ChangeAnimUp();
		}

		box = GetHitbox();
		if (mapa->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == State::ANDANDO) PARAR();
		}
	}
	else if (look == Vista::ABAJO) {
		pos.y += PLAYER_SPEED;
		if (state == State::QUIETO) StartWalkingDown();
		else
		{
			if (!MirandoAbajo()) ChangeAnimDown();
		}

		box = GetHitbox();
		if (mapa->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == State::ANDANDO) PARAR();
		}
	}
}
void Jugador::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	DrawPixel(pos.x, pos.y-4, WHITE);
}
void Jugador::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}