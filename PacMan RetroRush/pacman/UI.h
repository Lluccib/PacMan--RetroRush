#pragma once
#include <raylib.h>
#include "Globals.h"
#include "Entity.h"

enum class ELEMENTOSHUD {
	ICONOVIDAS1, ICONOVIDAS2, ICONOVIDAS3, ICONOVIDAS4, TOTALICONOS
};

class HUD : public Entity {
public:
	HUD(const Point &p);
	~HUD();

	AppStatus Initialise();
	void RenderUI(int lives);
	void Release();

private:
	int type;
};