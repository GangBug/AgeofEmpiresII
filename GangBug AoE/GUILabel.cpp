#include "GUILabel.h"
#include "GUIElement.h"
#include "M_Textures.h"
#include "M_Fonts.h"
#include "M_GUI.h"
#include "App.h"
#include "M_Resources.h"
#include "M_MisionManager.h"
//TEMP
#include "M_Render.h"

GUILabel::GUILabel(std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_LABEL);
	texture = nullptr;
}
GUILabel::GUILabel(const char * text, label_size _size, std::string name, int flags, SDL_Color color) : GUIElement(name, flags)
{
	SetColor(color);
	SetText(text, _size);
	SetType(gui_types::GUI_LABEL);
	lbSize = _size;	
}

void GUILabel::OnUpdate(const GUIElement * mouseHover, const GUIElement * focus, float dt)
{
	if (strcmp(GetName().c_str(), "label_gold") == 0)
		SetText(std::to_string(app->resources->GetCurrentGold()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_food") == 0)
		SetText(std::to_string(app->resources->GetCurrentFood()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_wood") == 0)
		SetText(std::to_string(app->resources->GetCurrentWood()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_MaxVillager") == 0)
		SetText(std::to_string(app->resources->GetTotalVillagers()).c_str(), SMALL);

	if (strcmp(GetName().c_str(), "label_ResourceWindow_TotalVillager_n") == 0)
		SetText(std::to_string(app->resources->GetTotalVillagers()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_ResourceWindow_UnployedVillager_n") == 0)
		SetText(std::to_string(app->resources->GetVillagers()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_ResourceWindow_Miners_n") == 0)
		SetText(std::to_string(app->resources->GetMiners()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_ResourceWindow_Farmers_n") == 0)
		SetText(std::to_string(app->resources->GetFarmers()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_ResourceWindow_Lumberjacks_n") == 0)
		SetText(std::to_string(app->resources->GetLumberjacks()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_ResourceWindow_Repairmen_n") == 0)
		SetText(std::to_string(app->resources->GetConstructors()).c_str(), SMALL);
	

	if (strcmp(GetName().c_str(), "label_Wave_stat_n") == 0)
		SetText(app->misionManager->GetStateName().c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_Wave_time_n") == 0)
		SetText(std::to_string(app->misionManager->GetMisionTimeleftf()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_EnemyDead_stat_n") == 0)
		SetText(std::to_string(app->misionManager->GetEnemyDeadUnits()).c_str(), SMALL);



	
}

GUILabel::~GUILabel()
{
	if (texture != nullptr && texture != app->gui->GetAtlas())
		app->tex->UnLoad(texture);
}

void GUILabel::SetText(const char* text, label_size _size)
{
	//FIX: This may be the cause that crashes the game whenever you close it.
	if (text != this->text)
	{
		if (texture != nullptr)
			SDL_DestroyTexture(texture);

		this->text = text;
		switch (_size)
		{
		case DEFAULT:
			texture = app->font->Print(text, app->font->defaultFont, color);
			break;
		case MEDIUM:
			texture = app->font->Print(text, app->font->mediumFont, color);
			break;
		case SMALL:
			texture = app->font->Print(text, app->font->smallFont, color);
			break;
		default:
			break;
		}

		int w, h;
		app->tex->GetSize(texture, (uint&)w, (uint&)h);
		SetSize(w, h);
		lbSize = _size;
	}
}

const SDL_Texture * GUILabel::GetTexture() const
{
	return texture;
}

void GUILabel::Draw() const
{
	if(GetVisible())
		if (texture != nullptr)
		{
			GB_Rectangle<float> rect = GetDrawRect();
			GB_Rectangle<float> sect;
			rect.x -= app->render->camera->GetRect().x;
			rect.y -= app->render->camera->GetRect().y;
			sect.w = rect.w;
			sect.h = rect.h;
			SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
			//app->render->Blit(texture, rect.x, rect.y, NULL, 0.0f);
			if(followScreen)
				app->render->Blit(texture, &rect.GetSDLrect(), &sect.GetSDLrect());
			else
				app->render->Blit(texture, &GetDrawRect().GetSDLrect(), &sect.GetSDLrect());
		}
}

void GUILabel::Serialize(pugi::xml_node root)
{
	pugi::xml_attribute atr;
	pugi::xml_node node;
	pugi::xml_node element;

	GB_Rectangle<float> xmlRect = app->gui->ScreenToXml(GetLocalRect());

	element = root.append_child("label");
	//Create atributes in label
	atr = element.append_attribute("size");
	atr.set_value(GetLabelSize());
	atr = element.append_attribute("name");
	atr.set_value(GetName().c_str());
	atr = element.append_attribute("text");
	atr.set_value(GetText().c_str());
	//atr = element.append_attribute("draggable");
	//atr.set_value(GetDraggable());
	atr = element.append_attribute("interactive");
	atr.set_value(GetInteractive());
	atr = element.append_attribute("canFocus");
	atr.set_value(GetCanFocus());
	atr = element.append_attribute("visible");
	atr.set_value(GetVisible());

	//Create Node Scenes
	pugi::xml_node n_listeners = element.append_child("scenes");
	for (std::map<std::string, Module*>::iterator it = scenes.begin(); it != scenes.end(); ++it)
	{
		node = n_listeners.append_child("scene");
		atr = node.append_attribute("name");
		atr.set_value((it->first.c_str()));
	}
	//Create Node Color
	node = element.append_child("color");
	//Create atributes in label/color
	atr = node.append_attribute("r");
	atr.set_value(color.r);
	atr = node.append_attribute("g");
	atr.set_value(color.g);
	atr = node.append_attribute("b");
	atr.set_value(color.b);
	atr = node.append_attribute("a");
	atr.set_value(color.a);


	//Create node label/position
	node = element.append_child("position");
	//Create atributes in label/position
	atr = node.append_attribute("x");
	atr.set_value(xmlRect.x);
	atr = node.append_attribute("y");
	atr.set_value(xmlRect.y);

}
void GUILabel::Deserialize(pugi::xml_node layout_element)
{
	std::string txt = layout_element.attribute("text").as_string();
	label_size size = (label_size)layout_element.attribute("size").as_int();
	//SetActive(layout_element.attribute("active").as_bool(false));
	SetActive(false);
	//SetDraggable(layout_element.attribute("draggable").as_bool(false));
	SetInteractive(layout_element.attribute("interactive").as_bool(false));
	SetCanFocus(layout_element.attribute("canFocus").as_bool(false));
	SetVisible(layout_element.attribute("visible").as_bool(false));
	
	//Load scenes
	for (pugi::xml_node it = layout_element.child("scenes").first_child(); it; )
	{
		pugi::xml_node next = it.next_sibling();
		if (Module* module = app->FindModule(it.attribute("name").as_string("")))
		{
			AddScene(module);
		}
		it = next;
	}

	SetText(txt.c_str(), size);
	GB_Rectangle<float> xmlRect;
	GB_Rectangle<int>	rect;
	xmlRect.x = layout_element.child("position").attribute("x").as_float();
	xmlRect.y = layout_element.child("position").attribute("y").as_float();
	rect = app->gui->XmlToScreen(xmlRect);

	SetGlobalPos(rect.x, rect.y);
	color.r = layout_element.child("color").attribute("r").as_int(255);
	color.g = layout_element.child("color").attribute("g").as_int(255);
	color.b = layout_element.child("color").attribute("b").as_int(255);
	color.a = layout_element.child("color").attribute("a").as_int(255);
	SetColor(color);


}
