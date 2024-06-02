#pragma once

#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class Resource {
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
    IMG_EMPTY,
    IMG_FULL,
    IMG_RED,
    IMG_PINK,
    IMG_BLUE,
    IMG_ORANGE,
    IMG_MENU,
    IMG_PLAYER,
    IMG_TILES,
    IMG_ITEMS,
    IMG_WIN,
    IMG_LOSE,
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
    void ReleaseTexture(Resource id);

    //Get texture by key
    const Texture2D* GetTexture(Resource id) const;

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
};