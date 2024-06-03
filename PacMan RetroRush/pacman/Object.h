#pragma once
#include "Entity.h"
#define OBJECT_PHYSICAL_SIZE	8
#define OBJECT_FRAME_SIZE		8
#define POINTS_PILL1	10
#define POINTS_PILL2	20
#define POINTS_FRUTAS1	100
#define POINTS_FRUTAS2	300

enum class ObjectType { PILL1, PILL2, FRUTAS1, FRUTAS2 };

class OBJETOS : public Entity
{
public:
	OBJETOS(const Point& p, ObjectType t);
	OBJETOS(const Point& p, int fruit);
	~OBJETOS();

	void DrawDebug(const Color& col) const;
	int Puntos() const;
	int Sonidos();

private:
	ObjectType type;
};