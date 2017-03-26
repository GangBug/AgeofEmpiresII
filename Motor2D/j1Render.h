#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Entity.h"

#define FPS 60

class Entity;

class Camera
{
private:
	SDL_Rect view_port;
	bool moving;
	iPoint destination;
	int speed;
	int x_movement;
	int y_movement;
	float zoom;
	int opacity;
	int frames_to_black;
	int frames_in_black;
	int frames_to_light;
	int opacity_delta;
	bool centerCamUnit;
	Entity* follow;

public:
	Camera(Camera& copy);
	Camera(SDL_Rect& rect);

	void SetPosition(iPoint pos);
	void SetSize(int w, int h);

	const iPoint GetPosition() const;
	const int GetWidth() const;
	const int GetHeight() const;
	const SDL_Rect GetVP() const;
	const iPoint GetCenter() const;
	const float GetOpacity() const;

	bool InsideRenderTarget(int x, int y);

	void Move(iPoint destination, int speed);
	void MoveUp(float amount);
	void MoveLeft(float amount);
	void MoveDown(float amount);
	void MoveRight(float amount);

	void SetZoom(float percentage); //%

	void Zoom(float percentage = 1);//percentage per frame

	SDL_Rect GetZoomedRect(const SDL_Rect &rect) const;

	void FadeToBlack(int secs_to_black, int wait, int secs_to_light);// secs at 60fps

	void CenterCamUnit(Entity* entity);
	void UnCenterCamUnit();
	void SetCenter(iPoint pos);

	void UpdateCamera();
};
class j1Render : public j1Module
{
public:
	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;
	iPoint WorldToScreen(int x, int y) const;

	// Draw & Blit
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Get width and height from texture
	void GetDimensionsFromTexture(SDL_Texture* tex, int& w, int& h);

	//Camera
	SDL_Rect ZoomedRect(const SDL_Rect &starting_rect) const;

	//Camera Properties
	float zoom;
public:

	SDL_Renderer*	renderer;
	Camera*			camera;
	SDL_Rect		viewport;
	SDL_Color		background;
};

#endif // __j1RENDER_H__