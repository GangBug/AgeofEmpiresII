#ifndef __M_ANIMATION_H__
#define __M_ANIMATION_H__

#include "Module.h"
#include <vector>
#include <list>
#include "GB_Rectangle.h"
#include "p2Point.h"
#include "Unit.h"
#include "j1Timer.h"

struct SDL_Texture;

struct Texture
{
public:
	Texture(SDL_Texture* tex, unit_type type) : texture(tex), unit(type) {}
	~Texture() {}
	SDL_Texture* texture;
	unit_type unit;
};

class Animation
{
	friend class M_Animation;
public:
	Animation(std::string name);
	~Animation();

	void SetLoop(bool loop);
	void SetSpeed(float speed);
	bool Finished() const;
	void Reset();

private:
	void CleanUp();
	void SetUnit(const pugi::xml_node node);
	void SetAction(const pugi::xml_node node);//TODO
	void SetDirection(const pugi::xml_node node);//TODO

	GB_Rectangle<int>& GetCurrentFrame();
	iPoint& GetCurrentPivot();
	
private:
	std::string name;
	unit_type unitType;

	std::vector<GB_Rectangle<int>> frames;
	std::vector<iPoint> pivotPoints;

	float currentFrame;
	bool loop = true;
	int loops = 0;
	float speed = 50.0f;
	Timer animationTimer;

};

class M_Animation : public Module
{
public:
	M_Animation(bool startEnabled = true);
	virtual ~M_Animation();

	bool Awake(pugi::xml_node& none);
	bool Start();
	bool CleanUp();

	Animation* DrawAnim(unit_type unit, iPoint pos);
	SDL_Texture* GetTexture(unit_type type)const;
	Animation* GetAnimation(unit_type unit)const;//needs direction/action too
	void GetFrame(GB_Rectangle<int>& rect, iPoint& pivot, const Unit* unit);

	void DrawDebug()override;

private:
	std::vector<Texture> textures;
	std::list<Animation*> animations;

};

#endif //__M_ANIMATION_H__
