#include "Log.h"
#include "App.h"
#include "M_Render.h"
#include "M_FileSystem.h"
#include "M_Textures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

M_Textures::M_Textures(bool startEnabled) : Module(startEnabled)
{
	name.assign("textures");
}

// Destructor
M_Textures::~M_Textures()
{}

// Called before render is available
bool M_Textures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool M_Textures::Start()
{
	LOG("start textures");
	bool ret = true;

	archeryTexture = Load("buildings/archery.png");
	stableTexture = Load("buildings/stable.bmp");
	barracksTexture = Load("buildings/barracks.bmp");
	townCenterTexture = Load("buildings/town.png");
	objectTexture = Load("objects/ObjectsSpritesheet.png");
	samuraiTexture = Load("dialogue/ryoma2.png");
	dialogueBoxTexture = Load("dialogue/dialogueBox.png");
	bossLifeBar = Load("textures/BossLifeBar.png");

	return ret;
}

// Called before quitting
bool M_Textures::CleanUp()
{
	LOG("Freeing textures and Image library");
	std::list<SDL_Texture*>::iterator item;

	for(item = textures.begin(); item != textures.end(); item++)
	{
		SDL_DestroyTexture((*item));
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const M_Textures::Load(const char* path)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load_RW(app->fs->Load(path), 1);

	if(surface == nullptr)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));//WITH THIS WE IGNORE THE PINK ON THE SPRITESHEETS
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool M_Textures::UnLoad(SDL_Texture* texture)
{
	std::list<SDL_Texture*>::iterator item;

	for(item = textures.begin(); item != textures.end(); item++)
	{
		if(texture == item._Ptr->_Myval)
		{
			SDL_DestroyTexture(*item);
			textures.erase(item);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const M_Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

	if(texture == nullptr)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.push_back(texture);
	}

	return texture;
}

// Retrieve size of a texture
void M_Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, nullptr, nullptr, (int*) &width, (int*) &height);
}

void M_Textures::DrawDebug()
{
}
