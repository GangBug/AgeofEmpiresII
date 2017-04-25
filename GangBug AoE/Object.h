#pragma once
#include "Entity.h"
#include "j1Timer.h"

enum object_type
{
	ASIAN_WALL_H, //0
	ASIAN_WALL_L,//1
	ASIAN_WALL_R,//2
	ASIAN_WALL_T,//3
	ASIAN_WALL_V,//4
	ASIAN_WALL_R_DMG,//5
	ASIAN_WALL_L_DMG,//6
	ASIAN_WALL_T_DMG,//7
	ASIAN_WALL_H_DMG,//8
	ASIAN_WALL_V_DMG,//9
	BAMBOO1,//10
	BAMBOO2,//11
	BAMBOO3,//12
	BAMBOO4,//13
	BANNERA,//14
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

class Object :
	public Entity 
{
public:
	Object(object_type objectType,Entity* parent );
	virtual ~Object();
	void OnUpdate(float dt)override;

public:
	enum object_type objectType;
	bool selected = false; // future
};