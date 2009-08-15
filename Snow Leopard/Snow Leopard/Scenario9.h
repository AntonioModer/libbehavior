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
#include "WaitForNextWave.h"




#ifndef SCENARIO9_H_
#define SCENARIO9_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{

	void spawnProjectile()
	{

	}

	WorldState* loadScenario9()
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

		director->brain->addChild(
			(new SequentialNode())
			->addChild(new SpawnShip(1))
			->addChild(new Cooldown(100))
			->addChild(new SpawnShip(2))
			->addChild(new Cooldown(100))
			->addChild(new SpawnShip(3))
			->addChild(new Cooldown(100)));


		return state;
	}

	


}
#endif