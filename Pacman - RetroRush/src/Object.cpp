#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
		case ObjectType::APPLE: rc = {0, 0, n, n}; break;
		case ObjectType::CHILI: rc = {2 * n, 0, n, n}; break;

		default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_ITEMS), rc);
}
Object::Object(const Point& p, int fruit) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE * 2, OBJECT_FRAME_SIZE * 2)
{

	Rectangle rc;
	const int n = 16;
	const int c = 8 + 1;
	switch (fruit)
	{
	case 1:
		rc = { 0, 3 * c, n, n };
		type = ObjectType::MONDONGO;
		break;
	case 2:
		rc = { n, 3 * c, n, n };
		type = ObjectType::STRAWBERRY;
		break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_ITEMS), rc, true);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::APPLE)		return POINTS_APPLE;
	else if (type == ObjectType::CHILI)	return POINTS_CHILI;
	else if (type == ObjectType::MONDONGO) return POINTS_MONDONGO;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
int Object::Sounds()
{
	if (type == ObjectType::APPLE)		return (int)ObjectType::APPLE;
	else if (type == ObjectType::CHILI)	return (int)ObjectType::CHILI;
	else if (type == ObjectType::MONDONGO)    return (int)ObjectType::MONDONGO;
	else if (type == ObjectType::STRAWBERRY) return (int)ObjectType::STRAWBERRY;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}