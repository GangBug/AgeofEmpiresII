#ifndef __PLAYER_MANAGER__
#define __PLAYER_MANAGER__

#include "Entity.h"

#include <vector>

class PlayerManager : public Entity
{
public:
	PlayerManager(Entity* parent);
	virtual ~PlayerManager();

	void OnStart()override;
	void OnFinish()override;
	void OnEnable()override;
	void OnDisable()override;

	void OnUpdate(float dt)override;

	bool OnSave(pugi::xml_node& node)const override;
	bool OnLoad(pugi::xml_node* node)override;

	void DrawDebug()override;

	bool GetOnSelection() const { return onSelection; }

private:


public:
	GB_Rectangle<int> selectionRect = ZeroRectangle;
	std::vector<Entity*> selectedEntities;
	bool onSelection = false;
	bool draggingCamera = false;
private:

};

#endif // !__PLAYER_MANAGER__