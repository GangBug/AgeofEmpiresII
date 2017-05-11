#include "M_ParticleSystem.h"
#include "M_Window.h"
#include "App.h"
#include "M_Render.h"
#include "M_Textures.h"
#include "Defs.h"
#include "Log.h"
#include <stdlib.h>  
#include <time.h>

M_ParticleSystem::M_ParticleSystem(bool startEnabled) : Module(startEnabled)
{
	name.assign("particlesystem");
}

M_ParticleSystem::~M_ParticleSystem()
{
}

// We are loading all the information from config here.
// Ideally we would have the position also taken from the xml.

bool M_ParticleSystem::Awake(pugi::xml_node &config)
{
	for (pugi::xml_node node = config.child("particle"); node; node = node.next_sibling("particle"))
	{
		string name(node.child("Info").child("Name").attribute("value").as_string());
		int id = node.child("Info").child("ID").attribute("value").as_int();
		string path(node.child("Textures").child("Texture").attribute("value").as_string());
		int lifespan = node.child("Stats").child("Lifespan").attribute("value").as_int();
		int w = node.child("Info").child("Collider").attribute("w").as_int();
		int h = node.child("Info").child("Collider").attribute("h").as_int();
		int rows = node.child("Animations").child("Animation").attribute("rows").as_int();
		int columns = node.child("Animations").child("Animation").attribute("columns").as_int();
		// -----------------------------
		SDL_Rect our_rect{ 0,0,w,h };
		ParticleAnimation anim;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				our_rect.x = j* w;
				our_rect.y = i* h;
				anim.PushBack(our_rect);
			}
			our_rect.x = 0;
		}
		anim.loop = true;
		anim.speed = 0.2f;

		Info curr(name, path, id, lifespan, anim);
		info.push_back(curr);
	}
	return true;
}

bool M_ParticleSystem::Start()
{
	srand(time(NULL));
	app->win->GetWindowSize(window_size.first, window_size.second);

	for (uint i = 0; i < info.size(); ++i) {
		info[i].texture = app->tex->Load(info[i].path.c_str());
	}

	return true;
}

update_status M_ParticleSystem::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void M_ParticleSystem::DrawParticles()
{
	update.Start();
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		it._Ptr->_Myval->Update();
	}
	LOG("Particles: %.10f", update.ReadSec());
	update.Start();
}

bool M_ParticleSystem::PostUpdate()
{
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		if (it._Ptr->_Myval->alive == false)
			DestroyParticle(it._Ptr->_Myval);
	}
	return true;
}

bool M_ParticleSystem::CleanUp()
{
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		if (it._Ptr->_Myval != nullptr)
			DestroyParticle(it._Ptr->_Myval);
	}

	for (uint i = 0; i < info.size(); ++i) {
		app->tex->UnLoad(info[i].texture);
	}

	particles.clear();
	return true;
}

void M_ParticleSystem::DestroyParticles()
{
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		if (it._Ptr->_Myval != nullptr)
			DestroyParticle(it._Ptr->_Myval);
	}
}

void M_ParticleSystem::DrawDebug()
{
}

Particle * M_ParticleSystem::CreateStaticBucle(fPoint startingposition, bool finite, ParticleType type)
{
	Particle* ret = nullptr;
	if (particles.size() < MAX_PARTICLES)
	{
		ret = new StaticBucle(startingposition, finite);
		ret->texture = info[type].texture;
		ret->anim = info[type].anim;
		ret->type = (ParticleType)info[type].id;
		ret->name = info[type].name;
		ret->lifetime = info[type].lifespan;
		particles.push_back(ret);
	}
	else {
		LOG("Maximum particles achieved.");
	}
	return ret;
}

bool M_ParticleSystem::DestroyParticle(Particle* curr)
{
	bool ret = true;
	if (curr != nullptr) {
		particles.remove(curr);
		//curr->CleanUp();
		delete curr;
	}
	else ret = false;
	return ret;
}

// -----------------------
//     STATIC BUCLES
// -----------------------
// Particles that stay in place for an indefinite amount
// of time repeating (often) the same animation.
// 1. The code makes all the animation print on the same place
// following the same pattern. This is not ideal, you might want your code to
// randomly print a different frame.
// 2. You can give it a lifetime on the xml and send 'true' on creation. Or
// You can send 'false' so it bucles forever.
//
// NOTICE that non finite particles that leave the screen are deleted, is that what your game needs?

StaticBucle::StaticBucle(fPoint startingposition, bool argfinite)
{
	pos = startingposition;
	finite = argfinite;
	anim.loop = false;
	timer.Start();
}

void StaticBucle::Update()
{
	if (type != PORTAL)
	{
		Draw();
	}
	alive = IsAlive();
}

void StaticBucle::Draw()
{
	if (type == PORTAL && pos.x > -1000)//estoy con el flip de los portales
	{
		app->render->Blit(texture, pos.x - 15, pos.y, &anim.GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}
	else
	{
		app->render->Blit(texture, pos.x, pos.y, &anim.GetCurrentFrame());
	}
}

bool StaticBucle::IsAlive()
{
	bool ret = true;
	if (timer.ReadSec() >= lifetime && finite == true) {
		ret = false;
	}
	else if (pos.x >= app->particleSystem->window_size.first || pos.y >= app->particleSystem->window_size.second || pos.x < 0 || pos.y < 0)
	{
		ret = false;
	}
	return ret;
}