#ifndef __M_VIDEO_H__
#define __M_VIDEO_H__

#include "Module.h"
#include "j1PerfTimer.h"

enum THEORAPLAY_VideoFormat;
struct THEORAPLAY_AudioPacket;
struct THEORAPLAY_Decoder;
struct THEORAPLAY_VideoFrame;

struct AudioQueue
{
	const THEORAPLAY_AudioPacket *audio;
	int offset;
	struct AudioQueue *next;
};

class M_Video : public Module
{
public:
	M_Video(bool startEnabled = true);
	// Destructor
	virtual ~M_Video();
	// Called before render is available
	bool Awake(pugi::xml_node&);
	// Update videoframes and audiopackets
	update_status PostUpdate(float dt);
	// Called before quitting
	bool CleanUp();

	// Play a video file
	void PlayVideo(const char *fname, SDL_Rect r);

	void DrawDebug()override;

	// Audio methods
	static void SDLCALL audio_callback(void *userdata, Uint8 *stream, int len);
	static void queue_audio(const THEORAPLAY_AudioPacket *audio);

private:
	void ResetValues();
	// Load video file
	void LoadVideo(const char *fname);

private:
	THEORAPLAY_Decoder* decoder;
	const THEORAPLAY_VideoFrame* video;
	const THEORAPLAY_AudioPacket* audio;
	SDL_Window* screen;
	SDL_Texture* texture;
	SDL_AudioSpec spec;
	SDL_Event event;

	Uint32 baseticks;
	Uint32 framems;

	int init_failed;
	int quit;

	void* pixels;
	int pitch;
	bool want_to_play;

	static AudioQueue* audio_queue;
	static AudioQueue* audio_queue_tail;

	SDL_Rect rendering_rect;

public:
	bool video_finished = false;
};

#endif // __M_VIDEO_H__