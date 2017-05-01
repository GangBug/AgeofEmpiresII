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
	BANNERB,//15
	CHARRIOT,//16
	PALISADE_R,//17
	PALISADE_L,//18
	PALISADE_T,//19
	PALISADE_H,//20
	PALISADE_V,//21
	SKELETONS_GROUP,//22
	SKULL_PILE,//23
	TORCH_FLOOR_SQUARE,//24
	TORCH_FLOOR_TWISTED,//25
	TRASH_LARGE,//26
	TRASH_MEDIUM,//27
	TRASH_SMALL,//28
	ALTAR,//29
	BAMBOO,//30	
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