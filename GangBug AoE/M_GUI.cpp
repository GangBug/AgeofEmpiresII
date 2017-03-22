#include "App.h"
#include "M_GUI.h"
#include "M_Input.h"
#include "M_Render.h"
#include "M_Textures.h"
#include "GB_Rectangle.h"

// GUI includes
#include "GUIElement.h"
#include "GUIImage.h"
#include "GUILabel.h"
#include "GUIButton.h"
#include "GUIInputText.h"
#include "GUIAutoLabel.h"
#include "GUIMouse.h"




M_GUI::M_GUI(bool startEnabled) : Module(startEnabled)
{
	name.assign("GUI");
	active = true;
}
M_GUI::~M_GUI()
{
}

bool M_GUI::Awake(pugi::xml_node &)
{
	
	
	return true;
}

bool M_GUI::Start()
{
	atlas = app->tex->Load("gui/atlas.png");

	/*GUIImage* img = new GUIImage();
	img->SetRectangle(0, 0, 231, 71);
	img->SetSection(0, 110, 231, 71);
	img->SetInteractive(true);		   
	img->SetCanFocus(true);			   
	guiList.push_back(img);			   

	GUILabel* label = new GUILabel();
	label->SetInteractive(true);
	label->SetCanFocus(true);
	label->SetText("Hello World!");
	label->CenterX();
	guiList.push_back(label);

	GUIButton* button = new GUIButton(GB_Rectangle<int>(0, 110, 231, 71), GB_Rectangle<int>(416, 171, 231, 71), GB_Rectangle<int>(647, 171, 231, 71));
	guiList.push_back(button);*/
	GUIInputText * input2 = new GUIInputText();
	input2->image->SetRectangle(0, 300, 231, 71);
	input2->label->SetLocalPos(0, 300);
	input2->SetRectangle(0, 300, 231, 71);
	guiList.push_back(input2);
	guiList.push_back(input2->image);
	guiList.push_back(input2->label);


	GUIInputText * input = new GUIInputText();	
	guiList.push_back(input);
	guiList.push_back(input->image);
	guiList.push_back(input->label);
	

	curs = new GUIMouse({ 0, 0 }, { 994,728, 25, 23 });
	//cursor  ------------------------------------------
	//curs = app->gui->createelement(uicursor, sdl_rect{ 994,728, 25, 23 }, p2point<int>{ 0, 0 },true);
	//curs->setlistener(this);

	lastFrameMS = new GUIAutoLabel<uint32>({ 0,0,30,30 }, &app->last_frame_ms);
	fps = new GUIAutoLabel<uint32>({ 0,30,30,30 }, &app->frames_on_last_update);
	debugGuiList.push_back(lastFrameMS);
	debugGuiList.push_back(fps);
	debugGuiList.push_back(CreateLabel({ 30,0,30,30 }, MEDIUM, "ms"));
	debugGuiList.push_back(CreateLabel({ 30,30,30,30 }, MEDIUM, "fps"));

	
	xMouse = new GUILabel("", SMALL);
	yMouse = new GUILabel("", SMALL);

	debugGuiList.push_back(xMouse);
	debugGuiList.push_back(yMouse);

	return true;
}

update_status M_GUI::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ManageEvents();
	
	if (focus != nullptr && focus->GetType() == GUI_INPUT_TEXT)
	{
		//printf("Texti input has focus");
		app->input->StartTyping();
	}
	else
	{
		app->input->StopTyping();
	}
	// TODO do Update()
	std::list<GUIElement*>::iterator it;
	for (it = guiList.begin(); it != guiList.end(); it++)
	{
		(*it)->Update(mouseHover, focus);
	}
	for (it = debugGuiList.begin(); it != debugGuiList.end(); it++)
	{
		(*it)->Update(mouseHover, focus);
	}
	return ret;
}

update_status M_GUI::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

update_status M_GUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool M_GUI::UpdateGuiList()
{
	return true;
}

bool M_GUI::UpdateDebugGuiList()
{
	return true;
}

//Checks if cursor is inside an element | returns null if nothing found

GUIElement * M_GUI::FindMouseHover()
{
	GUIElement* ret = nullptr;
	std::list<GUIElement*>::reverse_iterator it;

	for (it = debugGuiList.rbegin(); it != debugGuiList.rend(); it++)
	{
		if ((*it)->CheckMouseOver())
		{
			ret = (*it);
		}
	}
	for (it = guiList.rbegin(); it != guiList.rend(); it++)
	{
		if ((*it)->CheckMouseOver())
		{
			ret = (*it);
		}
	}	

	return ret;
}

//Manages the events on hover and focus

