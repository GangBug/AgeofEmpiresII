#pragma once
#include "Entity.h"
#include "j1Timer.h"

enum object_type
{
	OBJECT_NONE,
	BAMBOO1,//1
	BAMBOO2,//2
	BAMBOO3,//3
	BAMBOO4,//4
	BANNERA,//5
	BANNERB,//6
	ARCHERY,//7
	BARRACK,//8
	GREATHOUSE,//9
	HALL,//10
	HOUSE1,//11
	HOUSE2,//12
	HOUSE3,//13
	LUMBER,//14
	MARKET,//15
	MINE,//16
	MONASTERY,//17
	SIEGE,//18
	STABLE,//19
	UNIVERSITY,//20
	WONDER,//21
	CHARRIOT,//22
	GOLD1,//23
	GOLD2,//24
	GOLD3,//25
	GOLD4,//26
	GOLD5,//27
	GOLD6,//28
	SKELETONS1,//29
	SKULLPILE,//30
	STONE1,//31
	STONE2,//32
	STONE3,//33
	STONE4,//34
	STONE5,//35
	STONE6,//36
	TRASHLARGUE1,//37
	TRASHLARGUE2,//38
	TREES1,//39
	TREES2,//40
	TREES3,//41
	TREES4,//42
	TREES5,//43
	TREES6,//44
	TREES7,//45
	TREES8//46

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