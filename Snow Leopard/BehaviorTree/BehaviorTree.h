#ifndef BEHAVIORTREENODE_H_
#define BEHAVIORTREENODE_H_

#include <vector>
#include <map>
namespace BehaviorTree
{
	/// Enumerates the states every node can be in after execution during a particular time step
	/**
	- BT_SUCCESS indicates the node has completed running during this time step.
	- BT_FAILURE indicates the node has determined it will not be able to complete its task.
	- BT_RUNNING indicates that the node has successfully moved forward during this time step, but the task is not yet complete.
	*/
	enum BEHAVIOR_STATUS {BT_SUCCESS,BT_FAILURE,BT_RUNNING};
	class BehaviorTreeNode;
	/// A standard vector of Behavior Tree nodes. Provided for convenience.
	typedef std::vector<BehaviorTreeNode*> BehaviorTreeList;
	/// A standard iterator of a BehaviorTreeList. Provided for convenience.
	typedef BehaviorTreeList::iterator BehaviorTreeListIter;

	enum FAILURE_POLICY {FAIL_ON_ONE,FAIL_ON_ALL};
	enum SUCCESS_POLICY {SUCCEED_ON_ONE,SUCCEED_ON_ALL};


///Abstract base clase for Behavior Tree Nodes
class BehaviorTreeNode
{
public:
	/// This method is invoked by the node's parent when the node should be run.
	virtual BEHAVIOR_STATUS execute(void* agent) = 0;
	/// This method will be invoked before the node is executed for the first time.
	virtual void init(void* agent) = 0;
};

/// Abstract base class for Behavior Tree nodes with children
class BehaviorTreeInternalNode:public BehaviorTreeNode
{

public:
	virtual BEHAVIOR_STATUS execute(void* agent) = 0;
	virtual void init(void* object) = 0;
	/// Add a child to this node. Takes ownership of the child.
	virtual BehaviorTreeInternalNode* BehaviorTreeInternalNode::addChild(BehaviorTreeNode* newChild)
	{
		children.push_back(newChild);
		return this;
	};
	
protected:
	BehaviorTreeList children;
	~BehaviorTreeInternalNode()
	{
		BehaviorTreeListIter iter;
		for (iter = children.begin(); iter!= children.end(); iter++)
		{
			delete *iter;
		}
	}
	
	
};

///Executes behaviors in order
/** Executes its children in the order they were added.
	If the currently executing child returns BT_FAILURE, BT_ERROR, or BT_RUNNING, this returns the same status.
	If the currently executing child returns BT_SUCCESS, this begins executing the next child, if it exists. It continues in this fashion until one of the children returns BT_FAILURE, BT_ERROR, or BT_RUNNING. If all children have successfully executed, it will return BT_SUCCESS.
*/
class SequentialNode:public BehaviorTreeInternalNode
{
public:
	SequentialNode::SequentialNode();
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);

    int currentPosition;
};
///Executes behaviors in priority order until one of them is successful.
/** Attempts to execute children in the order they were added. 
	- If a child returns BT_FAILURE, begin executing the next child if there is one, else return BT_FAILURE.
	- If a child returns BT_ERROR, return BT_ERROR.
	- If a child returns BT_SUCCESS, return BT_SUCCESS.
	- If a child returns BT_RUNNING, return BT_RUNNING.
*/
class PrioritySelectorNode:public BehaviorTreeInternalNode
{
public:
	BEHAVIOR_STATUS execute(void* agent);
	int currentPosition;
	PrioritySelectorNode::PrioritySelectorNode();
	void init(void* agent);

	BehaviorTreeListIter currentlyRunningNode;

};
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
///Execute behaviors in parallel
/** There are two policies that control the flow of execution. The first is the policy for failure, and the second is the policy for success.
	For failure, the options are "fail when one child fails" and "fail when all children fail".
	For success, the options are similarly "complete when one child completes", and "complete when all children complete".
*/
class ParallelNode:public BehaviorTreeInternalNode
{
public:
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
	void setFailurePolicy(FAILURE_POLICY policy);
	void setSuccessPolicy(SUCCESS_POLICY policy);
	FAILURE_POLICY getFailurePolicy()
	{
		return failPolicy;
	};
	SUCCESS_POLICY getSuccessPolicy()
	{
		return succeedPolicy;
	};
	ParallelNode::ParallelNode( FAILURE_POLICY failurePolicy = FAIL_ON_ALL, SUCCESS_POLICY SuccessPolicy = SUCCEED_ON_ALL);

private:
	typedef std::map<BehaviorTreeNode*,BEHAVIOR_STATUS> ChildrenStatusMap;
	ChildrenStatusMap* childrenStatus;
	FAILURE_POLICY failPolicy;
	SUCCESS_POLICY succeedPolicy;
};
///A node that repeats its child a specified number of times.
class RepeatNode: public BehaviorTreeInternalNode
{
public:
	RepeatNode::RepeatNode(int repeats)
	{
		target = repeats;
		count = 0;
	};
	BehaviorTreeInternalNode* addChild(BehaviorTreeNode* newChild)
	{
		if (children.size() == 0)
			BehaviorTreeInternalNode::addChild(newChild);
		else
			throw new std::exception("Cannot add more than one child to a repeat node");
		
		return this;
	};
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent)
	{
		count = 0;
		if (children.size() == 1)
			children.at(0)->init(agent);
	};
protected:
	int count;
	int target;

};

class AlwaysRunning: public BehaviorTreeNode
{
	BEHAVIOR_STATUS execute(void* agent)
	{
		return BT_RUNNING;
	}
	void init(void* agent){};
};

class AlwaysSuccess: public BehaviorTreeNode
{
	BEHAVIOR_STATUS execute(void* agent)
	{
		return BT_SUCCESS;
	}
	void init(void* agent){};
};

class AlwaysFailure: public BehaviorTreeNode
{
	BEHAVIOR_STATUS execute(void* agent)
	{
		return BT_FAILURE;
	}
	void init(void* agent){};
};

class SuccessAfter: public BehaviorTreeNode
{
public:
	int n;
	int total;
	BEHAVIOR_STATUS execute(void* agent)
	{
		if (n == 0)
		{
			return BT_SUCCESS;
		}
		else
		{
			n--;
			return BT_RUNNING;
		}
	}
	void init(void* agent)
	{
		n = total;
	};
	SuccessAfter::SuccessAfter(int t)
	{
		total = t;
		n = total;
	}
};

class FailureAfter: public BehaviorTreeNode
{
public:
	int n;
	int total;
	BEHAVIOR_STATUS execute(void* agent)
	{
		if (n == 0)
		{
			return BT_FAILURE;
		}
		else
		{
			n--;
			return BT_RUNNING;
		}
	}
	void init(void* agent)
	{
		n = total;
	};
	FailureAfter::FailureAfter(int t)
	{
		total = t;
		n = total;
	}
};
}
#endif