namespace BehaviorTree
{
///Executes behaviors randomly, based on a given set of weights.
/** The weights are not percentages, but rather simple ratios.
For example, if there were two children with a weight of one, each would have a 50% chance of being executed.
If another child with a weight of eight were added, the previous children would have a 10% chance of being executed, and the new child would have an 80% chance of being executed.
This weight system is intended to facilitate the fine-tuning of behaviors.
*/
/*
class ProbabilitySelectorNode:public BehaviorTreeInternalNode
{
private:
double totalSum;
std::map<BehaviorTreeNode*,double>::iterator currentlyRunningNode;
std::map<BehaviorTreeNode*,double> weightingMap;
std::map<BehaviorTreeNode*,double>::iterator itr;
public:
void init(void* object);
BEHAVIOR_STATUS execute(void* object);
///Add a child to this node, with the given weight.
void addChild( BehaviorTreeNode* node,double weighting);
///Add a child to this node, with a weight of one.
void addChild( BehaviorTreeNode* node);
};
*/
}