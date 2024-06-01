#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

#define LEVELS 2

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    DebugMode debug;

    bool EndGame = false;
    bool intro = false;
    bool levelintro = 0;

private:
    AppStatus LoadLevel(int stage);

    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;
    
    Player *player;
    TileMap *level;
    std::vector<Object*> objects;

    Camera2D camera;
};

