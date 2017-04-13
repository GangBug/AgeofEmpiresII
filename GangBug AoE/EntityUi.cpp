#include "EntityUi.h"

#include "M_GUI.h"
#include "GUILabel.h"


EntityUi::EntityUi(Entity* parent) : Entity(ENTITY_UI, parent)
{
	name.assign("entity_ui");
	//TMP
	//GUIElement* e1 = app->gui->CreateLabel({ 50, 50, 100, 100 }, SMALL, "Hi im a test.");
	//app->gui->guiList.push_back(e1);
}


EntityUi::~EntityUi()
{
}

void EntityUi::OnStart()
{
	//TODO: Create all ui elements.
}

void EntityUi::OnFinish()
{
	//TODO: Destroy all ui elements.
}

void EntityUi::OnEnable()
{
	//TODO: Enable all ui elements.
}

void EntityUi::OnDisable()
{
	//TODO: Disable all ui elements.
}

bool EntityUi::OnSave(pugi::xml_node& node)const
{
	//TODO: Save all ui elements.
	return true;
}

bool EntityUi::OnLoad(pugi::xml_node* node)
{
	//TODO: Load all ui elements.
	return true;
}
