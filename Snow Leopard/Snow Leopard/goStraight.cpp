#include "goStraight.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

using namespace SL;

//goStraight will never return SUCCESS!!! Remember to provide decorators or conditions so it will fail.

bool goStraight::init(SL::GameObject *object)
{
	return true;
};
BehaviorTreeNode::BEHAVIOR_STATUS goStraight::execute(GameObject* object)
{
	object->applyForcePolar(object->displayHeading,.005);
	return SL_RUNNING;
}