#include "Log.h"
#include "App.h"
#include "M_Window.h"
#include "M_Render.h"

#include "M_EntityManager.h"
#include "Entity.h"
#include "M_Map.h"
#include "M_GUI.h"
#include "M_Animation.h"
#include "S_InGame.h"

//TEMP
#include "M_Textures.h"
#include <algorithm>

#define VSYNC true

M_Render::M_Render(bool startEnabled) : Module(startEnabled)
{
	name.assign("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
M_Render::~M_Render()
{}

// Called before render is available
bool M_Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == nullptr)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera = new Camera(SDL_Rect{ 0, 0, app->win->screenSurface->w, app->win->screenSurface->h });
	}

	return ret;
}

// Called before the first frame
bool M_Render::Start()
{
	LOG("render start");
	//Top right viewport
	if (app->editor == true)
	{
		editorViewPort = { 0, 0, app->win->GetWindowSize().x, app->win->GetWindowSize().y };
		gameViewPort = { 0, 0, app->win->GetWindowSize().x, app->win->GetWindowSize().y };

		gameViewPort.y = 0;
		gameViewPort.w = gameViewPort.w / 1.5f;
		gameViewPort.h = gameViewPort.h / 1.5f;
		gameViewPort.x = (editorViewPort.w - gameViewPort.w)/2;
		game_tex_background = app->tex->Load("gui/TestingTexture.png");
		editor_tex_background = app->tex->Load("gui/TestingTexture_Green.png");
	}
	else
	{
		SDL_RenderGetViewport(renderer, &gameViewPort);
	}
	
	
	//Render texture to screen
	
	// back background
	
	return true;
}

// Called each loop iteration
update_status M_Render::PreUpdate(float dt)
{
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

update_status M_Render::PostUpdate(float dt)
{
	if (app->editor)
	{	
		//Here goes the Editor UI
		SetViewPort(editorViewPort);
		SDL_RenderCopy(renderer, editor_tex_background, NULL, NULL);
		app->gui->DrawEditor();
	}	

	SetViewPort(gameViewPort);
	SDL_RenderCopy(renderer, game_tex_background, NULL, NULL);
	//TODO: Might have a better organitzation to draw map or change map system
	app->map->Draw();

	PerfTimer timer;
	std::vector<Entity*> entitiesVect;
	app->entityManager->Draw(entitiesVect, camera->GetRect());
	double tmp = timer.ReadMs();
	//LOG("Collecting entities lasted %f ms.", tmp);
	DrawEntities(entitiesVect);
	double tmp2 = timer.ReadMs();
	//LOG("Drawing entities lasted: %f ms.", tmp2 - tmp);
	//LOG("Total process lasted: %f ms.", tmp2);

	app->inGame->Draw();

	app->gui->Draw();

	if (app->debug)
	{
		app->DrawDebug();
	}

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool M_Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool M_Render::Load(pugi::xml_node& data)
{
	camera->SetPosition(iPoint(data.child("camera").attribute("x").as_int(), data.child("camera").attribute("y").as_int()));

	return true;
}

// Save Game State
bool M_Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera->GetPosition().x;
	cam.append_attribute("y") = camera->GetPosition().y;

	return true;
}

void M_Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void M_Render::DrawDebug()
{
}

void M_Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void M_Render::ResetViewPort()
{
	//SDL_RenderSetViewport(renderer, &viewport);
}

iPoint M_Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = app->win->GetScale();

	ret.x = (x - camera->GetPosition().x / scale);
	ret.y = (y - camera->GetPosition().y / scale);

	return ret;
}

