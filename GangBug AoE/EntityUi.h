#ifndef __ENTITY_UI__
#define __ENTITY_UI__


#include "Entity.h"

class EntityUi : public Entity
{
public:
	EntityUi(Entity* parent);
	virtual ~EntityUi();

	void OnStart()override;
	void OnFinish()override;

	void OnEnable()override;
	void OnDisable()override;

	bool OnSave(pugi::xml_node& node)const override;
	bool OnLoad(pugi::xml_node* node)override;
};

#endif // !__ENTITY_UI__