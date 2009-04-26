#ifndef TURNTOWARDSTARGET_H_
#define TURNTOWARDSTARGET_H_
#include "BehaviorTreeNode.h"

namespace SL
{
	class GameObject;
class turnTowardsTarget:public BehaviorTreeNode
{
	BEHAVIOR_STATUS execute(GameObject* object);
	bool init(GameObject* object);
};

}
#endif