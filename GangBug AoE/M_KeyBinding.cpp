#include "App.h"
#include "Log.h"
#include "M_KeyBinding.h"



M_KeyBinding::M_KeyBinding(bool enabled): Module(enabled)
{
	name.assign("key_bindings");
}


M_KeyBinding::~M_KeyBinding()
{
}

bool M_KeyBinding::Awake(pugi::xml_node & config)
{
	for (pugi::xml_node it = config.first_child(); it != NULL; it = it.next_sibling())
	{
		if (strcmp(it.attribute("name").as_string(), "cameraUp") == 0)
		{
			const char* key = it.first_child().value();
			cameraUp.scancode = charToScancode(key);
			if (cameraUp.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}
		if (strcmp(it.attribute("name").as_string(), "cameraDown") == 0)
		{
			const char* key = it.first_child().value();
			cameraDown.scancode = charToScancode(key);
			if (cameraDown.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}
		if (strcmp(it.attribute("name").as_string(), "cameraLeft") == 0)
		{
			const char* key = it.first_child().value();
			cameraLeft.scancode = charToScancode(key);
			if (cameraLeft.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}
		if (strcmp(it.attribute("name").as_string(), "cameraRight") == 0)
		{
			const char* key = it.first_child().value();
			cameraRight.scancode = charToScancode(key);
			if (cameraRight.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}
		if (strcmp(it.attribute("name").as_string(), "selectAllUnitsOfTypeSelected") == 0)
		{
			const char* key = it.first_child().value();
			selectAllUnitsOfTypeSelected.scancode = charToScancode(key);
			if (selectAllUnitsOfTypeSelected.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}
		if (strcmp(it.attribute("name").as_string(), "selectArchery") == 0)
		{
			const char* key = it.first_child().value();
			selectArchery.scancode = charToScancode(key);
			if (selectArchery.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}
		if (strcmp(it.attribute("name").as_string(), "selectStable") == 0)
		{
			const char* key = it.first_child().value();
			selectStable.scancode = charToScancode(key);
			if (selectStable.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}
		if (strcmp(it.attribute("name").as_string(), "selectBarracks") == 0)
		{
			const char* key = it.first_child().value();
			selectBarracks.scancode = charToScancode(key);
			if (selectBarracks.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}
		if (strcmp(it.attribute("name").as_string(), "selectCastle") == 0)
		{
			const char* key = it.first_child().value();
			selectCastle.scancode = charToScancode(key);
			if (selectCastle.scancode == SDL_SCANCODE_UNKNOWN)
			{
				SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    KeyBinding Failure    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return false;
			}
		}


	}
	return true;
}

bool M_KeyBinding::Start()
{
	return true;
}

update_status M_KeyBinding::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_KeyBinding::Update(float dt)
{
	SDL_Log("UP Scancode: %s", SDL_GetScancodeName(cameraUp.scancode));
	
	return UPDATE_CONTINUE;
}

update_status M_KeyBinding::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool M_KeyBinding::CleanUp()
{
	return true;
}

bool M_KeyBinding::Load(pugi::xml_node &)
{
	return true;
}

bool M_KeyBinding::Save(pugi::xml_node &) const
{
	return true;
}

void M_KeyBinding::GuiEvent(GUIElement * element, int64_t event)
{
}

void M_KeyBinding::DrawDebug()
{
}

SDL_Scancode M_KeyBinding::charToScancode(const char* c)
{
	SDL_Keycode kycd = SDL_GetKeyFromName(c);
	return SDL_GetScancodeFromKey(kycd);
}
