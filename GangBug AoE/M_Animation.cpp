#include "M_Animation.h"
#include "M_Textures.h"
#include "App.h"
#include "M_FileSystem.h"
#include "M_Textures.h"

#include "Log.h"

M_Animation::M_Animation(bool startEnabled) : Module(startEnabled)
{
	LOG("Animation: Creation.");
	name.assign("animation");
}

M_Animation::~M_Animation() {
	LOG("Animation: Destroying.");
}

bool M_Animation::Awake(pugi::xml_node& none)
{
	LOG("Animation: Awake.");
	bool ret = true;

	std::string animationInfo = "animation/Units_data.xml"; //TODO: Folder with xml info on animations

	char* buff = nullptr;
	int size = app->fs->Load(animationInfo.c_str(), &buff);
	pugi::xml_document animationData;
	pugi::xml_parse_result result = animationData.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL) 
	{
		LOG("Animation: Failed loading at awake: %s", result.description());
		return false;
	}

	//LOAD UNIT ANIMATIONS FROM XML

	pugi::xml_node node = animationData.child("Sprites").first_child();
	while (node != NULL)
	{
		pugi::xml_node actionNode = node.first_child();
		while (actionNode != NULL)
		{
			pugi::xml_node directionNode = actionNode.first_child();
			while (directionNode != NULL)
			{
				Animation* newAnim = new Animation(node.name());
				pugi::xml_node spriteNode = directionNode.first_child();
				while (spriteNode != NULL)
				{
					newAnim->frames.push_back({spriteNode.attribute("x").as_int(), spriteNode.attribute("y").as_int(), spriteNode.attribute("w").as_int(), spriteNode.attribute("h").as_int()});

					float pX = spriteNode.attribute("w").as_int() * spriteNode.attribute("pX").as_float();
					float pY = spriteNode.attribute("h").as_int() * spriteNode.attribute("pY").as_float();
					pX = (pX > (floor(pX) + 0.5f)) ? ceil(pX) : floor(pX);
					pY = (pY > (floor(pY) + 0.5f)) ? ceil(pY) : floor(pY);
					newAnim->pivotPoints.push_back({(int)pX, (int)pY});

					spriteNode = spriteNode.next_sibling();
				}

				std::string animName = node.name();
				newAnim->name = animName + "_" + actionNode.name() + "_" + directionNode.name();
				newAnim->SetUnit(node);
				newAnim->SetAction(actionNode);//TODO
				newAnim->SetDirection(directionNode);//TODO

				std::string action = actionNode.name();
				if (!action.compare("disappear") || !action.compare("die"))
				{
					newAnim->speed = 200.0f;
					newAnim->loop = false;
				}
				animations.push_back(newAnim);

				directionNode = directionNode.next_sibling();
			}
			actionNode = actionNode.next_sibling();
		}
		node = node.next_sibling();
	}
	return ret;
}

bool M_Animation::Start()
{
	LOG("Animation: Start.");
	bool ret = true;

	//ADD UNITS IMPORTANT: NEW UNITS PNGS GO HERE
	textures.push_back(Texture(app->tex->Load("animation/archer.png"), ARCHER));
	textures.push_back(Texture(app->tex->Load("animation/TarkanSpritesheet.png"), TARKAN_KNIGHT));
	textures.push_back(Texture(app->tex->Load("animation/SamuraiSpriteSheet.png"), SAMURAI));
	textures.push_back(Texture(app->tex->Load("animation/Boss.png"), DIABLO));
	textures.push_back(Texture(app->tex->Load("animation/VileChild_SpriteSheet.png"), VILE));
	textures.push_back(Texture(app->tex->Load("animation/HellWitch.png"), HELL_WITCH));

	return ret;
}

bool M_Animation::CleanUp()
{
	bool ret = true;

	for (std::list<Animation*>::iterator it = animations.begin(); it != animations.end(); it++)
	{
		(*it)->CleanUp();
		delete (*it);
	}

	animations.clear();

	return ret;
}

Animation* M_Animation::GetAnimation(unit_type unit, action_type action, direction unitDirection)const
{
	switch (unitDirection)
	{
	case NORTH_EAST:
		unitDirection = NORTH_WEST;
		break;

	case EAST:
		unitDirection = WEST;
		break;

	case SOUTH_EAST:
		unitDirection = SOUTH_WEST;
		break;

	default:
		break;
	}

	for (std::list<Animation*>::const_iterator it = animations.begin(); it != animations.end(); it++)
	{
		if ((*it)->unitType == unit && (*it)->unitAction == action && (*it)->unitDirection == unitDirection)
		{
			return *it;
		}
	}

	return nullptr;
}

