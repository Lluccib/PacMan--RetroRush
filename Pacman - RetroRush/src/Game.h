#pragma once
#include "Globals.h"
#include "Scene.h"


enum class GameState { INTRO, MAIN_MENU, PLAYING, SETTINGS, CREDITS, WIN, LOSE };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D *img_menu;
    const Texture2D* img_intro;
    const Texture2D* img_win;
    const Texture2D* img_lose;

    Sound fxmenu[8];
    Music music[8];

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};