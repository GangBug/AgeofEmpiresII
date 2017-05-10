#ifndef __PARTICLE_ANIMATION_H__
#define __PARTICLE_ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL.h"
#include "Defs.h"
#include <vector>
using namespace std;

class ParticleAnimation
{
public:
	bool loop = true;
	float speed = 1.0f;
	vector<SDL_Rect> frames;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

private:
	float current_frame;
	int last_frame = 0;
	int loops = 0;
	int last_loop = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames.push_back(rect);
	}

	// return current_frame+speed frame Rect
	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= frames.size())
		{
			current_frame = (loop) ? 0.0f : frames.size() - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	// return current_current frame Rect
	SDL_Rect& GetActualFrame() {

		return frames[(int)current_frame];
	}

	float GetFrameIndex()const {
		return current_frame;
	}

	void SetInitialFrame(uint frame) {
		current_frame = (float)frame;
	}

	bool IsFrame(uint frame_index) {
		return (current_frame == frame_index);
	}

	void AnimForward() {
		current_frame += speed;
		if (current_frame >= last_frame) current_frame = 0;
	}

	void AnimBack() {
		current_frame -= speed;
		if (current_frame < 0)current_frame = (float)last_frame - 1;
	}

	bool Finished()
	{
		if (last_loop != loops)
		{
			last_loop++;
			return true;
		}
		return false;
	}

	void Reset()
	{
		current_frame = 0;
		loops = 0;
	}

};
#endif
