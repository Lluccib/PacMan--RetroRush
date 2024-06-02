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
    AUDIO_INTRO,
    AUDIO_PUNCH1,
    AUDIO_PUNCH2,
    AUDIO_MUERTE,
    AUDIO_SIRENA1,
    AUDIO_SIRENA2,
    AUDIO_SIRENA3,
    AUDIO_SIRENA4,
    AUDIO_SIRENA5,
    AUD_PELLET,
    AUD_FRUIT,
    AUD_EATGHOST,
    AUD_RETREAT,
    AUD_NUMERO
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }

    //Load and unload texture
    AppStatus LoadTexture(Resource id, const std::string& file_path);
    void LoadSounds();

    void ReleaseTexture(Resource id);

    //Get texture by key
    const Texture2D* GetTexture(Resource id) const;
    Sound GetSound(AudioResource id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    //Dictionary to store loaded textures
    std::unordered_map<Resource, Texture2D> textures;
    Sound sounds[(int)AudioResource::AUD_NUMERO];
};