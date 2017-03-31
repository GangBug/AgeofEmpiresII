#include "j1App.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1FileSystem.h"
#include "GB_Rectangle.h"
#include "p2Log.h"

#include <iostream> 
#include <sstream> 

#include "Bezier.h"

// GUI includes
#include "GUIElement.h"
#include "GUIImage.h"
#include "GUILabel.h"
#include "GUIButton.h"
#include "GUIAutoLabel.h"




j1GUI::j1GUI()
{
	name.assign("GUI");
	active = true;
}
j1GUI::~j1GUI()
{
}

bool j1GUI::Awake(pugi::xml_node &)
{
	cBeizier = new CBeizier();

	return true;
}
bool j1GUI::Start()
{
	bool ret = true;
	//TODO 1: Load atlas
	atlas = App->tex->Load("gui/SS_Menu.png");

	////Debug UI
	lastFrameMS = new GUIAutoLabel<uint32>({ 0,0,30,30 }, &App->last_frame_ms, "ms");
	fps = new GUIAutoLabel<uint32>({ 0,30,30,30 }, &App->frames_on_last_update, "fps");
	debugGuiList.push_back(lastFrameMS);
	debugGuiList.push_back(fps);
	debugGuiList.push_back(CreateLabel({ 30,0,30,30 }, MEDIUM, "ms"));
	debugGuiList.push_back(CreateLabel({ 30,30,30,30 }, MEDIUM, "fps"));

	xMouse = new GUILabel("", SMALL, "mousex");
	yMouse = new GUILabel("", SMALL, "mousey");

	debugGuiList.push_back(xMouse);
	debugGuiList.push_back(yMouse);

	return ret;
}
bool j1GUI::PreUpdate()
{
	bool ret = true;

	ManageEvents();

	if (focus != nullptr && focus->GetType() == GUI_INPUT_TEXT)
		App->input->StartTyping();
	else
		App->input->StopTyping();

	//This code depends on UI Edition or not
	//This is the git gud code...
	//IterateList(&guiList, &M_GUI::DoElementUpdate, dt);
	//IterateList(&debugGuiList, &M_GUI::DoElementUpdate, dt);


	for (std::list<GUIElement*>::iterator it = guiList.begin(); it != guiList.end(); it++)
	{
		if ((*it)->GetActive())
			(*it)->Update(mouseHover, focus);
	}

	for (std::list<GUIElement*>::iterator it = debugGuiList.begin(); it != debugGuiList.end(); it++)
	{
		if ((*it)->GetActive())
			(*it)->Update(mouseHover, focus);
	}

	return ret;
}
bool j1GUI::Update(float dt)
{
	bool ret = true;

	return ret;
}
bool j1GUI::PostUpdate()
{
	if (UIEditing && App->input->GetKey(SDL_SCANCODE_2) == KEY_UP)
	{
		bool ret = SaveLayout();
		if (!ret)
		{
			LOG("Error loading ui.xml");
			return false;
		}
	}
	return true;
}
//TODO: LoadLayout needs lots of improvements...
bool j1GUI::LoadLayout(std::string _path)
{
	bool ret = true;

	//Cleaning guiList to override it
	if (guiList.size() > 0)
	{
		LOG("Deleting guiList UI");
		for (std::list<GUIElement*>::iterator it = guiList.begin(); it != guiList.end(); it++)
		{
			RELEASE((*it));
		}
		guiList.clear();
	}

	LOG("Loading UI xml");
	char* buffer = nullptr;
	char path[256];
	//sprintf_s(path, 256, "%sui.xml", app->fs->GetSaveDirectory());
	uint size;
	if (_path != "")
	{
		size = App->fs->Load(_path.c_str(), &buffer);
		guiLoadedPath = _path;
	}
	else
	{
		size = App->fs->Load("gui/gui.xml", &buffer);
	}
	
	if (size > 0 && buffer != nullptr)
	{
		LOG("Loading UI.");

		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size - 1);
		RELEASE(buffer);

		if (result != NULL)
		{
			root = data.child("gui").child("elements");
			pugi::xml_node buttons = root.child("buttons");
			pugi::xml_node imgs = root.child("imgs");
			//TODO 2: Load UI presets
			//Loading button presets
			for (pugi::xml_node it = buttons.first_child(); it != NULL; it = it.next_sibling())
			{
				std::string name = it.attribute("type_name").as_string();
				pugi::xml_object_range<pugi::xml_node_iterator> sections = it.child("sections").children();

				GB_Rectangle<int> stb;
				GB_Rectangle<int> hov;
				GB_Rectangle<int> clk;
				for (pugi::xml_node_iterator it_sections = sections.begin(); it_sections != sections.end(); it_sections++)
				{
					int flag = 0;
					if (!strcmp(it_sections->attribute("type").as_string(), "standBy"))
						flag = (1 << 1);
					else if (!strcmp((*it_sections).attribute("type").as_string(), "hover"))
						flag = (1 << 2);
					else if (!strcmp((*it_sections).attribute("type").as_string(), "clicked"))
						flag = (1 << 3);

					switch (flag)
					{
					case 2:
						stb.x = (*it_sections).attribute("x").as_int();
						stb.y = (*it_sections).attribute("y").as_int();
						stb.w = (*it_sections).attribute("w").as_int();
						stb.h = (*it_sections).attribute("h").as_int();
						break;
					case 4:
						hov.x = (*it_sections).attribute("x").as_int();
						hov.y = (*it_sections).attribute("y").as_int();
						hov.w = (*it_sections).attribute("w").as_int();
						hov.h = (*it_sections).attribute("h").as_int();
						break;
					case 8:
						clk.x = (*it_sections).attribute("x").as_int();
						clk.y = (*it_sections).attribute("y").as_int();
						clk.w = (*it_sections).attribute("w").as_int();
						clk.h = (*it_sections).attribute("h").as_int();
						break;
					default:
						ret = false;
						break;
					}
				}
				GUIButton* btn = CreateButton({ 0,0,stb.w,stb.h }, stb, hov, clk, name);
				btn->SetPresetType(name);
				guiPresets.insert(std::pair<std::string, GUIElement*>(name, btn));
			}

			//Loading image presets
			for (pugi::xml_node it = imgs.first_child(); it != NULL; it = it.next_sibling())
			{
				std::string name = it.attribute("type_name").as_string();
				pugi::xml_node section = it.child("section");

				GB_Rectangle<int> stb;
				if (!strcmp(section.attribute("type").as_string(), "standBy"))
				{
					stb.x = section.attribute("x").as_int();
					stb.y = section.attribute("y").as_int();
					stb.w = section.attribute("w").as_int();
					stb.h = section.attribute("h").as_int();
				}
				else
				{
					ret = false;
				}
				GUIImage* img = CreateImage({ 0,0,stb.w,stb.h }, stb, name);
				img->SetPresetType(name);
				guiPresets.insert(std::pair<std::string, GUIElement*>(name, img));
			}

			//TODO: Make label presets makes no sense for me, any other new GUI presset goes here, but for the moment
			//		only 3 Gui items are implemented Label, Image and Button...
			// bla bla some preset SLIDER
			// bla bla some preset LIFEBAR
			// NEWER PRESETS go up this comments

			//TODO: Load listeners and events

			//Load UI layout
			root = data.child("gui").child("layout");
			//pugi::xml_object_range<pugi::xml_node_iterator> elements = root.children();
			//for (pugi::xml_node_iterator it_elements = elements.begin();
			//	 it_elements != elements.end(); it_elements++)
			for (pugi::xml_node layout_element = root.first_child(); layout_element; )
			{
				pugi::xml_node next = layout_element.next_sibling();
				//BUTTON
				if (!strcmp(layout_element.name(), "button"))
				{
					std::string preset = layout_element.attribute("type").as_string();
					std::map<std::string, GUIElement*>::iterator node = guiPresets.find(preset);
					std::string name = layout_element.attribute("name").as_string("");
					GUIElement* tmpElement = FindElement(guiList, name);
					// If name is "" or another element with same name exists does not load the element, 
					// same if not preset found in presets list
					if (node != guiPresets.end() && tmpElement == nullptr && name != "")
					{
						GUIButton* btn = CreateButtonFromPreset({ 0,0,0,0 }, preset, name, "hola");
						btn->Deserialize(layout_element);
						guiList.push_back(btn);
						LOG("Item %s created", layout_element.name());
					}
					else if (tmpElement != nullptr)
					{
						LOG("Item %s not created, another element exists with same name, (%s)", layout_element.name(), name.c_str());
					}
					else if (node == guiPresets.end())
					{
						LOG("Item %s not created, no preset found for %s", layout_element.name(), name.c_str());
					}
					else if (name == "")
					{
						LOG("Item %s not created, name not valid", layout_element.name(), name.c_str());
					}
				}
				//IMAGE
				else if (!strcmp(layout_element.name(), "img"))
				{
					std::string preset = layout_element.attribute("type").as_string();
					std::map<std::string, GUIElement*>::iterator node = guiPresets.find(preset);
					std::string name = layout_element.attribute("name").as_string("");
					GUIElement* tmpElement = FindElement(guiList, name);
					// If name is "" or another element with same name exists does not load the element, 
					// same if not preset found in presets list
					if (node != guiPresets.end() && tmpElement == nullptr && name != "")
					{
						GUIImage* img = CreateImageFromPreset({ 0,0,0,0 }, preset, name);
						img->Deserialize(layout_element);
						guiList.push_back(img);
						LOG("Item %s created", layout_element.name());
					}
					else if (tmpElement != nullptr)
					{
						LOG("Item %s not created, another element exists with same name, (%s)", layout_element.name(), name.c_str());
					}
					else if (node == guiPresets.end())
					{
						LOG("Item %s not created, no preset found for %s", layout_element.name(), name.c_str());
					}
					else if (name == "")
					{
						LOG("Item %s not created, name not valid", layout_element.name(), name.c_str());
					}

				}
				//LABEL
				else if (!strcmp(layout_element.name(), "label"))
				{
					std::string name = layout_element.attribute("name").as_string("");
					GUIElement* tmpElement = FindElement(guiList, name);
					if (tmpElement == nullptr && name != "")
					{
						GUILabel* lb = CreateLabel({ 0,0,0,0 }, DEFAULT, name, "");
						lb->Deserialize(layout_element);
						guiList.push_back(lb);
						LOG("Item %s created", layout_element.name());
					}
					else if (tmpElement != nullptr)
					{
						LOG("Item %s not created, another element exists with same name, (%s)", layout_element.name(), name.c_str());
					}
					else if (name == "")
					{
						LOG("Item %s not created, name not valid", layout_element.name(), name.c_str());
					}
				}
				//Newer types of GUI elements to load go here
				layout_element = next;
			}

			data.reset();
			if (ret == true)
			{
				LOG("... ui loaded with succes.");
			}
			else
			{
				LOG("ERROR: ...loading ui process had an error.");
			}

		}
		else
		{
			LOG("Could not parse ui xml.");
		}
	}
	else
	{
		LOG("Could not load ui.xml");
	}
	return ret;
}
//TODO: SaveLayout needs lots of improvements...
bool j1GUI::SaveLayout()
{
	bool ret = true;

	char* buffer = nullptr;
	LOG("Loading UI xml to serialize");
	uint size;
	if (guiLoadedPath != "")
	{
		size = App->fs->Load(guiLoadedPath.c_str(), &buffer);
	}
	else
	{
		size = App->fs->Load("gui/gui.xml", &buffer);
	}

	LOG("Loading UI before serialize.");
	pugi::xml_document data;
	pugi::xml_node root;
	pugi::xml_parse_result result = data.load_buffer(buffer, size);
	RELEASE(buffer);
	if (result != NULL)
	{
		root = data.child("gui").child("layout");

		for (pugi::xml_node child = root.first_child(); child; )
		{
			pugi::xml_node next = child.next_sibling();

			child.parent().remove_child(child);

			child = next;
		}
		std::list<std::string> names;
		std::list<std::string>::iterator name_found;
		for (std::list<GUIElement*>::iterator it = guiList.begin(); it != guiList.end(); it++)
		{
			name_found = std::find(names.begin(), names.end(), (*it)->GetName());
			if (name_found == names.end())
			{
				names.push_back((*it)->GetName());

				(*it)->Serialize(root);
			}
		}

		std::stringstream stream;
		data.save(stream);

		App->fs->Save("gui/gui.xml", stream.str().c_str(), stream.str().length());
		LOG("... just saved the gui: gui.xml");
	}

	//pugi::xml_document data;
	//pugi::xml_node root = data.append_child("layout");


	return ret;
}
bool j1GUI::UpdateGuiList()
{
	return true;
}
bool j1GUI::UpdateDebugGuiList()
{
	return true;
}
bool j1GUI::UpdateEditorGuiList()
{
	return false;
}
//Checks if cursor is inside an element | returns null if nothing found
GUIElement * j1GUI::FindMouseHover()
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
void j1GUI::ManageEvents()
{
	GUIElement* newMouseHover = nullptr;

	//Find the element that is hovered actually
	newMouseHover = FindMouseHover();
	if (focus != nullptr && newMouseHover == nullptr && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
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
	// TODO find wich element has the focus
	// TODO manage the input & events
	if (mouseHover != nullptr && mouseHover->GetCanFocus() == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
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
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		BroadcastEventToListeners(mouseHover, MOUSE_LCLICK_UP);
		mouseHover->SetLClicked(false);
	}
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
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
	if (focus != nullptr && mouseHover != nullptr && mouseHover->GetCanFocus() == true && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		BroadcastEventToListeners(mouseHover, MOUSE_RCLICK_UP);
		mouseHover->SetRClicked(false);
	}
}
//Broadcast an event to all GUIElement listeners
void j1GUI::BroadcastEventToListeners(GUIElement * element, gui_events _event)
{
	if (!GetUIEditing())
	{
		int64_t event = _event;
		element->OnGuiEvent(event);

		if (element->GetElementStatus().interactive && element->GetElementStatus().active)
		{
			//if (event != MOUSE_ENTERS)
			SDL_Log("Event: %b", event);
			switch (event)
			{
			case MOUSE_ENTERS:
				event |= element->GetElementStatus().onMouseEnters;
				break;
			case MOUSE_LEAVES:
				event |= element->GetElementStatus().onMouseLeaves;
				break;
			case MOUSE_LCLICK_DOWN:
				event |= element->GetElementStatus().onLClickDown;
				break;
			case MOUSE_LCLICK_UP:
				event |= element->GetElementStatus().onLClickUp;
				break;
			case MOUSE_RCLICK_DOWN:
				event |= element->GetElementStatus().onRClickDown;
				break;
			case MOUSE_RCLICK_UP:
				event |= element->GetElementStatus().onRClickUp;
				break;
			case GAIN_FOCUS:
				event |= element->GetElementStatus().onGainFocus;
				break;
			case LOST_FOUCS:
				event |= element->GetElementStatus().onLooseFocus;
				break;
			default:
				break;
			}
			//First we get listeners list of previous element hovered
			std::list<j1Module*> tmpListeners = element->GetListeners();
			//Iterate over listeners list to send them hover is lost
			for (std::list<j1Module*>::iterator it = tmpListeners.begin(); it != tmpListeners.end(); it++)
			{
				(*it)->GuiEvent(element, event);
			}
		}
	}
}
void j1GUI::Draw()
{
	//IterateList(&guiList, &M_GUI::DoElementDraw);
	for (std::list<GUIElement*>::iterator it = background.begin(); it != background.end(); it++)
	{
		if ((*it)->GetActive())
			(*it)->Draw();
	}
	for (std::list<GUIElement*>::iterator it = guiList.begin(); it != guiList.end(); it++)
	{
		if ((*it)->GetActive())
			(*it)->Draw();
	}
}
void j1GUI::DrawEditor()
{
	for (std::list<GUIElement*>::iterator it = editorGuiList.begin(); it != editorGuiList.end(); it++)
	{
		if ((*it)->GetElementStatus().active) // Do update only if element is active
			(*it)->Draw();
	}
}
void j1GUI::DrawDebug()
{
	int x;
	int y;
	App->input->GetMousePosition(x, y);
	xMouse->SetText(std::to_string(x).c_str(), SMALL);
	yMouse->SetText(std::to_string(y).c_str(), SMALL);
	xMouse->SetGlobalPos(x + 10, y);
	yMouse->SetGlobalPos(x + 40, y);

	for (std::list<GUIElement*>::iterator it = guiList.begin(); it != guiList.end(); it++)
	{
		if ((*it)->GetActive())
		{
			GB_Rectangle<float> rect = (*it)->GetDrawRect();
			App->render->DrawQuad({ rect.x, rect.y, rect.w, rect.h }, 0, 255, 0, 255, false, false);
		}
	}
	//IterateList(&debugGuiList, &M_GUI::DoElementDraw);
	for (std::list<GUIElement*>::iterator it = debugGuiList.begin(); it != debugGuiList.end(); it++)
	{
		if ((*it)->GetActive())
			(*it)->Draw();
	}
}
GUIElement * j1GUI::FindElement(std::list<GUIElement*> list, std::string name)
{
	GUIElement* ret = nullptr;

	for (std::list<GUIElement*>::iterator i = list.begin(); i != list.end(); i++)
	{
		if (strcmp((*i)->GetName().c_str(), name.c_str()) == 0)
		{
			ret = (*i);
			break;
		}
	}
	return ret;
}
bool j1GUI::FindElement(std::list<GUIElement*> list, GUIElement * element)
{
	bool ret = false;

	for (std::list<GUIElement*>::iterator i = list.begin(); i != list.end(); i++)
	{
		if ((*i) == element)
		{
			ret = true;
			break;
		}
	}
	return ret;
}
SDL_Texture* j1GUI::GetAtlas() const
{
	return atlas;
}
void j1GUI::SetAtlas(SDL_Texture * texture)
{
	atlas = texture;
}
//Experimental methods, I just did this because of yes...
/*void M_GUI::IterateList(std::list<GUIElement*>* list, void (M_GUI::*method)(GUIElement*))
{
for (std::list<GUIElement*>::iterator it = (*list).begin(); it != (*list).end(); it++)
{
(*this.*method)(*it);
}
}*/
/*void M_GUI::DoElementUpdate(GUIElement * element, float dt)
{
if (element->GetElementStatus().active)
element->Update(mouseHover, focus, dt);

}*/
/*void M_GUI::DoElementDraw(GUIElement * element)
{
if (element->GetElementStatus().active)
element->Draw();
}*/
//Experimental methods, I just did this because of yes...

GUIButton * j1GUI::CreateButton(GB_Rectangle<int> _position, GB_Rectangle<int> _standBySection,
	GB_Rectangle<int> _hoverSection, GB_Rectangle<int> _clickedSection,
	std::string name)
{
	GUIButton* button = new GUIButton(_position, _standBySection, _hoverSection, _clickedSection, name);
	return button;
}
GUIButton * j1GUI::CreateButtonFromPreset(GB_Rectangle<int> _position, std::string preset, std::string name, const char* _text)
{
	GUIButton* ret;

	GUIElement* el = FindElement(guiList, name);
	if (el == nullptr)
		el = FindElement(debugGuiList, name);

	if (el == nullptr)
	{
		GUIButton* btn_preset = (GUIButton*)guiPresets.find(preset)->second;
		GUIButton* btn = new GUIButton(*btn_preset, name);
		btn->SetPresetType(preset);
		btn->SetRectangle(_position);
		btn->image->SetRectangle(_position);
		//btn->label->SetRectangle(_position);
		btn->SetText(_text);
		btn->label->Center();
		ret = btn;
	}
	else
	{
		ret = (GUIButton*)el;
	}
	return ret;
}
GUILabel * j1GUI::CreateLabel(GB_Rectangle<int> _position, label_size _size, std::string name, const char* _text)
{
	GUILabel* ret = nullptr;

	GUIElement* el = FindElement(guiList, name);
	if (el == nullptr)
		el = FindElement(debugGuiList, name);

	if (el == nullptr)
	{
		if (_text != nullptr)
		{
			ret = new GUILabel(_text, _size, name);
		}
		else
		{
			ret = new GUILabel(name);
		}
		ret->SetGlobalPos(_position.x, _position.y);
	}
	else
	{
		ret = (GUILabel*)el;
		if (_text != nullptr)
			ret->SetText(_text, _size);
	}
	return ret;
}
GUIImage * j1GUI::CreateImage(GB_Rectangle<int> _position, GB_Rectangle<int> _section, std::string name)
{
	GUIImage* image = new GUIImage(name);
	image->SetSection(_section);
	image->SetRectangle(_position);

	return image;
}
GUIImage * j1GUI::CreateImageFromPreset(GB_Rectangle<int> _position, std::string preset, std::string name)
{
	GUIImage* ret;

	GUIElement* el = FindElement(guiList, name);
	if (el == nullptr)
		el = FindElement(debugGuiList, name);

	if (el == nullptr)
	{
		GUIImage* img_preset = (GUIImage*)guiPresets.find(preset)->second;
		GUIImage* img = new GUIImage(*img_preset, name);
		img->SetPresetType(preset);
		img->SetRectangle(_position);
		ret = img;
	}
	else
	{
		ret = (GUIImage*)el;
	}


	return ret;
}

GUIElement * j1GUI::GuiFactory()
{





	return nullptr;
}
bool j1GUI::GetUIEditing() const
{
	return UIEditing;
}
void j1GUI::SetUIEditing(bool edit)
{
	UIEditing = edit;
	if (edit)
	{
		for (std::list<GUIElement*>::iterator it = guiList.begin();
			it != guiList.end(); it++)
		{
			(*it)->SetDraggable(true);
		}
	}
	else
	{
		for (std::list<GUIElement*>::iterator it = guiList.begin();
			it != guiList.end(); it++)
		{
			(*it)->SetDraggable(false);
		}
	}
}
