#include "BehaviorTree.h"
using namespace BehaviorTree;
using namespace std;

/*
void ProbabilitySelectorNode::init(void* agent)
{
totalSum = 0;
}
void ProbabilitySelectorNode::addChild(BehaviorTreeNode* node, double weighting)
{
weightingMap[node] = weighting;
totalSum += weighting;
BehaviorTreeInternalNode::children.push_back(node);
}

BEHAVIOR_STATUS ProbabilitySelectorNode::execute(void* agent)
{
//check if we've already chosen a node to run
if (&currentlyRunningNode)
{
BEHAVIOR_STATUS status = ((*currentlyRunningNode).first)->execute(agent);
return status;
}


//move the generator to gamelogic and have a method that returns a pointer to it
MTRand_closed random;
double chosen = random() * totalSum; //generate a number between 0 and the sum of the weights

double sum = 0;
for (itr = weightingMap.begin() ; itr!=weightingMap.end() ; itr++)
{
sum += (*itr).second;
if (sum >= chosen) //execute this node
{
BEHAVIOR_STATUS status = (*itr).first->execute(agent);

if (status == BT_RUNNING)
currentlyRunningNode = itr;
return status;
}
}



return BT_SUCCESS;
}
*/