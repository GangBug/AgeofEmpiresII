#include "GUILabel.h"
#include "GUIElement.h"
#include "M_Textures.h"
#include "M_Fonts.h"
#include "M_GUI.h"
#include "App.h"
#include "M_Resources.h"
#include "M_MissionManager.h"
#include "M_Metrics.h"
//TEMP
#include "M_Render.h"

GUILabel::GUILabel(std::string name, int flags, bool resize) : GUIElement(name, flags)
{
	SetType(GUI_LABEL);
	texture = nullptr;
	this->resize = resize;
}
GUILabel::GUILabel(const char * text, label_size _size, std::string name, bool resize, int flags, SDL_Color color) : GUIElement(name, flags)
{
	SetColor(color);
	SetText(text, _size);
	SetType(gui_types::GUI_LABEL);
	lbSize = _size;	
	this->resize = resize;
}

void GUILabel::OnUpdate(const GUIElement * mouseHover, const GUIElement * focus, float dt)
{
	if (strcmp(GetName().c_str(), "label_gold") == 0)
		SetText(std::to_string(app->resources->GetCurrentGold()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_food") == 0)
		SetText(std::to_string(app->resources->GetCurrentFood()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_wood") == 0)
		SetText(std::to_string(app->resources->GetCurrentWood()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_MaxVillager_n") == 0)
		SetText(std::to_string(app->resources->GetTotalVillagers()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "label_MaxPopulation_n") == 0)
		SetText(std::to_string(app->resources->GetTotalUnits()).c_str(), SMALL);

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

	//MAX UNIT
	if (strcmp(GetName().c_str(), "Total_Archer_Alive_InGame_n") == 0)
		SetText(std::to_string(app->metrics->GetArchersAlive()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "Total_Samurai_Alive_InGame_n") == 0)
		SetText(std::to_string(app->metrics->GetSamuraisAlive()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "Total_Tarkan_Alive_InGame_n") == 0)
		SetText(std::to_string(app->metrics->GetTarkanAlive()).c_str(), SMALL);

	//MISSION GOAL
	if (strcmp(GetName().c_str(), "label_Mision_One") == 0) // GO TOWN
	{
		SetText(app->missionManager->GetObjectiveState1().c_str(), SMALL);
		if (app->missionManager->GetState() == M_TOWNATTACK)
			SetColor({ 255,0,0 ,255});
		if (app->missionManager->GetState() == M_TOWNREPAIR)
			color = { 255,0,0};
		if (app->missionManager->GetState() == M_DEFEAT)
			SetColor(SDL_Color{ 255,0,0 ,255 });
	}
	if (strcmp(GetName().c_str(), "label_Mision_Two") == 0)// REPAIR TOWN
	{ 
		SetText(app->missionManager->GetObjectiveState2().c_str(), SMALL);

		if (app->missionManager->GetState() == M_WAVES)
			SetColor(SDL_Color{ 0,255,0,255 });
		if (app->missionManager->GetState() == M_DEFEAT)
			SetColor(SDL_Color{ 255,0,0,255 });
	}
	if (strcmp(GetName().c_str(), "label_Mision_Tree") == 0)//KILL WAVES
	{
		SetText(app->missionManager->GetObjectiveState3().c_str(), SMALL);

		if (app->missionManager->GetState() == M_BOSS)
			SetColor(SDL_Color{ 0,255,0 ,255 });
		if (app->missionManager->GetState() == M_DEFEAT)
			SetColor(SDL_Color{ 255,0,0,255 });
	}
	if (strcmp(GetName().c_str(), "label_Mision_Four") == 0) //KILL BOSS
	{
		SetText(app->missionManager->GetObjectiveState4().c_str(), SMALL);

		if (app->missionManager->GetState() == M_VICTORY)
			SetColor(SDL_Color{ 0,255,0,255 });
		if (app->missionManager->GetState() == M_DEFEAT)
			SetColor(SDL_Color{ 255,0,0,255 });
	}


	//TEMP DEBUG
	//if (strcmp(GetName().c_str(), "label_Wave_time_n") == 0)
	//{
	//	char timeText[64];
	//	sprintf_s(timeText, 64, "%.2f s", app->missionManager->GetMisionTimeleftf()); //Formating time to 2 decimals
	//	//sprintf_s(timeText, 64, "%d s", (int)app->missionManager->GetMisionTimeleftf()); //Formating time to int with 0 decimals. Ms are important?? TODO: Choose between displaying time with decimals or not.
	//	SetText(timeText, SMALL);

	//	//Deprecated... should delete this line
	//	//SetText(std::to_string(app->missionManager->GetMisionTimeleftf()).c_str(), SMALL);
	//}
	//if (strcmp(GetName().c_str(), "label_EnemyDead_stat_n") == 0)
	//	SetText(std::to_string(app->missionManager->GetEnemyDeadUnits()).c_str(), SMALL);

	//SCORE
	if (strcmp(GetName().c_str(), "Total_Score_n") == 0)
		SetText(std::to_string(app->metrics->GetFinalScore()).c_str(), MEDIUM);
	if (strcmp(GetName().c_str(), "Total_Time_n") == 0)
	{
		char timeText[64];
		sprintf_s(timeText, 64, "%.2f s", app->metrics->GetTotalTime()); //Formating time to 2 decimals
		//sprintf_s(timeText, 64, "%d s", (int)app->metrics->GetTotalTime()); //Formating time to int with 0 decimals. Ms are important?? TODO: Choose between displaying time with decimals or not.
		SetText(timeText, SMALL);

		//Deprecated... should delete this line
		//SetText(std::to_string(app->metrics->GetTotalTime()).c_str(), MEDIUM);
	}

	if (strcmp(GetName().c_str(), "Total_Kills_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalkills()).c_str(), MEDIUM);
	if (strcmp(GetName().c_str(), "Total_Viles_killed_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalVilesKilled()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "Total_Witchs_killed_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalWitchKilled()).c_str(), SMALL);

	if (strcmp(GetName().c_str(), "Total_KillsTotal_Resources_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalResources()).c_str(), MEDIUM);
	if (strcmp(GetName().c_str(), "Total_Gold_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalGold()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "Total_Wood_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalWood()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "Total_Food_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalFood()).c_str(), SMALL);


	if (strcmp(GetName().c_str(), "Total_Units_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalUnits()).c_str(), MEDIUM);
	if (strcmp(GetName().c_str(), "Total_Units_Alive_n") == 0)
		SetText(std::to_string(app->metrics->GetTotalUnitsAlive()).c_str(), MEDIUM);
	if (strcmp(GetName().c_str(), "Total_Samurais_Alive_n") == 0)
		SetText(std::to_string(app->metrics->GetSamuraisAlive()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "Total_Tarkan_Alive_n") == 0)
		SetText(std::to_string(app->metrics->GetTarkanAlive()).c_str(), SMALL);
	if (strcmp(GetName().c_str(), "Total_Archer_Alive_n") == 0)
		SetText(std::to_string(app->metrics->GetArchersAlive()).c_str(), SMALL);

}

GUILabel::~GUILabel()
{
	if (texture != nullptr && texture != app->gui->GetAtlas())
		app->tex->UnLoad(texture);
}

void GUILabel::SetText(const char* text, label_size _size)
{
	//FIX: This may be the cause that crashes the game whenever you close it.
	if (resize)
	{
		if (text != this->text)
		{
			if (texture != nullptr)
				SDL_DestroyTexture(texture);

			this->text = text;
			texture = app->font->Print(text, app->font->WOW, color);

			int w, h;
			app->tex->GetSize(texture, (uint&)w, (uint&)h);
			SetSize(w, h);
			lbSize = _size;
		}
	}
	else
	{
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

	
}

SDL_Texture * GUILabel::GetTexture() const
{
	return texture;
}

void GUILabel::Draw() const
{
	if (resize)
	{
		if (GetVisible())
			if (texture != nullptr)
			{
				GB_Rectangle<float> rect;
				GB_Rectangle<float> parentRect;
				if (GetParent())
				{
					parentRect = GetParent()->GetDrawRect();
					rect = parentRect;
				}
				else
				{
					rect = GetDrawRect();
				}
				GB_Rectangle<float> sect;
				rect.x -= app->render->camera->GetRect().x;
				rect.y -= app->render->camera->GetRect().y;
				SDL_QueryTexture(texture, nullptr, nullptr, &sect.w, &sect.h);
				rect.h = sect.h;
				rect.h *= rect.w;
				rect.h /= sect.w;
				if (GetParent())
				{
					if (rect.h > parentRect.h)
					{
						rect.h = GetParent()->GetDrawRect().h;

						rect.w *= rect.h;
						rect.w /= rect.h;
					}
				}
				//app->render->Blit(texture, rect.x, rect.y, NULL, 0.0f);
				if (followScreen)
					app->render->Blit(texture, &rect.GetSDLrect(), &sect.GetSDLrect());
				else
					app->render->Blit(texture, &GetDrawRect().GetSDLrect(), &sect.GetSDLrect());
			}

	}
	else
	{
		if (GetVisible())
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
				if (followScreen)
					app->render->Blit(texture, &rect.GetSDLrect(), &sect.GetSDLrect());
				else
					app->render->Blit(texture, &GetDrawRect().GetSDLrect(), &sect.GetSDLrect());
			}
	}




	/*
	//LabelDraw
	if (GetVisible())
		if (label->GetTexture() != nullptr)
		{
			SDL_Texture* tex = label->GetTexture();

			GB_Rectangle<float> rect;
			rect.x = (float)this->rect.x;
			GB_Rectangle<float> sect;
			rect.x -= app->render->camera->GetRect().x;
			rect.y -= app->render->camera->GetRect().y;
			SDL_QueryTexture(tex, nullptr, nullptr, &sect.w, &sect.h);
			rect.h = (sect.h * rect.w) / sect.w;
			//app->render->Blit(texture, rect.x, rect.y, NULL, 0.0f);
			SDL_Log("%d", rect.x);
			app->render->Blit(tex, &rect.GetSDLrect(), &sect.GetSDLrect());
		}


*/

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
