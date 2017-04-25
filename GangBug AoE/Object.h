#pragma once
#include "Entity.h"
#include "j1Timer.h"

enum object_type
{
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
