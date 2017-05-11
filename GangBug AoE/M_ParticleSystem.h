#ifndef __M_PARTICLESYSTEM_H__
#define __M_PARTICLESYSTEM_H__

#include "SDL\include\SDL_pixels.h"
#include "SDL/include/SDL.h"
#include "Module.h"
#include "j1Timer.h"
#include "ParticleAnimation.h"
#include "p2Point.h"
#include <list>
#include <string>

using namespace std;

#define GRAVITY 10
#define MAX_PARTICLES 50
#define EMITTER_SPEED 0.05

struct SDL_Texture;
class Collider;

enum ParticleType {
	BALL,
	STAR,
	FIRE,
	SMOKE,
	EXPLOSION,
	PORTAL,
	UNKNOWN
};

// Info holds the data gathered in Awake.
struct Info {
	string name;
	int id;
	int lifespan;
	ParticleAnimation anim;
	string path;
	SDL_Texture* texture;

	Info::Info(string argname, string argpath, int argid, int arglifespan, ParticleAnimation arganim)
		: name(argname), path(argpath), id(argid), lifespan(arglifespan), anim(arganim) {}
};

class Particle {
public:
	fPoint pos;

	int lifetime; // (s)
	bool alive = true;

	ParticleType type;
	SDL_Texture* texture;
	string name;
	Collider* collider;
	ParticleAnimation anim;
	Timer timer;

	virtual void Update() {};
	virtual void Draw() {};
	virtual bool IsAlive() { return true; };
};

class StaticBucle : public Particle {
public:
	StaticBucle(fPoint startingposition, bool);

	bool finite = false;

	void Update();
	void Draw();
	bool IsAlive();
};

class M_ParticleSystem : public Module {
public:

	M_ParticleSystem(bool startEnabled = true);

	virtual ~M_ParticleSystem();

	bool Awake(pugi::xml_node&);

	bool Start();

	update_status Update(float dt);

	void DrawParticles();

	bool PostUpdate();

	bool CleanUp();

	void DestroyParticles();

	void DrawDebug();

	Particle* CreateStaticBucle(fPoint startingposition, bool finite, ParticleType type);
public:
	bool DestroyParticle(Particle* curr);

public:
	pair<uint, uint> window_size;
	Timer update;
private:
	list<Particle*> particles;
	vector<Info> info;
};

#endif
