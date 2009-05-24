#ifndef GOSTRAIGHT_H_
#define GOSTRAIGHT_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
	/// Simple behavior of continuously going straight
	class GoStraight:public BehaviorTree::BehaviorTreeNode
	{
		BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		BehaviorTree::BehaviorTreeList getChildren()
		{
			return BehaviorTree::BehaviorTreeList();
		}
	};
	}

}
#endif