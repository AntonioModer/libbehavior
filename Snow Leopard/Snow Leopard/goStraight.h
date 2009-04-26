#ifndef GOSTRAIGHT_H_
#define GOSTRAIGHT_H_
#include "BehaviorTreeNode.h"

namespace SL
{
	class GameObject;
class goStraight:public BehaviorTreeNode
{
	BEHAVIOR_STATUS execute(GameObject* object);
	bool init(GameObject* object);
};

}
#endif