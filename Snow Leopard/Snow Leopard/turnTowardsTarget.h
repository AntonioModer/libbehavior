#ifndef TURNTOWARDSTARGET_H_
#define TURNTOWARDSTARGET_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	class turnTowardsTarget:public BehaviorTree::BehaviorTreeNode
{
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
	BehaviorTree::BehaviorTreeList getChildren()
	{
		return BehaviorTree::BehaviorTreeList();
	}
};

}
#endif