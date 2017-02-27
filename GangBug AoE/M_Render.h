#ifndef __M_RENDER_H__
#define __M_RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "Module.h"
#include <vector>

class Entity;

class M_Render : public Module
{
public:

	M_Render(bool startEnabled = true);

	// Destructor
	virtual ~M_Render();

	// Called before render is available
	bool Awake(pugi::xml_node&)override;

	// Called before the first frame
	bool Start()override;

	// Called each loop iteration
	update_status PreUpdate(float dt)override;
	update_status PostUpdate(float dt)override;

	// Called before quitting
	bool CleanUp()override;

	// Load / Save
	bool Load(pugi::xml_node&)override;
	bool Save(pugi::xml_node&) const override;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;

	// Draw & Blit
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	void DrawDebug()override;

private:
	void DrawEntities(std::vector<Entity*> entities);


public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;
};

#endif // __j1RENDER_H__