SDL_Texture* M_Animation::GetTexture(unit_type type) const{
	for (std::vector<Texture>::const_iterator it = textures.begin(); it != textures.end(); it++)
	{
		if (it->unit == type) 
		{
			return it->texture;
		}
	}

	LOG("Could not find texture for that unit.");
	return nullptr;
}

void M_Animation::GetFrame(GB_Rectangle<int>& rect, iPoint& pivot, const Unit* unit)
{
	Animation* tmp = GetAnimation(unit->GetType(), unit->GetAction(), unit->GetDir());

	if (tmp == nullptr) 
	{
		LOG("No unit found");
		return;
	}
	
	rect = tmp->GetCurrentFrame();
	if (tmp->Finished() == false)
	{
		pivot = tmp->GetCurrentPivot();
	}
}

void M_Animation::DrawDebug()
{
}


//-------------------------------------------ANIMATION-------------------------

Animation::Animation(std::string name) : name(name)
{
}

Animation::~Animation()
{
}

void Animation::CleanUp()
{
	frames.clear();
	pivotPoints.clear();

}

void Animation::SetLoop(bool loop)
{
	this->loop = loop;
}

void Animation::SetSpeed(float speed) 
{
	this->speed = speed;
}

bool Animation::Finished()const
{
	return (loops > 0 && loop == false);
}

void Animation::Reset()
{
	currentFrame = 0;
	animationTimer.Start();
	loops = 0;
}

void Animation::SetUnit(const pugi::xml_node node)
{
	//IMPORTANT: NEW UNITS NEED TO GO HERE TOO
	if (strcmp(node.name(), "archer") == 0)
		unitType = ARCHER;

	else if (strcmp(node.name(), "tarkannnight") == 0)
		unitType = TARKAN_KNIGHT;

	else if (strcmp(node.name(), "samurai") == 0)
		unitType = SAMURAI;

	else if (strcmp(node.name(), "boss") == 0)
		unitType = DIABLO;

	else if (strcmp(node.name(), "vile") == 0)
		unitType = VILE;
	else if (strcmp(node.name(), "hellwitch") == 0)
		unitType = HELL_WITCH;

	else
	{
		unitType = DEFAULT_UNIT;
		LOG("ERROR: XML Node UNIT TYPE does not match");
	}
}

void Animation::SetAction(const pugi::xml_node node)
{
	if (strcmp(node.name(), "attack") == 0)
		unitAction = ATTACK;

	else if (strcmp(node.name(), "die") == 0)
		unitAction = DIE;

	else if (strcmp(node.name(), "disappear") == 0)	//
		unitAction = DISAPPEAR;

	else if (strcmp(node.name(), "idle") == 0)
		unitAction = IDLE;

	else if (strcmp(node.name(), "walk") == 0)
		unitAction = WALK;

	else
	{
		unitAction = NO_ACTION;
		LOG("ERROR: XML Node ACTION TYPE does not match");
	}
}

void Animation::SetDirection(const pugi::xml_node node)
{
	if (strcmp(node.name(), "north") == 0)
	{
		unitDirection = NORTH;
	}

	else if (strcmp(node.name(), "north_west") == 0)
	{
		unitDirection = NORTH_WEST;
	}

	else if (strcmp(node.name(), "north_east") == 0)
	{
		unitDirection = NORTH_EAST;
	}

	else if (strcmp(node.name(), "east") == 0)
	{
		unitDirection = EAST;
	}

	else if (strcmp(node.name(), "west") == 0)
	{
		unitDirection = WEST;
	}

	else if (strcmp(node.name(), "south") == 0)
	{
		unitDirection = SOUTH;
	}

	else if (strcmp(node.name(), "south_east") == 0)
	{
		unitDirection = SOUTH_EAST;
	}

	else if (strcmp(node.name(), "south_west") == 0)
	{
		unitDirection = SOUTH_WEST;
	}

}

GB_Rectangle<int>& Animation::GetCurrentFrame()
{
	if (currentFrame == -1)
	{
		return GB_Rectangle<int>{0, 0, 0, 0};
	}

	currentFrame = (float)floor(animationTimer.Read() / speed);

	if (currentFrame >= frames.size())
	{
		if (loop == true)
		{
			animationTimer.Start();
			currentFrame = 0;
			loops++;
		}
		else
		{
			currentFrame = -1;
			loops = 1;
			return GB_Rectangle<int>{0, 0, 0, 0};
		}
	}

	return frames[(int)currentFrame];
}

iPoint& Animation::GetCurrentPivot()
{
	return pivotPoints[(int)currentFrame];
}

