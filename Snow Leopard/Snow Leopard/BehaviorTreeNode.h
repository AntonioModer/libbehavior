#ifndef BEHAVIORTREENODE_H_
#define BEHAVIORTREENODE_H_

#include <vector>
#include <map>
namespace BehaviorTree
{
///Abstract base clase for Behavior Tree Nodes
class BehaviorTreeNode
{
public:
	/// A standard vector of Behavior Tree nodes. Provided for convenience.
	typedef std::vector<BehaviorTreeNode*> BehaviorTreeList;
	/// A standard iterator of a BehaviorTreeList. Provided for convenience.
	typedef BehaviorTreeList::iterator BehaviorTreeListIter;
	/// Enumerates the states every node can be in after execution during a particular time step
	/**
	- BT_SUCCESS indicates the node has completed running during this time step.
	- BT_FAILURE indicates the node has determined it will not be able to complete its task.
	- BT_ERROR is rarely used. It indicates that error handling outside of the normal behavior tree chain of execution should be invoked.
	- BT_RUNNING indicates that the node has successfully moved forward during this time step, but the task is not yet complete.
	*/
	enum BEHAVIOR_STATUS {BT_SUCCESS,BT_FAILURE,BT_ERROR,BT_RUNNING};
	/// This method is invoked by the node's parent when the node should be run.
	virtual BEHAVIOR_STATUS execute(void* agent) = 0;
	/// This method will be invoked before the node is executed for the first time.
	virtual void init(void* agent) = 0;
	/// Retrieve a BehaviorTreeList of the node's children. If the node has no children, the list will be empty.
	virtual BehaviorTreeList getChildren() = 0;
};

/// Abstract base class for Behavior Tree nodes with children
class BehaviorTreeInternalNode:public BehaviorTreeNode
{

public:
	virtual BEHAVIOR_STATUS execute(void* agent) = 0;
	virtual void init(void* object) = 0;
	/// Add a child to this node. Takes ownership of the child.
	virtual void BehaviorTreeInternalNode::addChild(BehaviorTreeNode* newChild)
	{
		children.push_back(newChild);
	};
	virtual BehaviorTreeList getChildren()
	{
		return children;
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
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);

	protected: int currentPosition;
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
	BEHAVIOR_STATUS execute(void* agent);

	BehaviorTreeListIter currentlyRunningNode;

};
///Executes behaviors randomly, based on a given set of weights.
/** The weights are not percentages, but rather simple ratios.
	For example, if there were two children with a weight of one, each would have a 50% chance of being executed.
	If another child with a weight of eight were added, the previous children would have a 10% chance of being executed, and the new child would have an 80% chance of being executed.
	This weight system is intended to facilitate the fine-tuning of behaviors.
*/
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

///Execute behaviors in parallel
/** There are two policies that control the flow of execution. The first is the policy for failure, and the second is the policy for success.
	For failure, the options are "fail when one child fails" and "fail when all children fail".
	For success, the options are similarly "complete when one child completes", and "complete when all children complete".
*/
class ParallelNode:public BehaviorTreeInternalNode
{
private:
	std::map<BehaviorTreeNode*,bool> childrenStatus;
	std::map<BehaviorTreeNode*,bool>::iterator childrenStatusIterator;
public:
	enum FAILURE_POLICY {FAIL_ON_ONE,FAIL_ON_ALL};
	enum SUCCESS_POLICY {SUCCEED_ON_ONE,SUCCEED_ON_ALL};
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* agent);
	void setFailurePolicy(FAILURE_POLICY policy);
	void setSuccessPolicy(SUCCESS_POLICY policy);
};
///A decorator that repeats its child a specified number of times.
class RepeatNode: public BehaviorTreeInternalNode
{
public:
	/// Specify the number of times to repeat the child before returning success.
	void setNumberOfRepeats(unsigned int repeats)
	{
		target = repeats;
	};
	BEHAVIOR_STATUS execute(void* agent);
	void init(void* object)
	{
		count = 0;
	}
protected:
	int count;
	int target;

};
}
#endif