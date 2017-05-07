#include "M_DialogueManager.h"
#include "App.h"
#include "M_FileSystem.h"
#include "M_Audio.h"
#include "M_EntityManager.h"
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
	for (std::list<Dialogue>::iterator it = dialogues.begin(); it != dialogues.end(); it++)
	{
		if ((*it).active && !(*it).done)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
			{
				(*it).textLines.pop_front();
				if ((*it).textLines.empty())
				{
					(*it).active = false;
					(*it).done = true;
					onDialogue = false;
				}
			}
		}
	}
	return UPDATE_CONTINUE;
}
update_status M_DialogueManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
void M_DialogueManager::DrawDebug()
{
}

bool M_DialogueManager::PlayDialogue(DIALOGUE_EVENT event)
{

	app->audio->PlayFx(app->entityManager->fxAlert01);	//FX
	
	for (std::list<Dialogue>::iterator it = dialogues.begin(); it != dialogues.end(); it++)
	{
		if (!(*it).active && !(*it).done && (*it).event == event)
		{
			(*it).active = true;
			onDialogue = true;
			return true;
		}
	}
		return false;
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
		//GUILabel* tmp = new GUILabel("Dialogue", STANDARD_PRESET);
		GUILabel* tmp = new GUILabel(node.attribute("value").as_string(), SMALL, "Dialogue", STANDARD_PRESET, {0, 255, 0, 255});
		std::string str = node.attribute("value").as_string();
		//tmp->SetColor({ 255,255,255,255 });
		//tmp->SetText(str.c_str(), DEFAULT);
		tmp->SetGlobalPos(0, 50);
		tmp->FollowScreen(true); // Amb això a fals es quedara al mapa, si vols que segueixi la camara s'ha de posar a true
		
		textLines.push_back(tmp);
		node = node.next_sibling();
	}
}