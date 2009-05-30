#ifndef FIRE_H_
#define FIRE_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
	/// Fire your weapon
	class Fire:public BehaviorTree::BehaviorTreeNode
	{
	public:
		BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		Fire::Fire();
		BehaviorTree::BehaviorTreeList getChildren()
		{
			return BehaviorTree::BehaviorTreeList();
		}
	};
	}

}
#endif