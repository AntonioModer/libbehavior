#ifndef TURNTOWARDSTARGET_H_
#define TURNTOWARDSTARGET_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
		/// Behavior of turning to face a target
		class TurnTowardsTarget:public BehaviorTree::BehaviorTreeNode
		{
			BEHAVIOR_STATUS execute(void* agent);
			void init(void* agent);
			BehaviorTree::BehaviorTreeList getChildren()
			{
				return BehaviorTree::BehaviorTreeList();
			}
		};
	}

}
#endif