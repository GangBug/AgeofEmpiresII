#include "j1Animation.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "p2Log.h"
#include "j1Render.h"

j1Animation::j1Animation()
{
	name.assign("animation");
}

// Destructor
j1Animation::~j1Animation()
{}

bool j1Animation::Start()
{
	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	textures.push_back(Textures(App->tex->Load("animations/CavalryArcher.png"), CAVALRYARCHER));
	textures.push_back(Textures(App->tex->Load("animations/Twohandedswordman.png"), TWOHANDEDSWORDMAN));
	textures.push_back(Textures(App->tex->Load("animations/SiegeRam.png"), SIEGERAM));
	textures.push_back(Textures(App->tex->Load("animations/Twohandedswordmanenemy.png"), TWOHANDEDSWORDMANENEMY));
	textures.push_back(Textures(App->tex->Load("animations/archer.png"), ARCHER));
	
	return true;
}
bool j1Animation::Awake(pugi::xml_node& config)
{
	bool ret = true;

	std::string anim_folder = "animations/Units_data.xml";	//TODO: change name

	//Load animations data from animations folder
	char* buff = nullptr;
	int size = App->fs->Load(anim_folder.c_str(), &buff);
	pugi::xml_document anim_data;
	pugi::xml_parse_result result = anim_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading animations data: %s", result.description());
		return false;
	}

	//Loading units
	pugi::xml_node unit_node = anim_data.child("Sprites").first_child();

	while (unit_node != NULL)
	{
		pugi::xml_node action_node = unit_node.first_child();
		while (action_node != NULL)
		{
			pugi::xml_node direction_node = action_node.first_child();
			while (direction_node != NULL)
			{
				Animation* new_anim = new Animation(unit_node.name());
				pugi::xml_node sprite_node = direction_node.first_child();
				while (sprite_node != NULL)
				{
					new_anim->frames.push_back({ sprite_node.attribute("x").as_int(),sprite_node.attribute("y").as_int(), sprite_node.attribute("w").as_int(),sprite_node.attribute("h").as_int() });
					
					float pX = sprite_node.attribute("w").as_int() * sprite_node.attribute("pX").as_float();
					float pY = sprite_node.attribute("h").as_int() * sprite_node.attribute("pY").as_float();
					pX = (pX > (floor(pX) + 0.5f)) ? ceil(pX) : floor(pX);
					pY = (pY > (floor(pY) + 0.5f)) ? ceil(pY) : floor(pY);
					new_anim->pivot_points.push_back({ (int)pX, (int)pY });
					
					sprite_node = sprite_node.next_sibling();
				}
				std::string anim_name = unit_node.name();
				new_anim->name = anim_name + "_" + action_node.name() + "_" + direction_node.name();
				new_anim->SetUnit(unit_node);
				new_anim->SetAction(action_node);
				new_anim->SetDirection(direction_node);

				std::string action = action_node.name();
				if (!action.compare("disappear"))
				{
					new_anim->speed = 100.0f;
					new_anim->loop = false;
				}
				animations.push_back(new_anim);

				direction_node = direction_node.next_sibling();
			}
			action_node = action_node.next_sibling();
		}
		unit_node = unit_node.next_sibling();
	}

	return ret;
}


bool j1Animation::CleanUp()
{
	std::list<Animation*>::iterator temp = animations.begin();
	while (temp != animations.end())
	{
		temp._Ptr->_Myval->CleanUp();
		delete temp._Ptr->_Myval;
		temp++;
	}

	animations.clear();

	return true;
}

Animation* j1Animation::GetAnimation(const UNIT_TYPE unit, const ACTION_TYPE action, DIRECTION direction)
{
	switch (direction)
	{
	case NORTH_EAST:
		direction = NORTH_WEST;
		break;

	case EAST:
		direction = WEST;
		break;

	case SOUTH_EAST:
		direction = SOUTH_WEST;
		break;

	default:
		break;
	}
	std::list<Animation*>::iterator ret = animations.begin();
	while (ret != animations.end())
	{
		if (ret._Ptr->_Myval->unit_type == unit && ret._Ptr->_Myval->action_type == action && ret._Ptr->_Myval->direction_type == direction)
			return ret._Ptr->_Myval;

		ret++;
	}

	return nullptr;
}

SDL_Texture * j1Animation::GetTexture(const UNIT_TYPE unit)
{
	std::vector<Textures>::iterator ret = textures.begin();
	while (ret != textures.end())
	{
		if (ret._Ptr->unit == unit)
			return ret._Ptr->texture;

		ret++;
	}
	return nullptr;
}

