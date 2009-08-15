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
#include "WaitForSignal.h"




#ifndef SCENARIO9_H_
#define SCENARIO9_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	bool projectileSignal = false;
	
	BehaviorTreeInternalNode* makeSynchronizedProjectile()
	{
		BehaviorTreeInternalNode* brain = new ParallelNode();
		brain->addChild(new WaitForSignal(projectileSignal));
		brain->addChild((new ParallelNode())
			->addChild(new TurnTowardsTarget(2))
			->addChild(new GoStraight(5)));
		
		return brain;
	}

	void flipProjectileSignal(void* ignore)
	{
		projectileSignal = true;
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
		player->projectileBrain = &makeBoringBrain;
		state->insertObject(player,point(100,300));

		Ship* director = new Ship();
		director->setSprite("Hulls\\Sample Hull");
		director->HP = INT_MAX;

		director->projectileBrain = &makeSynchronizedProjectile;
		


		director->brain->addChild(
			(new SequentialNode())
			->addChild(new Fire())
			->addChild(new Cooldown(100))
			->addChild(new Fire())
			->addChild(new Cooldown(100))
			->addChild(new Fire())
			->addChild(new Cooldown(100))
			->addChild(new FunctionCall<>(&flipProjectileSignal)));


		return state;
	}

	


}
#endif