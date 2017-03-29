#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include <map> 

#define DEFAULT_MUSIC_FADE_TIME 2.0f

//------------------------
class AudioFX {
	friend class M_Audio;
public:
	void Play(int nLoops = 0);//default will play once

private:
	Mix_Chunk* mChunk = nullptr;
	const char* path;
};
//------------------------

//------------------------
class AudioMusic {
	friend class M_Audio;
public:
	bool Play(int nLoops = -1, float fade_time = DEFAULT_MUSIC_FADE_TIME);//default will play once
	static void Pause();
	static void Stop();
	static void Resume();


private:
	Mix_Music* mMusic = nullptr;
	const char* path;
};
//------------------------

//------------------------
class M_Audio : public j1Module {
public:

	M_Audio();

	// Destructor
	virtual ~M_Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&)override;

	// Called before quitting
	bool CleanUp()override;

public:
	AudioFX* LoadAudioFX(const std::string&filePath);
	AudioMusic* LoadAudioMusic(const std::string&filePath);

private:

	std::map<std::string, Mix_Chunk*> mEffectMap;
	std::map<std::string, Mix_Music*> mMusicMap;

	bool mIsInitialized = false;
};
//------------------------

//how to implemement:

/*

///in Application.h

#include "ModuleAudioEngine.h"
ModuleAudioEngine* audio;

///in Application.cpp

audio = new ModuleAudioEngine(this, true);
AddModule(audio); <- Add before Scene module;


///in module scene.h   !!!!!!!!!!!!!!!!!!!!!!!!!!!!! well in this case no, i will work with it !!!!!!!!!!!!!!!!!!!!!!!!!!!!

AudioMusic* bso_scene;

///in module scene.cpp

(in the start();)
App->audio->Init();
bso_scene = App->audio->LoadAudioMusic("Sound/BSO/BSO_SCENE1.ogg");

(then to the efects u can implement it on the monster class or in sscene too)

///.h
AudioFX grr_fx;

///.cpp

(constructor (or start in module scene))
grr_fx = App->audio->LoadAudioFX("Sound/FX/grr_fx.wav");



///// To use:
grr_fx.play(); default 0
AudioFX.play(); default -1


//dont forget to Clean it!
*/

#endif // __M_AUDIO_H__