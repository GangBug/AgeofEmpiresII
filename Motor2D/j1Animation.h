#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "j1Module.h"
#include <vector>
#include <list>
#include "Entity.h"
#include "Units.h"
#include "Boss.h"
#include "j1Timer.h"
#include "Units.h"

struct SDL_Texture;

struct Textures
{
public:
	Textures(SDL_Texture* tex, UNIT_TYPE u) : texture(tex), unit(u) {}
	~Textures() {}
	SDL_Texture* texture;
	UNIT_TYPE unit;
};

class Animation
{
	friend class j1Animation;

public:
	Animation(std::string name);
	~Animation();

private:
	std::string name;
	UNIT_TYPE unit_type;
	ACTION_TYPE action_type;
	DIRECTION direction_type;

	std::vector<SDL_Rect> frames;
	std::vector<iPoint> pivot_points;

	float current_frame;
	bool loop = true;
	int loops = 0;
	float speed;	//More speed equals slow down the animation
	j1Timer	anim_timer;

public:
	void SetSpeed(float speed);
	void SetLoopState(bool state);
	
	bool Finished() const;
	bool FinishedLoop() const;
	void Reset();

private:
	bool CleanUp();
	void SetUnit(const pugi::xml_node node);
	void SetAction(const pugi::xml_node node);
	void SetDirection(const pugi::xml_node node);

	SDL_Rect& GetCurrentFrame();
	iPoint& GetCurrentPivotPoint();
};

//------------------------------------------------------------------------//

class j1Animation:public j1Module
{
public:

	j1Animation();

	// Destructor
	virtual ~j1Animation();
	
	bool Awake(pugi::xml_node& config);

	bool Start();

	bool CleanUp();
	
	Animation* DrawAnimation(const UNIT_TYPE unit, const ACTION_TYPE action, DIRECTION direction, iPoint pos);
	bool GetAnimationFrame(SDL_Rect& frame, iPoint& pivot, const Unit* unit);
	SDL_Texture* GetTexture(const UNIT_TYPE unit);
	Animation* GetAnimation(const UNIT_TYPE unit, const ACTION_TYPE action, DIRECTION direction);

private:
	std::vector<Textures> textures;
	std::list<Animation*> animations;

};

#endif