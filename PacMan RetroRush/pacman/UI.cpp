#include <raylib.h>
#include "UI.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "Sprite.h"

HUD::HUD(const Point& p): Entity(p, (16 + 32), 16, (16 + 32), 16)
{
}

HUD::~HUD() {
	Release();
}

AppStatus HUD::Initialise() 
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ITEMS, "Assets/sprites/Arcade - Pac-Man - object Sprites.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_ITEMS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for items sprite");
		return AppStatus::ERROR;
	}

	float k = 16;
	float n = 16 * 3;

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ELEMENTOSHUD::TOTALICONOS);

	sprite->SetAnimationDelay((int)ELEMENTOSHUD::ICONOVIDAS1, ANIM_DELAY);
	sprite->AddKeyFrame((int)ELEMENTOSHUD::ICONOVIDAS1, { 0, 3 * k, n, k });

	sprite->SetAnimationDelay((int)ELEMENTOSHUD::ICONOVIDAS2, ANIM_DELAY);
	sprite->AddKeyFrame((int)ELEMENTOSHUD::ICONOVIDAS2, { 0, 2 * k, n, k });

	sprite->SetAnimationDelay((int)ELEMENTOSHUD::ICONOVIDAS3, ANIM_DELAY);
	sprite->AddKeyFrame((int)ELEMENTOSHUD::ICONOVIDAS3, { 0, k, n, k });

	sprite->SetAnimationDelay((int)ELEMENTOSHUD::ICONOVIDAS4, ANIM_DELAY);
	sprite->AddKeyFrame((int)ELEMENTOSHUD::ICONOVIDAS4, { k, 3 * k, n, k });

	sprite->SetAnimation((int)ELEMENTOSHUD::ICONOVIDAS3);

	return AppStatus::OK;
}

void HUD::RenderUI(int lives)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	switch (lives) {
	case 1:
		sprite->SetAnimation((int)ELEMENTOSHUD::ICONOVIDAS1);
		break;
	case 2:
		sprite->SetAnimation((int)ELEMENTOSHUD::ICONOVIDAS2);
		break;
	case 3:
		sprite->SetAnimation((int)ELEMENTOSHUD::ICONOVIDAS3);
		break;
	default:
		sprite->SetAnimation((int)ELEMENTOSHUD::ICONOVIDAS4);
	}
}

void HUD::Release() 
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ITEMS);

	render->Release();
}