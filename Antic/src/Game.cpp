#include "Game.h" 
#include "Globals.h"
#include "ResourceManager.h"

#include <stdio.h>


Game::Game()
{
    state = GameState::INTRO;
    scene = nullptr;
    img_menu = nullptr;
    img_empty = nullptr;
    img_full = nullptr;
    img_red = nullptr;
    img_pink = nullptr;
    img_blue = nullptr;
    img_orange = nullptr;
    img_intro = nullptr;

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
}
AppStatus Game::Initialise(float scale)
{
    float w, h, w2, h2;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Pacman - RetroRush");

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
    dst = { w2 / 2 - w / 2, h2 / 2 - h / 2, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    InitAudioDevice();

    fxmenu[0] = LoadSound("Wav/pacmaneatingcherry.wav");
    fxmenu[1] = LoadSound("Wav/pacmaneatingghost.wav");
    fxmenu[2] = LoadSound("Wav/pacmanextralive.wav");
    fxmenu[3] = LoadSound("Wav/pacmangameover.wav");
    fxmenu[4] = LoadSound("Wav/pacmanmovement.wav");
    fxmenu[5] = LoadSound("Wav/pacmansiren.wav");
    fxmenu[6] = LoadSound("Wav/pacmansong.wav");
    fxmenu[7] = LoadSound("Wav/pacmanwakawaka.wav");

    music[0] = LoadMusicStream("Fx/pacmaneatingcherry.ogg");
    music[1] = LoadMusicStream("Fx/pacmaneatingghost.ogg");
    music[2] = LoadMusicStream("Fx/pacmanextralive.ogg");
    music[3] = LoadMusicStream("Fx/pacmangameover.ogg");
    music[4] = LoadMusicStream("Fx/pacmanmovement.ogg");
    music[5] = LoadMusicStream("Fx/pacmansiren.ogg");
    music[6] = LoadMusicStream("Fx/pacmansong.ogg");
    music[7] = LoadMusicStream("Fx/pacmanwakawaka.ogg");

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
    
    if (data.LoadTexture(Resource::IMG_MENU, "images/GameStart.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::IMG_INTRO, "images/intro.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_intro = data.GetTexture(Resource::IMG_INTRO);

    if (data.LoadTexture(Resource::IMG_WIN, "images/win.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_win = data.GetTexture(Resource::IMG_WIN);

    if (data.LoadTexture(Resource::IMG_LOSE, "images/lose.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_lose = data.GetTexture(Resource::IMG_LOSE);
    
    if (data.LoadTexture(Resource::IMG_INTRO1, "images/preintro/imgintro1.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[0] = data.GetTexture(Resource::IMG_INTRO1);

    if (data.LoadTexture(Resource::IMG_INTRO2, "images/preintro/imgintro2.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[1] = data.GetTexture(Resource::IMG_INTRO2);

    if (data.LoadTexture(Resource::IMG_INTRO3, "images/preintro/imgintro3.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[2] = data.GetTexture(Resource::IMG_INTRO3);

    if (data.LoadTexture(Resource::IMG_INTRO4, "images/preintro/imgintro4.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[3] = data.GetTexture(Resource::IMG_INTRO4);

    if (data.LoadTexture(Resource::IMG_INTRO5, "images/preintro/imgintro5.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[4] = data.GetTexture(Resource::IMG_INTRO5);

    if (data.LoadTexture(Resource::IMG_INTRO6, "images/preintro/imgintro6.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[5] = data.GetTexture(Resource::IMG_INTRO6);

    if (data.LoadTexture(Resource::IMG_INTRO7, "images/preintro/imgintro7.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[6] = data.GetTexture(Resource::IMG_INTRO7);

    if (data.LoadTexture(Resource::IMG_INTRO8, "images/preintro/imgintro8.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[7] = data.GetTexture(Resource::IMG_INTRO8);

    if (data.LoadTexture(Resource::IMG_INTRO9, "images/preintro/imgintro9.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[8] = data.GetTexture(Resource::IMG_INTRO9);

    if (data.LoadTexture(Resource::IMG_INTRO10, "images/preintro/imgintro10.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    ImagesIntro[9] = data.GetTexture(Resource::IMG_INTRO10);

    if (data.LoadTexture(Resource::IMG_EMPTY, "images/intro/empty.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_empty = data.GetTexture(Resource::IMG_EMPTY);

    if (data.LoadTexture(Resource::IMG_RED, "images/intro/red.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_red = data.GetTexture(Resource::IMG_RED);

    if (data.LoadTexture(Resource::IMG_PINK, "images/intro/pink.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_pink = data.GetTexture(Resource::IMG_PINK);

    if (data.LoadTexture(Resource::IMG_BLUE, "images/intro/blue.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_blue = data.GetTexture(Resource::IMG_BLUE);

    if (data.LoadTexture(Resource::IMG_ORANGE, "images/intro/orange.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_orange = data.GetTexture(Resource::IMG_ORANGE);

    if (data.LoadTexture(Resource::IMG_FULL, "images/intro/full.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_full = data.GetTexture(Resource::IMG_FULL);

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
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;
    switch (state)
    {
        case GameState::INTRO:
             if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
             if (IsKeyPressed(KEY_SPACE))
             {

                state = GameState::MAIN_MENU;
             }
             break;
       
        case GameState::MAIN_MENU: 
            /*PlayMusicStream(music[6]);
            UpdateMusicStream(music[6]);*/
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if(BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::PLAYING;
            }
            else {
                timer3++;
            }
            break;

        case GameState::PLAYING:
            /*PlaySound(fxmenu[6]);*/
            PlayMusicStream(music[5]);
            UpdateMusicStream(music[5]);

            if (IsKeyPressed(KEY_ESCAPE))
            {
                FinishPlay();
                state = GameState::MAIN_MENU;

            }
            else
            {
                scene->HandleInputPlayer();
                //Game logic
                scene->Update();
            }
            if (IsKeyPressed(KEY_F3)) {
                FinishPlay();
                state = GameState::WIN;
            }
            if (IsKeyPressed(KEY_F4)) {
                FinishPlay();
                state = GameState::LOSE;
            }
            break;
        case GameState::WIN:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                state = GameState::MAIN_MENU;
              
            }
            break;

        case GameState::LOSE:
            PlayMusicStream(music[3]);
            UpdateMusicStream(music[3]);
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                state = GameState::MAIN_MENU;
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
        case GameState::INTRO:
            /*DrawTexture(*img_intro, 0, 0, WHITE);*/
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
            break;

        case GameState::MAIN_MENU:
            printf("%d\n", timer3);
            if (timer3 <= 60) {
                DrawTexture(*img_empty, 0, 0, WHITE);
            }
            else if (timer3 <= 240) {
                DrawTexture(*img_red, 0, 0, WHITE);
                if (timer3 <= 120) DrawRectangle(7 * TILE_SIZE, 7 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
                else if (timer3 <= 180) DrawRectangle(18 * TILE_SIZE, 7 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            }
            else if (timer3 <= 420) {
                DrawTexture(*img_pink, 0, 0, WHITE);
                if (timer3 <= 300) DrawRectangle(7 * TILE_SIZE, 9 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
                else if (timer3 <= 360) DrawRectangle(18 * TILE_SIZE, 9 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            }
            else if (timer3 <= 600) {
                DrawTexture(*img_blue, 0, 0, WHITE);
                if (timer3 <= 480) DrawRectangle(7 * TILE_SIZE, 13 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
                else if (timer3 <= 540) DrawRectangle(18 * TILE_SIZE, 13 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            }
            else if (timer3 <= 780) {
                DrawTexture(*img_orange, 0, 0, WHITE);
                if (timer3 <= 660) DrawRectangle(7 * TILE_SIZE, 15 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
                else if (timer3 <= 720) DrawRectangle(18 * TILE_SIZE, 15 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            }
            else {
                DrawTexture(*img_full, 0, 0, WHITE);
                if (timer3 == 840) timer3 = 0;
                /*if (timer3 >= 840) {
                    intro->Render();
                }*/
            }
            break;

        case GameState::PLAYING:
            scene->Render();
            break;

        case GameState::WIN:
            DrawTexture(*img_win, 0, 0, WHITE);
            break;

        case GameState::LOSE:
            DrawTexture(*img_lose, 0, 0, WHITE);
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
    data.ReleaseTexture(Resource::IMG_INTRO);
    data.ReleaseTexture(Resource::IMG_WIN);
    data.ReleaseTexture(Resource::IMG_LOSE);
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