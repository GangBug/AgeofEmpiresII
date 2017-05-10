#include "PlayerManager.h"

#include "App.h"
#include "M_EntityManager.h"
#include "M_Input.h"

#include "M_Render.h"

#include "Building.h"


PlayerManager::PlayerManager(Entity* parent) : Entity(ENTITY_PLAYER_MAN, parent)
{
	name.assign("player_manager");
}


PlayerManager::~PlayerManager()
{
}

void PlayerManager::OnStart()
{
}

void PlayerManager::OnFinish()
{
}

void PlayerManager::OnEnable()
{
}

void PlayerManager::OnDisable()
{
}

void PlayerManager::OnUpdate(float dt)
{
	static iPoint clickPosition;
	iPoint mPos;
	app->input->GetMouseMapPosition(mPos.x, mPos.y);

	key_state buttonLeftStat = app->input->GetMouseButtonDown(SDL_BUTTON_LEFT);

	if (buttonLeftStat == KEY_DOWN)
	{
		clickPosition = mPos;
		selectionRect.x = mPos.x;
		selectionRect.y = mPos.y;
		selectionRect.w = 0;
		selectionRect.h = 0;

		for (std::vector<Entity*>::iterator it = selectedEntities.begin(); it != selectedEntities.end(); ++it)
		{
			(*it)->selected = false;
		}
		selectedEntities.clear();
		onSelection = true;
	}

	if (onSelection == true)
	{
		if (buttonLeftStat == KEY_REPEAT)
		{
			iPoint dif = mPos - clickPosition;

			if (dif.x > 0)
			{
				selectionRect.x = clickPosition.x;
				selectionRect.w = dif.x;
			}
			else
			{
				selectionRect.x = clickPosition.x + dif.x;
				selectionRect.w = -dif.x;
			}

			if (dif.y > 0)
			{
				selectionRect.y = clickPosition.y;
				selectionRect.h = dif.y;
			}
			else
			{
				selectionRect.y = clickPosition.y + dif.y;
				selectionRect.h = -dif.y;
			}
		}
		else if (buttonLeftStat == KEY_UP)
		{
			app->entityManager->GetEntitiesOnRect(ENTITY_BASE | ENTITY_UNIT, selectedEntities, selectionRect);
			for (std::vector<Entity*>::iterator it = selectedEntities.begin(); it != selectedEntities.end(); ++it)
			{
				if (selectionRect.Contains((*it)->GetGlobalPosition().x, (*it)->GetGlobalPosition().y))
					(*it)->selected = true;
			}
			onSelection = false;
		}
	}
}

bool PlayerManager::OnSave(pugi::xml_node& node) const
{
	return true;
}

bool PlayerManager::OnLoad(pugi::xml_node * node)
{
	return true;
}

void PlayerManager::DrawDebug()
{
	//TMP/TEST
	for (std::vector<Entity*>::iterator it = selectedEntities.begin(); it != selectedEntities.end(); ++it)
	{
		Entity* tmp = (*it);
		if (tmp != nullptr)
		{
			iPoint p = tmp->GetPivot();
			GB_Rectangle<int> r = tmp->GetEnclosingBox();
			r.x -= p.x;
			r.y -= p.y;
			app->render->DrawQuad(r.GetSDLrect(), 0, 0, 255, 255);
		}
	}
}
