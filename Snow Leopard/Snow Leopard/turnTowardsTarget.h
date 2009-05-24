#ifndef TURNTOWARDSTARGET_H_
#define TURNTOWARDSTARGET_H_
#include "BehaviorTreeNode.h"

using namespace BehaviorTree;
namespace SL
{
	class turnTowardsTarget:public BehaviorTree::BehaviorTreeNode
{
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
	BehaviorTree::BehaviorTreeNode::BehaviorTreeList getChildren()
	{
		return BehaviorTree::BehaviorTreeNode::BehaviorTreeList();
	}
};

}
#endif