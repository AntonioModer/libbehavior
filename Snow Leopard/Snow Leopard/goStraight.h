#ifndef GOSTRAIGHT_H_
#define GOSTRAIGHT_H_
#include "BehaviorTreeNode.h"

using namespace BehaviorTree;
namespace SL
{
	class goStraight:public BehaviorTree::BehaviorTreeNode
{
	BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
	BehaviorTree::BehaviorTreeList getChildren()
	{
		return BehaviorTree::BehaviorTreeList();
	}
};

}
#endif