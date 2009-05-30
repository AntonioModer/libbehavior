#ifndef ABSOLUTEMOVEMENT_H_
#define ABSOLUTEMOVEMENT_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	enum ABSOLUTE_DIRECTION{UP,DOWN,LEFT,RIGHT};
	namespace Behaviors
	{
	/// Simple behavior if moving in a given direction with a constant speed
	class AbsoluteMovement:public BehaviorTree::BehaviorTreeNode
	{
	public:
		int speed;
		ABSOLUTE_DIRECTION direction;
		BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		AbsoluteMovement::AbsoluteMovement(ABSOLUTE_DIRECTION direction,int speed);
		BehaviorTree::BehaviorTreeList getChildren()
		{
			return BehaviorTree::BehaviorTreeList();
		}
	};
	}

}
#endif