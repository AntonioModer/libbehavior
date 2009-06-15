#include "Ship.h"
#include "GoStraight.h"
#include "AbsoluteMovement.h"
#include "Fire.h"
#include "Cooldown.h"
#include "WorldState.h"
#include "Projectile.h"
#include "turnTowardsTarget.h"
#include "BehaviorTree.h"

#ifndef BEHAVIORFACTORY_H_
#define BEHAVIORFACTORY_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	ParallelNode* makeHomingBrain()
	{
		ParallelNode* b = new ParallelNode();
		b->addChild(new TurnTowardsTarget(2));
		b->addChild(new GoStraight(5));
		return b;
	}

	ParallelNode* makeBoringBrain()
	{
		ParallelNode* b = new ParallelNode();
		b->addChild(new GoStraight(10));
		return b;
	}
}
#endif