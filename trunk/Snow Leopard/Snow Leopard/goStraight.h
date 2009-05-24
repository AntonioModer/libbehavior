#ifndef GOSTRAIGHT_H_
#define GOSTRAIGHT_H_
#include "BehaviorTreeNode.h"

using namespace BehaviorTree;
namespace SL
{
	class goStraight:public BehaviorTree::BehaviorTreeNode
{
	BehaviorTreeNode::BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
	BehaviorTree::BehaviorTreeNode::BehaviorTreeList getChildren()
	{
		return BehaviorTree::BehaviorTreeNode::BehaviorTreeList();
	}
};

}
#endif