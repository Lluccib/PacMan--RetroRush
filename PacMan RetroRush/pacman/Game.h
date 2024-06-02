#pragma once
#include "Globals.h"
#include "Scene.h"
#include "Intro.h"

enum class GameState { EMPEZAR, MENU_PRINCIPAL, INTRO, JUGANDO, SETTINGS, CREDITS };

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
    AppStatus BeginIntro();
    void FinishPlay();
    void FinishIntro();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    Intro* intro;
    const Texture2D* img_intro;
    const Texture2D* img_menu;
    const Texture2D* img_menu_empty;
    const Texture2D* img_menu_blinky;
    const Texture2D* img_menu_pinky;
    const Texture2D* img_menu_inky;
    const Texture2D* img_menu_clyde;
    const Texture2D* menu1;
    
    #define NUM_IMAGES 10
    float timer = 0.0f;
    float timer2 = 0.0f;
    int timer3 = 0;
    int currentImage = 0;
    const Texture2D* ImagesIntro[NUM_IMAGES];
    int counter = 0;
    float counter2 = 0;

    RenderTexture2D target;
    Rectangle src, dst;
};