#include "GUILabel.h"
#include "GUIElement.h"
#include "M_Textures.h"
#include "M_Fonts.h"
#include "M_GUI.h"
#include "App.h"

//TEMP
#include "M_Render.h"

GUILabel::GUILabel(std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_LABEL);
	texture = nullptr;
}
GUILabel::GUILabel(const char * text, label_size _size, std::string name, int flags) : GUIElement(name, flags)
{
	SetText(text, _size);
	SetType(gui_types::GUI_LABEL);
	lbSize = _size;
}

GUILabel::~GUILabel()
{
	if (texture != nullptr && texture != app->gui->GetAtlas())
		app->tex->UnLoad(texture);
}

void GUILabel::SetText(const char* text, label_size _size)
{
	if (texture != nullptr)
		SDL_DestroyTexture(texture);

	this->text = text;
	switch (_size)
	{
		case DEFAULT:
			texture = app->font->Print(text, app->font->defaultFont);
			break;
		case MEDIUM:
			texture = app->font->Print(text, app->font->mediumFont);
			break;
		case SMALL:
			texture = app->font->Print(text, app->font->smallFont);
			break;
		default:
			break;
	}

	int w, h;
	app->tex->GetSize(texture, (uint&)w, (uint&)h);
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
		app->render->Blit(texture, rect.x, rect.y, NULL, 0.0f);
		//app->render->Blit(texture, &GetDrawRect().GetSDLrect(), &sect.GetSDLrect());
	}
}

void GUILabel::Serialize(pugi::xml_node root)
{
	pugi::xml_attribute atr;
	pugi::xml_node position;
	pugi::xml_node size;
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
	//Create node label/position
	position = element.append_child("position");
	//Create atributes in label/position
	atr = position.append_attribute("x");
	atr.set_value(xmlRect.x);
	atr = position.append_attribute("y");
	atr.set_value(xmlRect.y);

}

void GUILabel::Deserialize(pugi::xml_node layout_element)
{
	std::string txt = layout_element.attribute("text").as_string();
	label_size size = (label_size)layout_element.attribute("size").as_int();
	SetText(txt.c_str(), size);
	GB_Rectangle<float> xmlRect;
	GB_Rectangle<int>	rect;
	xmlRect.x = layout_element.child("position").attribute("x").as_float();
	xmlRect.y = layout_element.child("position").attribute("y").as_float();
	rect = app->gui->XmlToScreen(xmlRect);

	SetGlobalPos(rect.x, rect.y);


}
