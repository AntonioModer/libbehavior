#include "BehaviorTreeNode.h"
#include "mtrand.h"

using namespace SL;

BehaviorTreeNode::BEHAVIOR_STATUS SequentialNode::execute(GameObject* object)
	{
		BehaviorTreeNode* currentTask = children.at(currentPosition);
		BEHAVIOR_STATUS result = currentTask->execute(object);

		if (result == SL_SUCCESS)
		{
			if (currentPosition == children.size()) //finished last task
			{
				return SL_SUCCESS;
			}
			else
			{
				currentPosition++;
				return SL_RUNNING;
			}
		}
		return result;
	}

bool SequentialNode::init(GameObject* object)
{
	currentPosition = 0;
	return true;
}
BehaviorTreeNode::BEHAVIOR_STATUS PrioritySelectorNode::execute(GameObject* object)
{
	if (*currentlySL_RUNNINGNode) //there's one still SL_RUNNING
	{
		BEHAVIOR_STATUS status = (*currentlySL_RUNNINGNode)->execute(object);
		if (status == SL_RUNNING)
			return SL_RUNNING;
		else if (status == SL_SUCCESS)
		{
			delete &currentlySL_RUNNINGNode;
			return SL_SUCCESS;
		}
	}

	else //need to choose one
	{
		currentlySL_RUNNINGNode = children.begin();
		BEHAVIOR_STATUS status;
        while ((status = (*currentlySL_RUNNINGNode)->execute(object)) == SL_FAILURE) //keep trying children until one doesn't fail
        {
			currentlySL_RUNNINGNode++;
            if (currentlySL_RUNNINGNode == children.end()) //all of the children failed
                return SL_FAILURE;
        }
		if (status == SL_SUCCESS)
			delete &currentlySL_RUNNINGNode;
        return status;
	}

}

bool ProbabilitySelectorNode::init(GameObject* object)
{
	totalSum = 0;
	return true;
}
bool ProbabilitySelectorNode::addChild(BehaviorTreeNode* node, double weighting)
{
	weightingMap[node] = weighting;
	totalSum += weighting;
	BehaviorTreeInternalNode::children.push_back(node);
	return true;
}

BehaviorTreeNode::BEHAVIOR_STATUS ProbabilitySelectorNode::execute(GameObject *object)
{
	//check if we've already chosen a node to run
	if (&currentlySL_RUNNINGNode)
	{
		BEHAVIOR_STATUS status = ((*currentlySL_RUNNINGNode).first)->execute(object);
		if (status != SL_RUNNING)
			delete &currentlySL_RUNNINGNode;
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
			BEHAVIOR_STATUS status = (*itr).first->execute(object);

			if (status == SL_RUNNING)
				currentlySL_RUNNINGNode = itr;
			return status;
		}
	}

	

	return SL_SUCCESS;
}

bool ParallelNode::init(GameObject* object)
{
	return true;
}

BehaviorTreeNode::BEHAVIOR_STATUS ParallelNode::execute(GameObject* object)
{
	BehaviorTreeListIter itr;
	for (itr = children.begin() ; itr != children.end() ; itr++)
	{
		BEHAVIOR_STATUS status;
		if (!(childrenStatus[*itr]))
		{
			BEHAVIOR_STATUS status = (*itr)->execute(object);
			if (status == SL_FAILURE || status == SL_ERROR)
				return status;
			if (status == SL_SUCCESS)
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