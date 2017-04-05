#pragma once
#ifndef _OBJECT
#define _OBJECT

#include "Entity.h"
#include "SDL/include/SDL_rect.h"

enum OBJECT_TYPE
{
	ASIAN_WALL_H,
	ASIAN_WALL_L,
	ASIAN_WALL_R,
	ASIAN_WALL_T,
	ASIAN_WALL_V,
	ASIAN_WALL_R_DMG,
	ASIAN_WALL_L_DMG,
	ASIAN_WALL_T_DMG,
	ASIAN_WALL_H_DMG,
	ASIAN_WALL_V_DMG,
	BAMBOO1,
	BAMBOO2,
	BAMBOO3,
	BAMBOO4,
	BANNERA,
	BANNERB,
	CHARRIOT,
	PALISADE_R,
	PALISADE_L,
	PALISADE_T,
	PALISADE_H,
	PALISADE_V,
	SKELETONS_GROUP,
	SKULL_PILE,
	TORCH_FLOOR_SQUARE,
	TORCH_FLOOR_TWISTED,
	TRASH_LARGE,
	TRASH_MEDIUM,
	TRASH_SMALL,
	ALTAR,
	OBJECT_NONE

};

class Object : public Entity
{
public:
	Object(OBJECT_TYPE oType, fPoint pos);
	~Object();
	void Update();
	void PostUpdate() {}
	void Draw() {}

	OBJECT_TYPE type;
};

#endif