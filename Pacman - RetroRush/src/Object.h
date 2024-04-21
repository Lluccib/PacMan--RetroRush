#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	7
#define OBJECT_FRAME_SIZE		8

#define POINTS_APPLE	10
#define POINTS_CHILI	100
#define POINTS_MONDONGO 1000//NUEVO OBJETO

enum class ObjectType { APPLE, CHILI, MONDONGO};//LE INTRODUZCO EL NUEVO OBJETO

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;

private:
	ObjectType type;
};

