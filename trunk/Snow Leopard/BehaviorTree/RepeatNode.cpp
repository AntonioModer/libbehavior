#include "BehaviorTree.h"

using namespace BehaviorTree;
using namespace std;
BEHAVIOR_STATUS RepeatNode::execute(void* agent)
{
	if (children.size()== 0)
		return BT_SUCCESS;
	else
	{
		BEHAVIOR_STATUS status = children.at(0)->execute(agent);
		if (status == BT_SUCCESS)
		{
			count++;
			if (count == target)
			{
				init(agent);
				return BT_SUCCESS;
			}
			else
				return BT_RUNNING;
		}
		return status;
	}
}

void BehaviorTree::RepeatNode::init( void* agent )
{
	count = 0;
	if (children.size() == 1)
		children.at(0)->init(agent);
}

BehaviorTreeInternalNode* BehaviorTree::RepeatNode::addChild( BehaviorTreeNode* newChild )
{
	if (children.size() == 0)
		BehaviorTreeInternalNode::addChild(newChild);
	else
		throw new std::exception("Cannot add more than one child to a repeat node");

	return this;
}

RepeatNode::RepeatNode(int repeats)
{
	target = repeats;
	count = 0;
};