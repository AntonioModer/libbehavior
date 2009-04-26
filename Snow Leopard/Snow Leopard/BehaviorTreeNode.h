#ifndef BEHAVIORTREENODE_H_
#define BEHAVIORTREENODE_H_

#include <vector>
#include <map>
namespace SL
{

class GameObject;

class BehaviorTreeNode
{
public:
	typedef std::vector<BehaviorTreeNode*> BehaviorTreeList;
	typedef BehaviorTreeList::iterator BehaviorTreeListIter;
	enum BEHAVIOR_STATUS {SL_SUCCESS,SL_FAILURE,SL_ERROR,SL_RUNNING};
	virtual BEHAVIOR_STATUS execute(GameObject* object) = 0;
	virtual bool init(GameObject* object) = 0;
	BehaviorTreeNode* deepCopy();
};


class BehaviorTreeInternalNode:public BehaviorTreeNode
{

public:
		virtual BEHAVIOR_STATUS execute(GameObject* object) = 0;
	virtual bool init(GameObject* object) = 0;
	bool BehaviorTreeInternalNode::addChild(BehaviorTreeNode* newChild)
	{
		children.push_back(newChild);
		return true;

	};
	
protected:
	BehaviorTreeList children;
	
	
};


class SequentialNode:public BehaviorTreeInternalNode
{
	BEHAVIOR_STATUS execute(GameObject* object);
	bool init(GameObject* object);

	private: int currentPosition;
};

class PrioritySelectorNode:public BehaviorTreeInternalNode
{
	BEHAVIOR_STATUS execute(GameObject* object);

	BehaviorTreeListIter currentlySL_RUNNINGNode;

};

class ProbabilitySelectorNode:public BehaviorTreeInternalNode
{
private:
	double totalSum;
	std::map<SL::BehaviorTreeNode*,double>::iterator currentlySL_RUNNINGNode;
	std::map<SL::BehaviorTreeNode*,double> weightingMap;
	std::map<SL::BehaviorTreeNode*,double>::iterator itr;
public:
	bool init(GameObject* object);
	BEHAVIOR_STATUS execute(GameObject* object);
	bool addChild( BehaviorTreeNode* node,double weighting);
};


class ParallelNode:public BehaviorTreeInternalNode
{
private:
	std::map<SL::BehaviorTreeNode*,bool> childrenStatus;
	std::map<SL::BehaviorTreeNode*,bool>::iterator childrenStatusIterator;
public:
	BEHAVIOR_STATUS execute(GameObject* object);
	bool init(GameObject* object);
};

}
#endif