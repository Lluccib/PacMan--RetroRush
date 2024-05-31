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

    const Texture2D* img_red;
    const Texture2D* img_pink;
    const Texture2D* img_blue;
    const Texture2D* img_orange;
    const Texture2D* img_empty;
    const Texture2D* img_full;

    #define NUM_IMAGES 10
    float timer = 0.0f;
    float timer2 = 0.0f;
    int currentImage = 0;
    const Texture2D* ImagesIntro[NUM_IMAGES];

    Sound fxmenu[9];
    Music music[9];

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};