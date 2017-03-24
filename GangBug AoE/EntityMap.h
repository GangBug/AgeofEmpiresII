#ifndef __ENTITY_MAP__
#define __ENTITY_MAP__


#include "Entity.h"

class EntityMap : public Entity
{
public:
	EntityMap(Entity* parent);
	virtual ~EntityMap();

	void OnStart()override;
	void OnFinish()override;

	void OnEnable()override;
	void OnDisable()override;

	bool OnSave(pugi::xml_node& node)const override;
	bool OnLoad(pugi::xml_node* node)override;
};

#endif // !__ENTITY_MAP__