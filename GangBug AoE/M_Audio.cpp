#include "Log.h"
#include "App.h"
#include "M_FileSystem.h"
#include "M_Audio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

M_Audio::M_Audio(bool startEnabled) : Module(startEnabled)
{
	music = NULL;
	name.assign("audio");
}

// Destructor
M_Audio::~M_Audio()
{}

// Called before render is available
bool M_Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	if (firstMission == nullptr)
	{
		menuMusic = Mix_LoadMUS_RW(app->fs->Load("audio/BSO/BSO_Menu.ogg"), 1);
		firstMission = Mix_LoadMUS_RW(app->fs->Load("audio/BSO/BSO_First.ogg"), 1);
		secondMission = Mix_LoadMUS_RW(app->fs->Load("audio/BSO/BSO_Second.ogg"), 1);
		thirdMission = Mix_LoadMUS_RW(app->fs->Load("audio/BSO/BSO_Third.ogg"), 1);
		finalMission = Mix_LoadMUS_RW(app->fs->Load("audio/BSO/BSO_Final.ogg"), 1);
	}

	return ret;
}

// Called before quitting
bool M_Audio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		//Mix_FreeMusic(music); //TODO
	}

	if (thirdMission != NULL)
	{
		//Mix_FreeMusic(thirdMission); //TODO
	}

	if (menuMusic != NULL)
	{
		//Mix_FreeMusic(menuMusic); //TODO
	}

	std::list<Mix_Chunk*>::iterator item;
	for (item = fx.begin(); item != fx.end(); ++item)
		Mix_FreeChunk(*item);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool M_Audio::CleanData()
{
	if (!active)
		return true;

	LOG("Freeing sound FX and music");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	std::list<Mix_Chunk*>::iterator item;
	for (item = fx.begin(); item != fx.end(); ++item)
	{
		Mix_FreeChunk(*item);
	}


	fx.clear();
	Mix_HaltMusic();

	return true;
}

// Play a music file
bool M_Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != nullptr)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS_RW(app->fs->Load(path), 1);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

bool M_Audio::PlayTheme(_Mix_Music* theme)
{
	bool ret = true;
	if (Mix_PlayMusic(theme, -1) < 0)
	{
		LOG("Cannot play music Mix_GetError(): %s", Mix_GetError());
		ret = false;
	}

	return ret;
}

// Load WAV
unsigned int M_Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(app->fs->Load(path), 1);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool M_Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if (!active)
		return false;

	if (id > 0 && id <= fx.size())
	{
		std::list<Mix_Chunk*>::iterator item = fx.begin();
		advance(item, id - 1);
		Mix_PlayChannel(-1, (*item), repeat);
	}

	return ret;
}

void M_Audio::DrawDebug()
{
}
