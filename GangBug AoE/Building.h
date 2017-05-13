#include "Entity.h"
#include "Unit.h"
#include "j1Timer.h"

class GUIElement;
class Particle;

enum building_type
{
	BUILD_ARCHERY,
	BUILD_STABLES,
	BUILD_BARRACK,
	BUILD_TOWNCENTER,
	BUILD_PORTAL,
	BUILD_NOBUILDING
};

class Building :
	public Entity
{
public:
	Building(building_type buildType, iPoint tileAttack, Entity* parent);
	virtual ~Building();

	void OnUpdate(float dt)override;

	bool OnSave(pugi::xml_node& node)const override;
	bool OnLoad(pugi::xml_node* node)override;
	void BuyUnit();

	int GetHP() const;
	void SetHP(int HP);
	void DoDamage(int dmg);
	void Repair(int amount);
	void PlaySelectFx();

	void PrintProgression();

	void Serialize(pugi::xml_node&);

public:

	Timer buyTimer;
	uint unitsToAdd;

	enum building_type buildType;
	enum unit_type unitType;

	iPoint tileAttack;

	int unitGoldCost;
	int unitWoodCost;
	int unitFoodCost;
	int HP;
	int fullHP;
	bool horde;
	Particle* portalParticle = nullptr;

private:

	GUIElement* creatorButton;
	GUIElement* unitInfoLabel;
	Particle* fire = nullptr;
	Particle* fire2 = nullptr;
	
};