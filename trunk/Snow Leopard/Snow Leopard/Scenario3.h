#include "Ship.h"
#include "GoStraight.h"
#include "AbsoluteMovement.h"
#include "Fire.h"
#include "Cooldown.h"
#include "WorldState.h"
#include "Projectile.h"
#include "turnTowardsTarget.h"
#include "BehaviorTree.h"

#ifndef SCENARIO3_H_
#define SCENARIO3_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	WorldState* loadScenario3()
	{
		WorldState* state = new WorldState();

		GameObject* background = new GameObject();
		background->usesPhysics = false;
		background->setSprite("Backgrounds\\background");
		state->addBackgroundObject(background);

		Ship* player = new Ship();
		player->setSprite("Hulls\\Sample Hull");
		player->isPlayer = true;
		state->insertObject(player,point(100,300));

		Ship* opponent = new Ship();
		opponent->displayHeading.set_degrees(180);
		opponent->setSprite("Hulls\\Fighter1");
		state->insertObject(opponent,point(400,50));
		return state;
	}


}
#endif