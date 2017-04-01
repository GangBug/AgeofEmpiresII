#include "GUILabel.h"
#include "GUIElement.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1GUI.h"
#include "j1App.h"

//TEMP
#include "j1Render.h"

GUILabel::GUILabel(std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_LABEL);
	texture = nullptr;
	color = { 0,0,0,0 }; //TODO: SEND THE COLOR THROUGH THE XML
}
GUILabel::GUILabel(const char * text, label_size _size, std::string name, int flags) : GUIElement(name, flags)
{
	SetText(text, _size);
	SetType(gui_types::GUI_LABEL);
	lbSize = _size;
}

GUILabel::~GUILabel()
{
	if (texture != nullptr && texture != App->gui->GetAtlas())
		App->tex->UnLoad(texture);
}

void GUILabel::SetText(const char* text, label_size _size)
{
	if (texture != nullptr)
		SDL_DestroyTexture(texture);

	this->text = text;
	switch (_size)
	{
	case DEFAULT:
		texture = App->font->Print(text, App->font->defaultFont, color);
		break;
	case MEDIUM:
		texture = App->font->Print(text, App->font->mediumFont, color);
		break;
	case SMALL:
		texture = App->font->Print(text, App->font->smallFont, color);
		break;
	default:
		break;
	}

	int w, h;
	App->tex->GetSize(texture, (uint&)w, (uint&)h);
	SetSize(w, h);
	lbSize = _size;
}

const SDL_Texture * GUILabel::GetTexture() const
{
	return texture;
}

void GUILabel::Draw() const
{
	if (texture != nullptr)
	{
		GB_Rectangle<float> rect = GetDrawRect();
		GB_Rectangle<float> sect;
		sect.x = 0;
		sect.y = 0;
		sect.w = rect.w;
		sect.h = rect.h;
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		App->render->Blit(texture, rect.x, rect.y, NULL/*, 0.0f*/);
		//app->render->Blit(texture, &GetDrawRect().GetSDLrect(), &sect.GetSDLrect());
	}
}

void GUILabel::Serialize(pugi::xml_node root)
{
	pugi::xml_attribute atr;
	pugi::xml_node position;
	pugi::xml_node size;
	pugi::xml_node element;

	element = root.append_child("label");
	//Create atributes in label
	atr = element.append_attribute("size");
	atr.set_value(GetLabelSize());
	atr = element.append_attribute("name");
	atr.set_value(GetName().c_str());
	atr = element.append_attribute("text");
	atr.set_value(GetText().c_str());
	//Create node label/position
	position = element.append_child("position");
	//Create atributes in label/position
	atr = position.append_attribute("x");
	atr.set_value(GetLocalRect().x*WINDOWSCALEINV);
	atr = position.append_attribute("y");
	atr.set_value(GetLocalRect().y*WINDOWSCALEINV);

}

void GUILabel::Deserialize(pugi::xml_node layout_element)
{
	std::string txt = layout_element.attribute("text").as_string();
	label_size size = (label_size)layout_element.attribute("size").as_int();
	SetText(txt.c_str(), size);
	GB_Rectangle<int> rect;
	rect.x = layout_element.child("position").attribute("x").as_int()*WINDOWSCALE;
	rect.y = layout_element.child("position").attribute("y").as_int()*WINDOWSCALE;
	SetGlobalPos(rect.x, rect.y);


}