iPoint M_Render::WorldToScreen(int x, int y) const
{
	iPoint ret;
	int scale = app->win->GetScale();

	ret.x = (x + camera->GetPosition().x / scale);
	ret.y = (y + camera->GetPosition().y / scale);

	return ret;
}
// Blit to screen
bool M_Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera->GetPosition().x * speed) + x * scale;
	rect.y = (int)(camera->GetPosition().y * speed) + y * scale;

	iPoint screen_position = app->render->WorldToScreen(x, y);

	if (camera->InsideRenderTarget(screen_position))
	{
		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}

		if (flip == SDL_FLIP_HORIZONTAL)
		{
			screen_position.x -= (rect.w - pivot_x);
			screen_position.y -= pivot_y;
		}

		else if (flip == SDL_FLIP_VERTICAL)
		{
			screen_position.x -= pivot_x;
			screen_position.y -= (rect.h - pivot_y);
		}

		else if (flip == SDL_FLIP_NONE)
		{
			screen_position.x -= pivot_x;
			screen_position.y -= pivot_y;
		}

		rect.w *= scale;
		rect.h *= scale;

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (pivot_x != INT_MAX && pivot_y != INT_MAX)
		{
			pivot.x = pivot_x;
			pivot.y = pivot_y;
			p = &pivot;
		}
		rect = app->render->camera->GetRect();
		if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, (SDL_RendererFlip)flip) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

