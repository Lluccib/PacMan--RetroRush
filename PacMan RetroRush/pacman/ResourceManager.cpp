#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
    InitAudioDevice();
    LoadSounds();
}
ResourceManager::~ResourceManager()
{
    Release();
}

//Load a texture from a file and associate it with the key id
AppStatus ResourceManager::LoadTexture(Resource id, const std::string& file_path)
{
    //Load the texture
    Texture2D texture = ::LoadTexture(file_path.c_str());
    if (texture.id == 0)
    {
        //Error loading texture
        LOG("Failed to load texture ", file_path);
        return AppStatus::ERROR;
    }

    //Insert the loaded texture into the map with the specified key
    textures[id] = texture;
    return AppStatus::OK;
}

void ResourceManager::LoadSounds()
{
    sounds[(int)AudioResource::AUD_INTRO] = LoadSound("Resources/game audio/game_start.wav");

    sounds[(int)AudioResource::AUD_MUNCH1] = LoadSound("Resources/game audio/munch_1.wav");
    sounds[(int)AudioResource::AUD_MUNCH2] = LoadSound("Resources/game audio/munch_2.wav");

    sounds[(int)AudioResource::AUD_DEATH] = LoadSound("Resources/game audio/pacman_death.wav");

    sounds[(int)AudioResource::AUD_SIREN1] = LoadSound("Resources/game audio/siren_1.wav");
    sounds[(int)AudioResource::AUD_SIREN2] = LoadSound("Resources/game audio/siren_2.wav");
    sounds[(int)AudioResource::AUD_SIREN3] = LoadSound("Resources/game audio/siren_3.wav");
    sounds[(int)AudioResource::AUD_SIREN4] = LoadSound("Resources/game audio/siren_4.wav");
    sounds[(int)AudioResource::AUD_SIREN5] = LoadSound("Resources/game audio/siren_5.wav");

    sounds[(int)AudioResource::AUD_PELLET] = LoadSound("Resources/game audio/power_pellet.wav");
    sounds[(int)AudioResource::AUD_FRUIT] = LoadSound("Resources/game audio/eat_fruit.wav");
    sounds[(int)AudioResource::AUD_EATGHOST] = LoadSound("Resources/game audio/eat_ghost.wav");

    sounds[(int)AudioResource::AUD_RETREAT] = LoadSound("Resources/game audio/retreating.wav");
}

//Release the texture associated with the key id
void ResourceManager::ReleaseTexture(Resource id)
{
    //Find the texture associated with the key
    auto it = textures.find(id);

    //If found, unload the texture and remove it from the map
    if (it != textures.end())
    {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

//Get a texture by key
const Texture2D* ResourceManager::GetTexture(Resource id) const
{
    //Find the texture associated with the key and return it
    auto it = textures.find(id);
    if (it != textures.end())   return &(it->second);

    //Return nullptr if key is not found
    return nullptr;
}

Sound ResourceManager::GetSound(AudioResource id) const
{
    return (sounds[(int)id]);
}

void ResourceManager::Release()
{
    for (auto& pair : textures)
    {
        UnloadTexture(pair.second);
    }
    textures.clear();
    for (int i = 0; i < (int)AudioResource::AUD_NUM; ++i) {
        UnloadSound(sounds[i]);
    }
    CloseAudioDevice();
}