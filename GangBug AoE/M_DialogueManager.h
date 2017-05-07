#ifndef __M_DIALOGUE_MANAGER_H__
#define __M_DIALOGUE_MANAGER_H__

#include "Module.h"
#include "GUILabel.h"
#include <vector>
#include <string>

#define TEXT_OFFSET_X 50
#define TEXT_OFFSET_Y 550
#define D_BOX_OFFSET_Y 500

enum DIALOGUE_CHARACTER
{
	D_CHARACTER_SAMURAI,
	D_CHARACTER_DEMON,
	D_CHARACTER_NONE
};

enum DIALOGUE_EVENT
{
	D_EVENT_FIRST_ENCOUNTER,
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
};

#endif