#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"

#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"



void AudioFX::Play(int loops/*==0*/)
{//loop -1 == loop 4ever, 0== play one time, 1== play two times...

	if (Mix_PlayChannel(-1, mChunk, loops) == -1) {
		LOG("Mix_PlayChanel! SDL_Error: %s\n", SDL_GetError());
	}
}
bool AudioMusic::Play(int loops/*==-1*/, float fade_time)
{//loop -1 == loop 4ever, 0== play 0 times, 1== play 1 times, 2== play 2 times...
	Mix_VolumeMusic(50);
	Mix_FadeInMusic(mMusic, -1, (fade_time * 1000.0f));

	LOG("Successfully playing");
	return true;
}

void AudioMusic::Pause()
{
	Mix_PauseMusic();
}

void AudioMusic::Stop()
{
	Mix_HaltMusic();
}

void AudioMusic::Resume()
{
	Mix_ResumeMusic();
}

//------------------------------------------ Audio engine
M_Audio::M_Audio()
{
}

M_Audio::~M_Audio()
{
}
//initialice audio
bool M_Audio::Init()
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;	// can support MIX_INIT_FAC, MIX_INIT_MP3, MIX_INIT_OGG, MIX_INIT_MOD,
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = true;
		mIsInitialized = true;
	}



	return ret;

}

//destroy all the elements
bool M_Audio::CleanUp()
{


	if (mIsInitialized) {
		mIsInitialized = false;
		Mix_CloseAudio();
		mMusicMap.clear();
		Mix_Quit();
	
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}

	return true;

}


void M_Audio::setMusicVolume(int vol)
{
	Mix_VolumeMusic(vol);
}

AudioFX M_Audio::LoadAudioFX(const std::string & filePath)
{
	//look to the cachhe to found the audio
	auto it = mEffectMap.find(filePath);

	AudioFX ret;

	if (it == mEffectMap.end()) {
		//failed to find -> load
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

		if (chunk == nullptr) {
			LOG("Mix_LoadWAV! SDL_Error: %s\n", SDL_GetError());
		}

		ret.mChunk = chunk;
		mEffectMap[filePath] == chunk;

	}
	else {
		//is on cache
		ret.mChunk = it->second;

	}


	return ret;
}

void M_Audio::FadeMusic(int ms)
{
	/*if (mMusicMap.begin != nullptr)
	{
		Mix_FadeOutMusic((int)(ms * 1000.0f));
	}*/
}




AudioMusic M_Audio::LoadAudioMusic(const std::string & filePath)
{

	//look to the cachhe to found the audio
	auto it = mMusicMap.find(filePath);

	AudioMusic ret;

	if (it == mMusicMap.end()) {
		//failed to find -> load
		Mix_Music* MixMusic = Mix_LoadMUS(filePath.c_str());

		if (MixMusic == nullptr) {
			LOG("Mix_LoadWAV! SDL_Error: %s\n", SDL_GetError());
		}

		ret.mMusic = MixMusic;
		mMusicMap[filePath] == MixMusic;

	}
	else {
		//is on cache
		ret.mMusic = it->second;

	}

	return ret;
}
//------------------------------------------

