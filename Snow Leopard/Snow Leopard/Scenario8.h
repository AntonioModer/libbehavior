#include "Ship.h"
#include "GoStraight.h"
#include "AbsoluteMovement.h"
#include "DestroyAllNearbyProjectiles.h"
#include "Fire.h"
#include "Cooldown.h"
#include "WorldState.h"
#include "Projectile.h"
#include "turnTowardsTarget.h"
#include "BehaviorTree.h"
#include "BehaviorFactory.h"
#include "GotoPoint.h"
#include "SpawnShip.h"

#ifndef SCENARIO8_H_
#define SCENARIO8_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{

	WorldState* loadScenario8()
	{
		WorldState* state = new WorldState();

		GameObject* background = new GameObject();
		background->usesPhysics = false;
		background->setSprite("Backgrounds\\background");
		state->addBackgroundObject(background);

		Ship* player = new Ship();
		player->setSprite("Hulls\\Sample Hull");
		player->isPlayer = true;
		player->getProjectileBrain = &makeBoringBrain;
		state->insertObject(player,point(100,300));

		GameObject* director = new GameObject();

		director->brain->addChild((new CountLimitNode(3,false))->addChild(new SpawnShip()));

		state->insertObject(director,point(0,0));

		return state;
	}


}
#endif