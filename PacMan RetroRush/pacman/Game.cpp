#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::MENU;
    scene = nullptr;
    intro = nullptr;
    img_menu = nullptr;
    img_menu_empty = nullptr;
    img_menu_blinky = nullptr;
    img_menu_pinky = nullptr;
    img_menu_inky = nullptr;
    img_menu_clyde = nullptr;
    menu1 = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
    if (intro != nullptr)
    {
        intro->Release();
        delete intro;
        intro = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h, w2, h2;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Pac-Man RetroRush");

    int monitor = GetCurrentMonitor();
    w2 = (float)GetMonitorWidth(monitor);
    h2 = (float)GetMonitorHeight(monitor);

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { w2/2 - w/2, h2/2 - h/2, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    SetWindowSize(w2, h2);
    ToggleFullscreen();

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    //cargamos todos los assets para la intro
    if (data.LoadTexture(Resource::IMG_MENU, "Assets/sprites/intro/full.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::IMG_MENU_EMPTY, "Assets/sprites/intro/empty.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_empty = data.GetTexture(Resource::IMG_MENU_EMPTY);

    if (data.LoadTexture(Resource::IMG_MENU_ORANGE, "Assets/sprites/intro/orange.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_clyde = data.GetTexture(Resource::IMG_MENU_ORANGE);

    if (data.LoadTexture(Resource::IMG_MENU_RED, "Assets/sprites/intro/red.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_blinky = data.GetTexture(Resource::IMG_MENU_RED);

    if (data.LoadTexture(Resource::IMG_MENU_PINK, "Assets/sprites/intro/pink.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_pinky = data.GetTexture(Resource::IMG_MENU_PINK);

    if (data.LoadTexture(Resource::IMG_MENU_BLUE, "Assets/sprites/intro/blue.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_inky = data.GetTexture(Resource::IMG_MENU_BLUE);

    
    if (data.LoadTexture(Resource::IMG_MENU1, "Assets/sprites/intro/intro.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    menu1 = data.GetTexture(Resource::IMG_MENU1);


    if (data.LoadTexture(Resource::IMG_INTRO1, "Assets/sprites/preintro/imgintro1.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[0] = data.GetTexture(Resource::IMG_INTRO1);

    if (data.LoadTexture(Resource::IMG_INTRO2, "Assets/sprites/preintro/imgintro2.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[1] = data.GetTexture(Resource::IMG_INTRO2);

    if (data.LoadTexture(Resource::IMG_INTRO3, "Assets/sprites/preintro/imgintro3.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[2] = data.GetTexture(Resource::IMG_INTRO3);

    if (data.LoadTexture(Resource::IMG_INTRO4, "Assets/sprites/preintro/imgintro4.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[3] = data.GetTexture(Resource::IMG_INTRO4);

    if (data.LoadTexture(Resource::IMG_INTRO5, "Assets/sprites/preintro/imgintro5.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[4] = data.GetTexture(Resource::IMG_INTRO5);

    if (data.LoadTexture(Resource::IMG_INTRO6, "Assets/sprites/preintro/imgintro6.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[5] = data.GetTexture(Resource::IMG_INTRO6);

    if (data.LoadTexture(Resource::IMG_INTRO7, "Assets/sprites/preintro/imgintro7.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[6] = data.GetTexture(Resource::IMG_INTRO7);

    if (data.LoadTexture(Resource::IMG_INTRO8, "Assets/sprites/preintro/imgintro8.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[7] = data.GetTexture(Resource::IMG_INTRO8);

    if (data.LoadTexture(Resource::IMG_INTRO9, "Assets/sprites/preintro/imgintro9.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[8] = data.GetTexture(Resource::IMG_INTRO9);

    if (data.LoadTexture(Resource::IMG_INTRO10, "Assets/sprites/preintro/imgintro10.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[9] = data.GetTexture(Resource::IMG_INTRO10);

    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
AppStatus Game::BeginIntro()
{
    intro = new Intro();
    if (intro == nullptr)
    {
        LOG("Failed to allocate memory for intro");
        return AppStatus::ERROR;
    }
    if (intro->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise intro");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
void Game::FinishIntro()
{
    intro->Release();
    delete intro;
    intro = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {

    case GameState::MENU:
     if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
     if(IsKeyPressed(KEY_SPACE))
     {
         if(BeginIntro() != AppStatus::OK) return AppStatus::ERROR;
         state = GameState::MAIN_MENU;
     }
     else {

         counter2 += 1.0f / 60.0f;
     }
     break;

    case GameState::MAIN_MENU:


        if (IsKeyPressed(KEY_ESCAPE))
        {
            FinishIntro();
            counter = 0;
            state = GameState::MENU;
        }
        else if (IsKeyPressed(KEY_SPACE))
        {
            FinishIntro();
            counter = 0;
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
            scene->intro = true;
            state = GameState::PLAYING;
        }
        else {
            if (counter >= 840) {
                intro->Update();
                if (intro->loopCheck) counter = 0;
            }
            counter++;
        }
        break;

    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE) || scene->EndGame)
        {
            FinishPlay();
            if (BeginIntro() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::MAIN_MENU;
        }
        else
        {
            //Game logic
            scene->Update();
        }
        break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);  

    switch (state)
    {
    case GameState::MENU:
        if (counter2 < 3.0f) {
            DrawTexture(*menu1, 0, 0, WHITE);
        }
        else {
            timer2 += 1.0f / 60.0f;
            timer += 1.0f / 60.0f;
            if (timer2 < 1.0f) {
                if (timer >= 0.1f) { // Mostramos cada imagen durante 3 segundos
                    currentImage++;
                    if (currentImage == 2) {
                        currentImage = 0;
                    }
                    timer = 0.0f;
                }
                DrawTexture(*ImagesIntro[currentImage], 0, 0, WHITE);
            }
            if (timer2 >= 1.0f && timer2 < 2.0f) {
                if (timer >= 0.1f) { // Mostramos cada imagen durante 3 segundos
                    currentImage++;
                    if (currentImage == 4) {
                        currentImage = 2;
                    }
                    timer = 0.0f;
                }
                DrawTexture(*ImagesIntro[currentImage], 0, 0, WHITE);
            }
            if (timer2 >= 2.0f && timer2 < 3.0f) {
                if (timer >= 0.1f) { // Mostramos cada imagen durante 3 segundos
                    currentImage++;
                    if (currentImage == 6) {
                        currentImage = 4;
                    }
                    timer = 0.0f;
                }
                DrawTexture(*ImagesIntro[currentImage], 0, 0, WHITE);
            }
            if (timer2 >= 3.0f && timer2 < 4.0f) {
                if (timer >= 0.1f) { // Mostramos cada imagen durante 3 segundos
                    currentImage++;
                    if (currentImage == 8) {
                        currentImage = 6;
                    }
                    timer = 0.0f;
                }
                DrawTexture(*ImagesIntro[currentImage], 0, 0, WHITE);
            }
            if (timer2 >= 4.0f && timer2 < 5.0f) {
                if (timer >= 0.1f) { // Mostramos cada imagen durante 3 segundos
                    currentImage++;
                    if (currentImage == 10) {
                        currentImage = 8;
                    }
                    timer = 0.0f;
                }
                DrawTexture(*ImagesIntro[currentImage], 0, 0, WHITE);
            }

        }

        
        break;

    case GameState::MAIN_MENU:
        if (counter <= 60) {
            DrawTexture(*img_menu_empty, 0, 0, WHITE);
        }
        else if (counter <= 240) {
            DrawTexture(*img_menu_blinky, 0, 0, WHITE);
            if (counter <= 120) DrawRectangle(7 * TILE_SIZE, 7 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 180) DrawRectangle(18 * TILE_SIZE, 7 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else if (counter <= 420) {
            DrawTexture(*img_menu_pinky, 0, 0, WHITE);
            if (counter <= 300) DrawRectangle(7 * TILE_SIZE, 9 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 360) DrawRectangle(18 * TILE_SIZE, 9 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else if (counter <= 600) {
            DrawTexture(*img_menu_inky, 0, 0, WHITE);
            if (counter <= 480) DrawRectangle(7 * TILE_SIZE, 13 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 540) DrawRectangle(18 * TILE_SIZE, 13 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else if (counter <= 780) {
            DrawTexture(*img_menu_clyde, 0, 0, WHITE);
            if (counter <= 660) DrawRectangle(7 * TILE_SIZE, 15 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 720) DrawRectangle(18 * TILE_SIZE, 15 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else {
            DrawTexture(*img_menu, 0, 0, WHITE);
            if (counter == 840) intro->loopCheck = false;
            if (counter >= 840) {
                intro->Render();
            }
        }
        break;

    case GameState::PLAYING:
        scene->Render();
        break;
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_MENU_RED);
    data.ReleaseTexture(Resource::IMG_MENU_PINK);
    data.ReleaseTexture(Resource::IMG_MENU_BLUE);
    data.ReleaseTexture(Resource::IMG_MENU_ORANGE);
    data.ReleaseTexture(Resource::IMG_MENU_EMPTY);
    data.ReleaseTexture(Resource::IMG_INTRO1);
    data.ReleaseTexture(Resource::IMG_INTRO2);
    data.ReleaseTexture(Resource::IMG_INTRO3);
    data.ReleaseTexture(Resource::IMG_INTRO4);
    data.ReleaseTexture(Resource::IMG_INTRO5);
    data.ReleaseTexture(Resource::IMG_INTRO6);
    data.ReleaseTexture(Resource::IMG_INTRO7);
    data.ReleaseTexture(Resource::IMG_INTRO8);
    data.ReleaseTexture(Resource::IMG_INTRO9);
    data.ReleaseTexture(Resource::IMG_INTRO10);

    UnloadRenderTexture(target);
}