
#include "Entity.h"
#include <cmath>
#include "Globals.h"

Entity::Entity(const Point& p, int w, int h) : pos(p), dir({ 0,0 }), width(w), height(h), frame_width(w), frame_height(h), render(nullptr)
{
}
Entity::Entity(const Point& p, int w, int h, int frame_w, int frame_h) : pos(p), dir({ 0,0 }), width(w), height(h), frame_width(frame_w), frame_height(frame_h), render(nullptr)
{
}
Entity::~Entity()
{
	if (render != nullptr)
	{
		delete render;
		render = nullptr;
	}
}
void Entity::SetPos(const Point& p)
{
	pos = p;
}
const Point& Entity::GetPos() const
{
	return pos;
}
void Entity::SetDirection(const Point& d)
{
	dir = d;
}
void Entity::Update()
{
	printf("%d %d\n", prev_x, prev_y);
	prev_x = pos.x;
	prev_y = pos.y;
	pos += dir;
}
AABB Entity::GetHitbox() const
{
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
Point Entity::GetRenderingPosition() const
{
	Point p;
	p.x = pos.x + width / 2 - frame_width / 2;
	p.y = pos.y - (frame_height-1);
	return p;
}
void Entity::Draw() const
{
	Point p = GetRenderingPosition();
	render->Draw(p.x, p.y);
}
void Entity::DrawTint(const Color& col) const
{
	Point p = GetRenderingPosition();
	render->DrawTint(p.x, p.y, col);
}
void Entity::DrawHitbox(const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(pos.x, pos.y-(height-1), width, height, c);
	render->DrawCorners(pos.x, pos.y-(height-1), width, height);
}
void Entity::DrawHitbox(int x, int y, int w, int h, const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(x, y-(h-1), w, h, c);
	render->DrawCorners(x, y-(h-1), w, h);
}