#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	7
#define OBJECT_FRAME_SIZE		8

#define POINTS_APPLE	10
#define POINTS_CHILI	20
#define POINTS_MONDONGO 100
#define POINTS_STRAWBERRY	300


enum class ObjectType { APPLE, CHILI, MONDONGO, STRAWBERRY};//LE INTRODUZCO EL NUEVO OBJETO

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	Object(const Point& p, int fruit);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;
	int Sounds();

private:
	ObjectType type;
};

