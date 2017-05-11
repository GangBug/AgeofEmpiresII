#ifndef __M_RENDER_H__
#define __M_RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "Module.h"
#include <vector>
#include "GB_Rectangle.h"

#define UPPER_LIMIT 0
#define LOWER_LIMIT -3000
#define LEFT_LIMIT 4000
#define RIGHT_LIMIT -2000

enum camera_direction
{
	NONE = 0,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Entity;

class Camera
{
private:
	SDL_Rect viewport;
	iPoint destination;
	fPoint movement;

	int speed;

	bool centerCamUnit;
	bool moving;

	Entity* follow;

public:
	Camera(SDL_Rect& rect);

	void SetPosition(iPoint pos);
	void SetSize(iPoint size);

	const iPoint GetPosition() const;
	const iPoint GetSize() const;
	const iPoint GetCenter() const;

	const SDL_Rect GetVP() const;
	const float GetOpacity() const;

	bool InsideRenderTarget(GB_Rectangle<int> rect);

	void Move(iPoint destination, int speed);
	void Move(float amount, camera_direction direction);

	SDL_Rect GetRect() const;

	void CenterCamUnit(Entity* entity);
	void UnCenterCamUnit();
	void SetCenter(iPoint pos);

	void UpdateCamera();

public:
	void MoveUp(float amount);
	void MoveLeft(float amount);
	void MoveDown(float amount);
	void MoveRight(float amount);

};

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
	iPoint WorldToScreen(int x, int y) const;

	// Draw & Blit
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0) const;
	bool Blit(SDL_Texture* texture, const SDL_Rect* _rect, const SDL_Rect* section = nullptr, bool useCamera = false, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool BlitAdri(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true, bool useGameViewPort = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true, bool useGameViewPort = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true, bool useGameViewPort = true) const;

	bool FogBlit(iPoint& position, uint cell_size, Uint8 alpha);

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	void DrawDebug()override;


private:
	void DrawEntities(std::vector<Entity*> entities);

public:

	SDL_Renderer*	renderer;
	Camera*			camera;
	//SDL_Rect		viewport;
	SDL_Color		background;

	SDL_Rect		editorViewPort;
	SDL_Rect		gameViewPort;
	SDL_Rect		miniMapViewPort;
	SDL_Texture*	game_tex_background;
	SDL_Texture*	editor_tex_background;
};

#endif // __M_RENDER_H__