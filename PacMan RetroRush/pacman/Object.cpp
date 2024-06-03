#include "Object.h"
#include "StaticImage.h"

OBJETOS::OBJETOS(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t;

	Rectangle rc;
	const int n = TILE_SIZE;
	const int z = TILE_SIZE + 1;
	switch (type)
	{
	case ObjectType::PILL1: rc = { 13 * z, 2 * z, n, n }; break;
	case ObjectType::PILL2: rc = { 15 * z, 2 * z, n, n }; break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}

OBJETOS::OBJETOS(const Point& p, int fruit) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE * 2, OBJECT_FRAME_SIZE * 2)
{
	Rectangle rc;
	const int n = 16;
	const int c = 8 + 1;
	switch (fruit)
	{
	case 1: 
		rc = { 0, 3 * c, n, n }; 
		type = ObjectType::FRUTAS1;
		break;
	case 2: 
		rc = { n, 3 * c, n, n }; 
		type = ObjectType::FRUTAS2;
		break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc, true);
}
OBJETOS::~OBJETOS()
{
}
void OBJETOS::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(position.x, position.y, width, height, col);
}
int OBJETOS::Sonidos()
{
	if (type == ObjectType::PILL1)		return (int)ObjectType::PILL1;
	else if (type == ObjectType::PILL2)	return (int)ObjectType::PILL2;
	else if (type == ObjectType::FRUTAS1)    return (int)ObjectType::FRUTAS1;
	else if (type == ObjectType::FRUTAS2) return (int)ObjectType::FRUTAS2;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
int OBJETOS::Puntos() const
{
	if (type == ObjectType::PILL1)		return POINTS_PILL1;
	else if (type == ObjectType::PILL2)	return POINTS_PILL2;
	else if (type == ObjectType::FRUTAS1)    return POINTS_FRUTAS1;
	else if (type == ObjectType::FRUTAS2) return POINTS_FRUTAS2;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
