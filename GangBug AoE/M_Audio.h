
#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "Module.h"
#include <list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class M_Audio : public Module
{
public:

	M_Audio(bool startEnabled = true);

	// Destructor
	virtual ~M_Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	//Called between scenes
	bool CleanData();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	bool PlayTheme(_Mix_Music* theme);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void DrawDebug()override;

	_Mix_Music*				menuMusic = nullptr;
	_Mix_Music*				firstMission = nullptr;
	_Mix_Music*				secondMission = nullptr;
	_Mix_Music*				thirdMission = nullptr;
	_Mix_Music*				finalMission = nullptr;

private:

	_Mix_Music*				music = nullptr;
	std::list<Mix_Chunk*>	fx;

};

#endif // __j1AUDIO_H__