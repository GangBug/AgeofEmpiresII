#ifndef __M_DIALOGUE_MANAGER_H__
#define __M_DIALOGUE_MANAGER_H__

#include "Module.h"
#include "GUILabel.h"
#include <vector>
#include <string>
#include "GUIBox.h"

#define TEXT_OFFSET_X 115
#define TEXT_OFFSET_Y 73
#define D_BOX_OFFSET_X 436
#define D_BOX_OFFSET_Y 100

enum DIALOGUE_CHARACTER
{
	D_CHARACTER_SAMURAI,
	D_CHARACTER_DEMON,
	D_CHARACTER_NONE
};

enum DIALOGUE_EVENT
{
	D_EVENT_FIRST_MISSION,
	D_EVENT_FIRST_MISSION_FINISH,
	D_EVENT_TOWN_REPAIR,
	D_EVENT_WAVES_START,
	D_EVENT_DIABLO_SPAWN_SAMURAI,
	D_EVENT_DIABLO_SPAWN_DIABLO,
	D_EVENT_VICTORY_DIABLO,
	D_EVENT_VICTORY_SAMURAI,
	D_EVENT_DEFEAT,
	D_EVENT_NONE
};

struct Dialogue
{
public:
	Dialogue();
	std::list<GUILabel*> textLines;
	DIALOGUE_CHARACTER character;
	DIALOGUE_EVENT event;
	bool done = false;
	bool active = false;

	void SetCharacter(pugi::xml_node);
	void SetEvent(pugi::xml_node);
	void SetText(pugi::xml_node);
};

class M_DialogueManager : public Module
{
public:
	M_DialogueManager(bool startEnabled = false);
	virtual ~M_DialogueManager();

	bool Start();

	bool CleanUp();

	update_status Update(float dt)override;
	update_status PostUpdate(float dt)override;

	void DrawDebug()override;

	bool PlayDialogue(DIALOGUE_EVENT event);

	

public:
	bool onDialogue = false;
	std::list<Dialogue> dialogues;
	GUIBox* boxDiag;
};

#endif