#ifndef __M_AUDIO_H__
#define __M_AUDIO_H__

#include "Module.h"
#include <vector>

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
	bool Awake(pugi::xml_node&)override;

	// Called before quitting
	bool CleanUp()override;

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void DrawDebug()override;

private:

	_Mix_Music*			music = nullptr;
	std::vector<Mix_Chunk*>	fx;
};

#endif // __M_AUDIO_H__