void M_GUI::ManageEvents()
{
	GUIElement* newMouseHover = nullptr;

	//Find the element that is hovered actually
	newMouseHover = FindMouseHover();
	if (focus != nullptr && newMouseHover == nullptr && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_DOWN)
	{
		BroadcastEventToListeners(focus, LOST_FOUCS);
		focus = nullptr;
	}
	//If the hover is null it gets the new element to hover
	if (mouseHover == nullptr && newMouseHover != nullptr)
	{
		mouseHover = newMouseHover;
		mouseHover->SetMouseInside(true);
		BroadcastEventToListeners(mouseHover, MOUSE_ENTERS);
	}
	//If the hovered elements are diferent events ant status are managed here
	if (mouseHover != newMouseHover && newMouseHover != nullptr)
	{
		//Send leaving event
		BroadcastEventToListeners(mouseHover, MOUSE_LEAVES);
		//Set the new hover
		mouseHover->SetMouseInside(false);
		mouseHover = newMouseHover;
		mouseHover->SetMouseInside(true);
		//send entering event
		BroadcastEventToListeners(mouseHover, MOUSE_ENTERS);
	}
	if (newMouseHover == nullptr && mouseHover != nullptr) //This is maybe unnecessary, but i think this check here helps to a better readability
	{
		BroadcastEventToListeners(mouseHover, MOUSE_LEAVES);
		mouseHover->SetMouseInside(false);
		mouseHover = nullptr;				
	}
	// TODO trobar quin element te el focus
	// TODO manage the input & events
	if (mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_DOWN)
	{
		if (focus != mouseHover && focus != nullptr)
		{
			BroadcastEventToListeners(focus, LOST_FOUCS);
		}
		focus = mouseHover;
		mouseHover->SetLClicked(true);
		BroadcastEventToListeners(mouseHover, MOUSE_LCLICK_DOWN);
		BroadcastEventToListeners(mouseHover, GAIN_FOCUS);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_state::KEY_UP)
	{
		BroadcastEventToListeners(mouseHover, MOUSE_LCLICK_UP);
		mouseHover->SetLClicked(false);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_DOWN)
	{
		if (focus != mouseHover)
		{
			BroadcastEventToListeners(focus, LOST_FOUCS);
		}
		focus = mouseHover;
		mouseHover->SetRClicked(true);
		BroadcastEventToListeners(mouseHover, MOUSE_RCLICK_DOWN);
		BroadcastEventToListeners(mouseHover, GAIN_FOCUS);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_state::KEY_UP)
	{
		BroadcastEventToListeners(mouseHover, MOUSE_RCLICK_UP);
		mouseHover->SetRClicked(false);
	}
}

//Broadcast an event to all GUIElement listeners

void M_GUI::BroadcastEventToListeners(GUIElement * element, gui_events event)
{
	if (event != MOUSE_ENTERS)
		SDL_Log("Event: %d", event);
	//First we get listeners list of previous element hovered
	std::list<Module*> tmpListeners = element->GetListeners();
	//Iterate over listeners list to send them hover is lost
	for (std::list<Module*>::iterator it = tmpListeners.begin(); it != tmpListeners.end(); it++)
	{
		(*it)->GuiEvent(element, event);		
	}
}

void M_GUI::Draw()
{
	for (std::list<GUIElement*>::iterator it = guiList.begin(); it != guiList.end(); it++)
	{
		(*it)->Draw();
	}
}

void M_GUI::DrawEditor()
{
	for (std::list<GUIElement*>::iterator it = editorGuiList.begin(); it != editorGuiList.end(); it++)
	{
		(*it)->Draw();
	}
}

void M_GUI::DrawDebug()
{
	//GB_Rectangle<int> rect;
	//rect.x = 0;
	//rect.y = 0;
	//SDL_QueryTexture(atlas, NULL, NULL, &rect.w, &rect.h);
	//SDL_Rect sdlrect = rect.GetSDLrect();
	//sdlrect.x = 0;
	//sdlrect.y = 110;
	//sdlrect.w = 231;
	//sdlrect.h = 71;
	//app->render->Blit(atlas, 0, 0, &sdlrect);
	int x;
	int y;
	app->input->GetMousePosition(x, y);
	xMouse->SetText(std::to_string(x).c_str(), SMALL);
	yMouse->SetText(std::to_string(y).c_str(), SMALL);
	xMouse->SetLocalPos(x + 10, y);
	yMouse->SetLocalPos(x + 40, y);

	for (std::list<GUIElement*>::iterator it = guiList.begin(); it != guiList.end(); it++)
	{
		GB_Rectangle<int> rect = (*it)->GetRectangle();
		app->render->DrawQuad({ rect.x, rect.y, rect.w, rect.h }, 0, 255, 0, 255, false, false);
	}
	for (std::list<GUIElement*>::iterator it = debugGuiList.begin(); it != debugGuiList.end(); it++)
	{
		(*it)->Draw();
	}
	
}

SDL_Texture* M_GUI::GetAtlas() const
{
	return atlas;
}

void M_GUI::SetAtlas(SDL_Texture * texture)
{
	atlas = texture;
}

GUIButton * M_GUI::CreateButton(GB_Rectangle<int> _position, 
								GB_Rectangle<int> _standBySection, 
								GB_Rectangle<int> _hoverSection, 
								GB_Rectangle<int> _clickedSection)
{
	GUIButton* button = new GUIButton(_position, _standBySection, _hoverSection, _clickedSection);
	return button;
}

GUILabel * M_GUI::CreateLabel(GB_Rectangle<int> _position, size _size, const char* _text)
{
	GUILabel* label;
	if (_text != nullptr)
	{
		label = new GUILabel(_text, _size);
	}
	else
	{
		label = new GUILabel();
	}
	label->SetRectangle(_position);
	label->SetLocalPos(_position.x, _position.y);
	return label;
}

GUIImage * M_GUI::CreateImage(GB_Rectangle<int> _position, GB_Rectangle<int> _section)
{
	GUIImage* image = new GUIImage();
	image->SetSection(_section);
	image->SetRectangle(_position);
	return image;
}

GUIElement * M_GUI::GuiFactory()
{





	return nullptr;
}