// Blit to screen
bool M_Render::Blit(SDL_Texture* texture, const SDL_Rect* _rect, const SDL_Rect* section, bool useCamera, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint scale = app->win->GetScale();
	SDL_Rect rect(*_rect);

	if (useCamera)
	{
		rect.x = (int)(camera->GetRect().x + rect.x * scale);
		rect.y = (int)(camera->GetRect().y + rect.y * scale);
		rect.w *= scale;
		rect.h *= scale;
	}
	else
	{
		rect.x = (int)(camera->GetRect().x * speed) + _rect->x * scale;
		rect.y = (int)(camera->GetRect().y * speed) + _rect->y * scale;
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = nullptr;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool M_Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool useCamera, bool useGameViewPort) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(useCamera)
	{
		rec.x = (int)(camera->GetRect().x + rect.x - (useGameViewPort == true ? gameViewPort.x : 0) * scale);
		rec.y = (int)(camera->GetRect().y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool M_Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera, bool useGameViewPort) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	int realPosX1 = x1 - (useGameViewPort == true ? gameViewPort.x : 0);
	int realPosX2 = x2 - (useGameViewPort == true ? gameViewPort.x : 0);

	if(useCamera)
		result = SDL_RenderDrawLine(renderer, camera->GetRect().x + realPosX1 * scale, camera->GetRect().y + y1 * scale, camera->GetRect().x + realPosX2 * scale, camera->GetRect().y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, realPosX1 * scale, y1 * scale, realPosX2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw line to screen. SDL_RenderDrawLine error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool M_Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera, bool useGameViewPort) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	int realPosX = x - (useGameViewPort == true ? gameViewPort.x : 0);

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(realPosX + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw circle to screen. SDL_RenderDrawPoints error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void M_Render::DrawEntities(std::vector<Entity*> entities)
{
	//TODO: This comparison must be check with final units and objects to check which would be the best way to order the vector.
	std::sort(entities.begin(), entities.end(), ([](Entity* et1, Entity* et2) { return (et1->GetGlobalPosition().y + et1->GetEnclosingBox().h / 2) < (et2->GetGlobalPosition().y + et2->GetEnclosingBox().h / 2); }));
	//return et1->GetGlobalPosition().y < et2->GetGlobalPosition().y;
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		Entity* tmp = (*it);

		if (tmp != nullptr)
		{
			SDL_Texture* texture = tmp->GetTexture();

			if (texture != nullptr)
			{
				uint scale = app->win->GetScale();
				GB_Rectangle<int> section = tmp->GetDrawQuad();
				fPoint pos = tmp->GetGlobalPosition();
				SDL_Rect finalRect;

				finalRect.x = (int)(camera->GetRect().x /* * speed */) + pos.x - gameViewPort.x  * scale; //TODO: Take into account viewport position and viewport ratio
				finalRect.y = (int)(camera->GetRect().y /* * speed */) + pos.y * scale; //TODO: Take into account viewport position and viewport ratio

				finalRect.w = section.w * scale * tmp->scale.x; //TODO: Viewport ratio
				finalRect.h = section.h * scale * tmp->scale.y; //TODO: Viewport ratio

				SDL_Point piv;
				iPoint p = tmp->GetPivot();
				piv.x = p.x;
				piv.y = p.y;

				finalRect.x -= p.x;
				finalRect.y -= p.y;


				if (SDL_RenderCopyEx(renderer, texture, &section.GetSDLrect(), &finalRect, 0, nullptr, SDL_FLIP_NONE) != 0)
				{
					LOG("ERROR: Could not blit to screen entity [%s]. SDL_RenderCopyEx error: %s.\n", tmp->GetName(), SDL_GetError());
				}
			}
		}
	}
}

Camera::Camera(SDL_Rect & rect) : moving(false), destination(0,0), speed(0), movement(0,0), follow(nullptr)
{
	viewport.x = rect.x;
	viewport.y = rect.y;
	viewport.w = rect.w;
	viewport.h = rect.h;
}

SDL_Rect Camera::GetRect() const
{
	return viewport;
}

//TODO: Fix this function. Math operations are wrong and maybe lacks from some returns.
bool Camera::InsideRenderTarget(iPoint pos)
{
	pos.x = (pos.x - viewport.w / 2.0f) + viewport.w / 2.0f;

	pos.y = (pos.x - viewport.h / 2.0f) + viewport.h / 2.0f;

	int tileWidth = app->map->data.tileWidth;
	int tileHeight = app->map->data.tileHeight;

	if (pos.x < tileWidth)
		return false;

	if (pos.x > viewport.w)
		return false;

	if (pos.y < tileHeight)
		return false;

	return true;
}

void Camera::SetPosition(iPoint pos)
{
	viewport.x = pos.x;
	viewport.y = pos.y;
}

void Camera::SetSize(iPoint size)
{
	viewport.w = size.x;
	viewport.h = size.y;
}

const iPoint Camera::GetPosition() const
{
	return iPoint(viewport.x, viewport.y);
}

const iPoint Camera::GetSize() const
{
	return iPoint(viewport.w, viewport.h);
}

void Camera::Move(iPoint destination, int speed) //Automatic camera
{
	moving = true;

	this->destination = destination;
	this->speed = speed;

	movement.y = abs(destination.y - viewport.y);
	movement.x = abs(destination.x - viewport.x);
}

void Camera::Move(float amount, camera_direction direction) //Manual camera
{
	switch (direction)
	{
	case UP:
		viewport.y += floor(amount);
		break;

	case DOWN:
		viewport.y -= floor(amount);
		break;

	case LEFT:
		viewport.x += floor(amount);
		break;

	case RIGHT:
		viewport.x -= floor(amount);
		break;
	}
}

void Camera::CenterCamUnit(Entity * entity) //Follow Unit
{
	follow = entity;
	centerCamUnit = true;
}

void Camera::UnCenterCamUnit() //Unfollow Unit
{
	centerCamUnit = false;
}

void Camera::SetCenter(iPoint pos)
{
	viewport.x = -(pos.x - viewport.w / 2);
	viewport.y = -(pos.y - viewport.h / 2);
}

void Camera::UpdateCamera()
{
	if (moving)
	{
		float total_movement = movement.x + movement.y;

		if (destination.x > viewport.x)
		{
			if ((speed * (float(movement.x) / float(total_movement))) > (destination.x - viewport.x))
			{
				viewport.x = destination.x;
				moving = false;
			}
			else
				viewport.x += speed * (float(movement.x) / float(total_movement));
		}
		else
		{
			if ((speed * (float(movement.x) / float(total_movement))) > abs(destination.x - viewport.x))
			{
				viewport.x = destination.x;
				moving = false;
			}
			else
				viewport.x -= speed * (float(movement.x) / float(total_movement));
		}

		if (destination.y > viewport.y)
		{
			if ((speed * (float(movement.y) / float(total_movement))) > (destination.y - viewport.y))
			{
				viewport.y = destination.y;
				moving = false;
			}
			else
				viewport.y += speed * (float(movement.y) / float(total_movement));
		}
		else
		{
			if ((speed * (float(movement.y) / float(total_movement))) > abs(destination.y - viewport.y))
			{
				viewport.y = destination.y;
				moving = false;
			}
			else
				viewport.y -= speed * (float(movement.x) / float(total_movement));
		}
	}
}
