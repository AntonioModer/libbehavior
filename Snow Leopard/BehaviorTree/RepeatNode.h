namespace BehaviorTree
{
///A node that repeats its child a specified number of times.
class RepeatNode: public BehaviorTreeInternalNode
{
public:
	RepeatNode::RepeatNode(int repeats);
	BehaviorTreeInternalNode* addChild(BehaviorTreeNode* newChild);
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
protected:
	int count;
	int target;

};
}