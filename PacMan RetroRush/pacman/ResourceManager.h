#pragma once

#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class Resource {
    IMG_MENU1,
    IMG_MENU,
    IMG_MENU_RED,
    IMG_MENU_BLUE,
    IMG_MENU_PINK,
    IMG_MENU_ORANGE,
    IMG_MENU_EMPTY,
    IMG_PLAYER,
    IMG_TILES,
    IMG_TILES_WHITE,
    IMG_ENEMY,
    IMG_ITEMS,
    IMG_FONT,
    IMG_INTRO,
    IMG_INTRO1,
    IMG_INTRO2,
    IMG_INTRO3,
    IMG_INTRO4,
    IMG_INTRO5,
    IMG_INTRO6,
    IMG_INTRO7,
    IMG_INTRO8,
    IMG_INTRO9,
    IMG_INTRO10,
    IMG_INTRO11,
};

enum class AudioResource {
    INTRO,
    PUNCH1,
    PUNCH2,
    SIRENA1,
    SIRENA2,
    SIRENA3,
    SIRENA4,
    SIRENA5,
    MUERTE,
    AUD_PELLET,
    FRUITAUD,
    EATGHOST,
    RETRATADO,
    AUD_NUMERO
};

class ResourceManager {
public:
    static ResourceManager& Instance()
    {
        static ResourceManager instance;
        return instance;
    }
    AppStatus LoadTexture(Resource id, const std::string& file_path);
    void LoadSounds();
    void Release();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    void ReleaseTexture(Resource id);
    const Texture2D* GetTexture(Resource id) const;
    Sound GetSound(AudioResource id) const;

private:
   
    std::unordered_map<Resource, Texture2D> textures;
    Sound sounds[(int)AudioResource::AUD_NUMERO]; 
    ResourceManager();
    ~ResourceManager();
};