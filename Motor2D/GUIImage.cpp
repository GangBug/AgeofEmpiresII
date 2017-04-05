#include "GUIImage.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "p2Log.h"

GUIImage::GUIImage(std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_IMAGE);
	atlas = App->gui->GetAtlas();
 
	//texture = nullptr;
}
GUIImage::GUIImage(const GUIImage & img, std::string name, int flags) : GUIElement(name, flags)
{
	SetType(GUI_IMAGE);
	atlas = App->gui->GetAtlas();
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
	GB_Rectangle<float> rect = GetDrawRect();
	rect.x -= App->render->camera->GetPosition().x;
	rect.y -= App->render->camera->GetPosition().y;
	App->render->Blit(atlas, &rect.GetSDLrect(), &GetSection().GetSDLrect());
}
void GUIImage::Serialize(pugi::xml_node root)
{
	pugi::xml_attribute atr;
	pugi::xml_node position;
	pugi::xml_node size;
	pugi::xml_node element;

	element = root.append_child("img");
	//Create atributes in img
	atr = element.append_attribute("type");
	atr.set_value(GetPresetType().c_str());
	atr = element.append_attribute("name");
	atr.set_value(GetName().c_str());
	//Create node img/position
	position = element.append_child("position");
	//Create atributes in img/position
	atr = position.append_attribute("x");
	atr.set_value(GetLocalRect().x*App->gui->GetScaleX());
	atr = position.append_attribute("y");
	atr.set_value(GetLocalRect().y*App->gui->GetScaleY());
	//Create node img/size
	size = element.append_child("size");
	//Create atributes in img/size
	atr = size.append_attribute("w");
	atr.set_value(GetLocalRect().w*App->gui->GetScaleX());
	atr = size.append_attribute("h");
	atr.set_value(GetLocalRect().h*App->gui->GetScaleY());

}
void GUIImage::Deserialize(pugi::xml_node layout_element)
{
	std::string name = layout_element.attribute("name").as_string();
	GB_Rectangle<int> rect;
	rect.x = layout_element.child("position").attribute("x").as_int()*App->gui->GetScaleX();
	rect.y = layout_element.child("position").attribute("y").as_int()*App->gui->GetScaleY();
	rect.w = layout_element.child("size").attribute("w").as_int()*App->gui->GetScaleX();
	rect.h = layout_element.child("size").attribute("h").as_int()*App->gui->GetScaleY();
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

void GUIImage::SetAtlas(SDL_Texture * _atlas)
{
	atlas = _atlas;
}
