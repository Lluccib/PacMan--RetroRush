#pragma once
#include <raylib.h>
#include "Point.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "AABB.h"
class Entity
{
public:
	Entity(const Point& p, int width, int height);
	Entity(const Point& p, int width, int height, int frame_width, int frame_height);
	virtual ~Entity();
	//lógica
	void SetPos(const Point& p);
	void Update();
	AABB GetHitbox() const;
	
	//dibujado
	void DrawPlayer() const;
	void DrawTint(const Color& col) const;
	void Draw() const;
	

	void DrawHitbox(const Color& col) const;
	void DrawHitbox(int x, int y, int w, int h, const Color& col) const;

protected:
	Point position, direction;
	int width, height;
	Point GetRenderingPosition() const;
	Point GetRenderingPositionPlayer() const;

	
	
	int frame_width, frame_height;

	RenderComponent* render;
};