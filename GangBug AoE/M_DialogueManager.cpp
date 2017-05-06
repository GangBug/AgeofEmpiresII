#include "M_DialogueManager.h"
#include "App.h"
#include "M_FileSystem.h"
#include "M_Render.h"
#include "M_Textures.h"
#include "M_Input.h"
#include "Log.h"

M_DialogueManager::M_DialogueManager(bool startEnabled) : Module(startEnabled)
{
	name.assign("dialogue_manager");
}

M_DialogueManager::~M_DialogueManager()
{
}

bool M_DialogueManager::Start()
{
	std::string dialogue_folder = "dialogue/dialogues.xml";	//Load dialogues data from dialogue folder
	char* buff = nullptr;
	int size = app->fs->Load(dialogue_folder.c_str(), &buff);
	pugi::xml_document dialogue_data;
	pugi::xml_parse_result result = dialogue_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading dialogues data: %s", result.description());
		return false;
	}

	//Loading Objects Sprites
	pugi::xml_node dialogueNode = dialogue_data.child("dialogues").first_child();
	while (dialogueNode != NULL)
	{
		Dialogue newDialogue;
		newDialogue.SetCharacter(dialogueNode);
		newDialogue.SetEvent(dialogueNode);
		newDialogue.SetText(dialogueNode.child("text"));

		if (newDialogue.event != D_EVENT_NONE)
		{
			dialogues.push_back(newDialogue);
		}
		dialogueNode = dialogueNode.next_sibling();
	}

	return true;
}

update_status M_DialogueManager::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_V) == KEY_UP)
	{
		dialogues.begin()._Ptr->_Myval.active = true;
	}

	for (std::list<Dialogue>::iterator it = dialogues.begin(); it != dialogues.end(); it++)
	{
		if ((*it).active && !(*it).done)
		{
			(*it).textLines.begin()._Ptr->_Myval->SetVisible(true);
			(*it).textLines.begin()._Ptr->_Myval->SetActive(true);
			(*it).textLines.begin()._Ptr->_Myval->Draw();
			if (app->input->GetKey(SDL_SCANCODE_P) == KEY_UP)
			{
				(*it).textLines.pop_front();
				if (!(*it).textLines.empty())
				{
					(*it).active = false;
					(*it).done = true;
				}
			}
		}
	}
	return UPDATE_CONTINUE;
}

void M_DialogueManager::DrawDebug()
{
}

//-------------------

Dialogue::Dialogue()
{
	event = D_EVENT_NONE;
}


void Dialogue::SetCharacter(pugi::xml_node node)
{
	if (strcmp(node.attribute("character").as_string(), "samurai") == 0) {
		character = D_CHARACTER_SAMURAI;
	}
	else if (strcmp(node.attribute("character").as_string(), "demon") == 0) {
		character = D_CHARACTER_DEMON;
	}
	else {
		character = D_CHARACTER_NONE;
	}
}

void Dialogue::SetEvent(pugi::xml_node node)
{
	if (strcmp(node.attribute("event").as_string(), "firstEncounter") == 0) {
		event = D_EVENT_FIRST_ENCOUNTER;
	}
	else {
		event = D_EVENT_NONE;
	}
}

void Dialogue::SetText(pugi::xml_node node)
{
	while (node != NULL)
	{
		GUILabel* tmp = new GUILabel(node.attribute("value").as_string(), SMALL, "Dialogue", STANDARD_PRESET);
		tmp->SetGlobalPos(0, 0);
		tmp->FollowScreen(false);
		textLines.push_back(tmp);
		node = node.next_sibling();
	}
}