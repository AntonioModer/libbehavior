#ifndef SPAWNSHIP_H_
#define SPAWNSHIP_H_
#include "BehaviorTree.h"

using namespace BehaviorTree;
namespace SL
{
	namespace Behaviors
	{
	/// Destroy all projectiles within a 200 unit radius of the agent
	class SpawnShip:public BehaviorTree::BehaviorTreeNode
	{
	public:
		BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
		void init(void* agent);
		SpawnShip::SpawnShip();
		const BehaviorTree::BehaviorTreeList* getChildren() const
		{
			return NULL;
		}
	};
	}

}
#endif