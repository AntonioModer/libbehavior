#include "BehaviorTree.h"
#include "mtrand.h"

using namespace BehaviorTree;
using namespace std;
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

ParallelNode::ParallelNode(FAILURE_POLICY failurePolicy, SUCCESS_POLICY successPolicy)
{
	failPolicy = failurePolicy;
	succeedPolicy = successPolicy;
	childrenStatus = NULL;
}
void ParallelNode::init(void* agent)
{
	for (BehaviorTreeListIter iter = children.begin(); iter!= children.end(); iter++)
				(*iter)->init(agent);
	
	if (childrenStatus != NULL)
		delete childrenStatus;
	childrenStatus = new ChildrenStatusMap();
	for (int i = 0 ; i<children.size(); i++)
		childrenStatus->insert( make_pair(children.at(i),BT_RUNNING));
}

BEHAVIOR_STATUS ParallelNode::execute(void* agent)
{
	if (childrenStatus == NULL)
		init(agent);
	// go through all children and update the childrenStatus
	for (int i = 0 ; i<children.size(); i++)
	{
		BehaviorTreeNode* node = children[i];
		if ((*childrenStatus)[node] == BT_RUNNING)
		{
			BEHAVIOR_STATUS status = node->execute(agent);
			if (status == BT_FAILURE)
			{
				if (failPolicy == FAIL_ON_ONE)
				{
					init(agent);
					return BT_FAILURE;
				}
				else
				{
					(*childrenStatus)[node] = BT_FAILURE;
				}
			}
			if (status == BT_SUCCESS)
				(*childrenStatus)[node] = BT_SUCCESS;
		}
	}

	//look through the childrenStatus and see if we have met any of our end conditions
	ChildrenStatusMap::iterator iter;
	bool sawSuccess = false;
	bool sawAllFails = true;
	bool sawAllSuccess = true;
	for (iter = childrenStatus->begin(); iter != childrenStatus->end() ; iter++)
	{
		switch((*iter).second)
		{
		case BT_SUCCESS:
			//can't instantly return success for succeedOnOne policy if failOnOne is also true, because failOnOne overrides succeedOnOne
			if (succeedPolicy == SUCCEED_ON_ONE && failPolicy != FAIL_ON_ONE)
			{
				init(agent);
				return BT_SUCCESS;
			}
			sawSuccess = true;
			sawAllFails = false;
			break;
		case BT_FAILURE:
			if (failPolicy == FAIL_ON_ONE)
			{
			init(agent);
				return BT_FAILURE;
			}
			sawAllSuccess = false;
			break;
		case BT_RUNNING:
			sawAllFails = false;
			sawAllSuccess = false;
			//optimization for early exit
			if (failPolicy == FAIL_ON_ALL && succeedPolicy == SUCCEED_ON_ALL)
			{
				return BT_RUNNING;
			}
			break;
		}
	}
	if (failPolicy == FAIL_ON_ALL && sawAllFails)
	{
		init(agent);
		return BT_FAILURE;
	}
	else if (succeedPolicy == SUCCEED_ON_ALL && sawAllSuccess || succeedPolicy == SUCCEED_ON_ONE && sawSuccess)
	{
		init(agent);
		return BT_SUCCESS;
	}
	else
	{
		return BT_RUNNING;
	}
}