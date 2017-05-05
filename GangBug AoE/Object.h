#pragma once
#include "Entity.h"
#include "j1Timer.h"

enum object_type
{
	BAMBOO1,//1
	BAMBOO2,//2
	BAMBOO3,//3
	BAMBOO4,//4
	BANNERA,//5
	BANNERB,//6
	HOUSE,//7
	TOWNHALL,//8
	HOUSE2,//9
	CHARRIOT,//10
	SKELETONS,//11
	SKULL_PILE,//12
	STONES1,//13
	STONES2,//14
	STONES3,//15
	STONES4,//16
	STONES5,//17
	STONES6,//18
	TRASH1,//19
	TRASH2,//20
	TREES1,//21
	TREES2,//22
	TREES3,//23
	TREES4,//24
	TREES5,//25
	TREES6,//26
	TREES7,//27
	TREES8,//28
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