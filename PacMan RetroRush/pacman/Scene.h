#pragma once
#include <raylib.h>
#include "Player.h"
#include "Enemys.h"
#include "TileMap.h"
#include "Object.h"
#include "UI.h"
#include "Text.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

#define LEVELS 2
//temmporizadores en (ms)
#define PELLETTIME 480;
#define FRUITTIME 900;

#define CYANBLUE CLITERAL(Color){0, 255, 255, 255}//fuente color pacman 1980
#define PARTICIONES 48//las particiones de los items
#define SIREN_LENGTH 48//toda la alargada de la sirena (sonido)
class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    
    bool intro = false;
    bool levelintro = 0;
    bool fin = false;
private:
    AppStatus LoadLevel(int stage);
    //renderizado
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;
    void RenderGUI() const;

    void CheckCollisions();
    void ClearLevel();

    //jugador
    Jugador* player;

    //enemigos
    ENEMIGO* blinky;
    ENEMIGO* inky;
    ENEMIGO* clyde;
    ENEMIGO* pinky;

    //interfaz 
    HUD* vidasHUD;

    TileMap* nivel;
    std::vector<OBJETOS*> objects;
    
    //sirena de fondo
    Sound sirenas[5];
    //efectos
    Sound pill_sonido, intro_sonido, fruta_sonido, comerenemigo_sonido, punch1_sonido, punch2_sonido;   
    //cambio de sirena
    int sirena = 0;
    //largo de sirena
    int largoSirena = SIREN_LENGTH;
   
    //activadores de boost
    bool pillarcereza = false;
    int cerezaTimepo = 480;

    //estados de la partida (ganas, pierdes, se acaba nivel, nivel actual)
    bool EndLevel = false;
    bool ganar = false;
    bool perder = false;
    int level_count = 1;
  
    bool punch1 = true;

    //godmode
    bool god_mode = false;

    int intro_count = 240;
    int ghost_points = 200;

    int cuentafrutas = FRUITTIME;
    //Estado de pillado
    bool blinkypillado = false;
    bool inkypillado = false;
    bool clycepillado = false;
    bool pinkypillado = false;
    //posiciones 
    int clydeX, clydeY;
    int blinkyX, blinkyY;
    int pinkyX, pinkyY;
    int inkyX, inkyY;
    int playerX, playerY;
    int fruitX, fruitY;
    
    Camera2D camera;
    DebugMode debug;

    Text* fuente;

   
};