Animation* j1Animation::DrawAnimation(const UNIT_TYPE unit, const ACTION_TYPE action, DIRECTION direction, iPoint pos)
{
	bool flip = false;
	//direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST
	switch (direction)
	{
	case NORTH_EAST:
		flip = true;
		direction = NORTH_WEST;
		break;

	case EAST:
		flip = true;
		direction = WEST;
		break;

	case SOUTH_EAST:
		flip = true;
		direction = SOUTH_WEST;
		break;

	default:
		break;
	}
	
	Animation* anim = App->anim->GetAnimation(unit, action, direction);
		SDL_Texture* tex = App->anim->GetTexture(unit);
		SDL_Rect rect = anim->GetCurrentFrame();
		iPoint* p = &anim->GetCurrentPivotPoint();

		if (anim == NULL)
		{
			LOG("ERROR: DrawAnimation: animation not found");
			return NULL;
		}

		if (tex == NULL)
		{
			LOG("ERROR: DrawAnimation: texture not found");
			return NULL;
		}

		if (p == NULL)
		{
			LOG("ERROR: DrawAnimation: pivot point not found");
			return NULL;
		}
		if (flip == true)
			App->render->Blit(tex, pos.x - p->x, pos.y - p->y, &rect, SDL_FLIP_HORIZONTAL);

		else
			App->render->Blit(tex, pos.x - p->x, pos.y - p->y, &rect);

	return anim;
}

bool j1Animation::GetAnimationFrame(SDL_Rect& frame, iPoint& pivot, const Unit* unit)
{
	bool ret = false;
	//direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST
	DIRECTION direction = unit->GetDir();

	switch (direction)
	{
	case NORTH_EAST:
		direction = NORTH_WEST;
		break;

	case EAST:
		direction = WEST;
		break;

	case SOUTH_EAST:
		direction = SOUTH_WEST;
		break;

	default:
		break;
	}

	Animation* anim = App->anim->GetAnimation(unit->GetUnitType(), unit->GetActionType(), direction);
	if (anim->Finished() == false)
	{
		frame = anim->GetCurrentFrame();
		pivot = anim->GetCurrentPivotPoint();

		if (anim == NULL)
		{
			LOG("ERROR: GetAnimationFrame: animation not found");
			return NULL;
		}
	}
	return anim->Finished();
}


//--------------------------------------------------------------------------------------//

Animation::Animation(std::string name): name(name)
{
	speed = 100.0f;
}

// Destructor
Animation::~Animation()
{}

void Animation::SetSpeed(float spd)
{
	speed = spd;
}

void Animation::SetLoopState(bool state)
{
	loop = state;
}

SDL_Rect& Animation::GetCurrentFrame()
{
	if (current_frame == -1)
		return frames[(int)frames.size() - 1];
	
	current_frame = (float) floor(anim_timer.Read() / speed);

	if (current_frame >= frames.size())
	{
		if (loop == true)
		{
			anim_timer.Start();
			current_frame = 0;
			loops++;
		}
		else
		{
			current_frame = -1;
			loops = 1;
			return frames[(int)frames.size() - 1];
		}
	}

	return frames[(int)current_frame];
}

iPoint& Animation::GetCurrentPivotPoint()
{
	return pivot_points[(int)current_frame];
}

bool Animation::Finished() const
{
	return (loop == false && loops > 0);
}

bool Animation::FinishedLoop() const
{
	return loops > 0;
}

void Animation::Reset()
{
	current_frame = 0;
	anim_timer.Start();
	loops = 0;
}

void Animation::SetUnit(const pugi::xml_node node)
{
	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	if (strcmp(node.name(), "twohandedswordman") == 0)
		unit_type = TWOHANDEDSWORDMAN;

	else if (strcmp(node.name(), "twohandedswordmanenemy") == 0)
		unit_type = TWOHANDEDSWORDMANENEMY;

	else if (strcmp(node.name(), "cavalryarcher") == 0)
		unit_type = CAVALRYARCHER;

	else if (strcmp(node.name(), "siegeram") == 0)
		unit_type = SIEGERAM;

	else if (strcmp(node.name(), "archer") == 0)
		unit_type = ARCHER;

	else
	{
		unit_type = NO_UNIT;
		LOG("ERROR: XML Node UNIT TYPE does not match");
	}
}

void Animation::SetAction(const pugi::xml_node node)
{
	if (strcmp(node.name(), "attack") == 0)
		action_type = ATTACK;

	else if (strcmp(node.name(), "die") == 0)
		action_type = DIE;

	else if (strcmp(node.name(), "disappear") == 0)	//IF PROBLEM CHECK DIsAppEAR SPELLING!!!!!
		action_type = DISAPPEAR;

	else if (strcmp(node.name(), "idle") == 0)
		action_type = IDLE;

	else if (strcmp(node.name(), "walk") == 0)
		action_type = WALK;

	else
	{
		action_type = NO_ACTION;
		LOG("ERROR: XML Node ACTION TYPE does not match");
	}
}

void Animation::SetDirection(const pugi::xml_node node)
{
	if (strcmp(node.name(), "north") == 0)
		direction_type = NORTH;

	else if (strcmp(node.name(), "south") == 0)
		direction_type = SOUTH;

	else if (strcmp(node.name(), "south_west") == 0)
		direction_type = SOUTH_WEST;

	else if (strcmp(node.name(), "west") == 0)
		direction_type = WEST;

	else if (strcmp(node.name(), "north_west") == 0)
		direction_type = NORTH_WEST;

	else
	{
		direction_type = NO_DIRECTION;
		LOG("ERROR: XML Node DIRECTION TYPE does not match");
	}

}

bool Animation::CleanUp()
{
	frames.clear();
	pivot_points.clear();

	return true;
}