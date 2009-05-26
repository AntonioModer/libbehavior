#include "BehaviorTree.h"
#include "mtrand.h"

using namespace BehaviorTree;
void SequentialNode::init(void* agent)
{
	currentPosition = -1;
	for (BehaviorTreeListIter iter = children.begin(); iter!= children.end(); iter++)
				(*iter)->init(agent);
}

SequentialNode::SequentialNode()
{
	currentPosition = -1;
}

BEHAVIOR_STATUS SequentialNode::execute(void* agent)
	{
		if (currentPosition == -1) //starting out
		{
			init(agent);
			currentPosition = 0;
		}

		BehaviorTreeNode* currentTask = children.at(currentPosition);
		BEHAVIOR_STATUS result = currentTask->execute(agent);

		while(result == BT_SUCCESS)
		{
			if (currentPosition == children.size()-1) //finished last task
			{
				currentPosition = -1; //indicate we are not running anything
				return BT_SUCCESS;
			}
			else
			{
				currentPosition++;
				currentTask = children.at(currentPosition);
				result = currentTask->execute(agent);
			}
		}
		if (result == BT_FAILURE)
			currentPosition = -1;
		return result;
	}

PrioritySelectorNode::PrioritySelectorNode()
{
	currentPosition = -1;
}

void PrioritySelectorNode::init(void* agent)
{
	currentPosition = -1;
	for (BehaviorTreeListIter iter = children.begin(); iter!= children.end(); iter++)
				(*iter)->init(agent);
}


BEHAVIOR_STATUS PrioritySelectorNode::execute(void* agent)
{
	if (currentPosition != -1) //there's one still running
	{
		BEHAVIOR_STATUS status = (children.at(currentPosition))->execute(agent);
		if (status == BT_RUNNING)
			return BT_RUNNING;
		else if (status == BT_SUCCESS)
		{
			currentPosition = -1;
			return BT_SUCCESS;
		}
		else if (status == BT_FAILURE)
		{
			currentPosition++;
			if (currentPosition == children.size())
			{
				currentPosition = -1;
				return BT_FAILURE;
			}
		}
	}
	else
	{
		init(agent);
		currentPosition = 0;
	}

	BehaviorTreeNode* currentlyRunningNode = children.at(currentPosition);
	BEHAVIOR_STATUS status;
	while ((status = (*currentlyRunningNode).execute(agent)) == BT_FAILURE) //keep trying children until one doesn't fail
	{
		currentPosition++;
		if (currentPosition == children.size()) //all of the children failed
		{
			currentPosition = -1;
			return BT_FAILURE;
		}
	}
	return status;

}

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

void ParallelNode::init(void* object)
{
}

BEHAVIOR_STATUS ParallelNode::execute(void* agent)
{
	BehaviorTreeListIter itr;
	for (itr = children.begin() ; itr != children.end() ; itr++)
	{
		BEHAVIOR_STATUS status;
		if (!(childrenStatus[*itr]))
		{
			BEHAVIOR_STATUS status = (*itr)->execute(agent);
			if (status == BT_FAILURE)
				return status;
			if (status == BT_SUCCESS)
				childrenStatus[*itr] = true;
		}
		
			
			bool succeeded = true;
		for (childrenStatusIterator = childrenStatus.begin();childrenStatusIterator != childrenStatus.end() ; childrenStatusIterator++)
		{
			if ((*childrenStatusIterator).second)
				succeeded = false;
		}
	}
}