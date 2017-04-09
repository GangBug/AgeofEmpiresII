#include "GUIImage.h"
#include "M_Render.h"
#include "M_GUI.h"

GUIImage::GUIImage(std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_IMAGE);
	atlas = app->gui->GetAtlas();
	//texture = nullptr;
}
GUIImage::GUIImage(const GUIImage & img, std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_IMAGE);
	atlas = app->gui->GetAtlas();
	SetRectangle(img.GetLocalRect());
	section = img.section;
}
GUIImage::~GUIImage()
{
}

void GUIImage::OnUpdate(const GUIElement * mouseHover, const GUIElement * focus, float dt)
{
}
void GUIImage::Draw() const
{
	if(GetVisible())
	{
		GB_Rectangle<float> rect = GetDrawRect();
		rect.x -= app->render->camera->GetRect().x;
		rect.y -= app->render->camera->GetRect().y;
		app->render->Blit(atlas, &GetDrawRect().GetSDLrect(), &GetSection().GetSDLrect(), alpha);
	}
}
void GUIImage::Serialize(pugi::xml_node root)
{
	pugi::xml_attribute atr;
	pugi::xml_node position;
	pugi::xml_node size;
	pugi::xml_node element;
	pugi::xml_node n_scenes;
	pugi::xml_node n_scene;

	GB_Rectangle<float> xmlRect = app->gui->ScreenToXml(GetLocalRect());

	element = root.append_child("img");
	//Create atributes in img
	atr = element.append_attribute("type");
	atr.set_value(GetPresetType().c_str());
	atr = element.append_attribute("name");
	atr.set_value(GetName().c_str());
	//atr = element.append_attribute("draggable");
	//atr.set_value(GetDraggable());
	atr = element.append_attribute("interactive");
	atr.set_value(GetInteractive());
	atr = element.append_attribute("canFocus");
	atr.set_value(GetCanFocus());
	atr = element.append_attribute("active");
	atr.set_value(GetActive());
	atr = element.append_attribute("visible");
	atr.set_value(GetVisible());

	//Create Node Scenes
	n_scenes = element.append_child("scenes");
	for (std::map<std::string, Module*>::iterator it = scenes.begin(); it != scenes.end(); it++)
	{
		n_scene = n_scenes.append_child("scene");
		atr = n_scene.append_attribute("name");
		atr.set_value((it->first.c_str()));
	}
	//Create node img/position
	position = element.append_child("position");
	//Create atributes in img/position
	atr = position.append_attribute("x");
	atr.set_value(xmlRect.x);
	atr = position.append_attribute("y");
	atr.set_value(xmlRect.y);
	//Create node img/size
	size = element.append_child("size");
	//Create atributes in img/size
	atr = size.append_attribute("w");
	atr.set_value(xmlRect.w);
	atr = size.append_attribute("h");
	atr.set_value(xmlRect.h);

}
void GUIImage::Deserialize(pugi::xml_node layout_element)
{
	std::string name = layout_element.attribute("name").as_string();
	SetActive(layout_element.attribute("active").as_bool(false));
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

	GB_Rectangle<float> xmlRect;
	GB_Rectangle<int>	rect;
	xmlRect.x = layout_element.child("position").attribute("x").as_float();
	xmlRect.y = layout_element.child("position").attribute("y").as_float();
	xmlRect.w = layout_element.child("size").attribute("w").as_float();
	xmlRect.h = layout_element.child("size").attribute("h").as_float();

	rect = app->gui->XmlToScreen(xmlRect);
	SetRectangle(rect);


}
GB_Rectangle<int> GUIImage::GetSection() const
{
	return section;
}
void GUIImage::SetSection(GB_Rectangle<int> _section)
{
	section = _section;
}
void GUIImage::SetSection(int _sectionX, int _sectionY, int _sectionW, int _sectionH)
{
	section.x = _sectionX;
	section.y = _sectionY;
	section.w = _sectionW;
	section.h = _sectionH;
}
