#ifndef BEHAVIORFACTORY_H_
#define BEHAVIORFACTORY_H_
#include "Ship.h"
#include "GoStraight.h"
#include "AbsoluteMovement.h"
#include "Fire.h"
#include "Cooldown.h"
#include "WorldState.h"
#include "Projectile.h"
#include "turnTowardsTarget.h"
#include "BehaviorTree.h"

using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	static ParallelNode* makeHomingBrain()
	{
		ParallelNode* b = new ParallelNode();
		b->addChild(new TurnTowardsTarget(2));
		b->addChild(new GoStraight(5));
		return b;
	}

	static ParallelNode* makeBoringBrain()
	{
		ParallelNode* b = new ParallelNode();
		b->addChild(new GoStraight(10));
		return b;
	}

	static ParallelNode* makeSlowBoringBrain()
	{
		ParallelNode* b = new ParallelNode();
		b->addChild(new GoStraight(3));
		return b;
	}
}
#endif