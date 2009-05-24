#include "goStraight.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

using namespace SL;
using namespace BehaviorTree;

//goStraight will never return SUCCESS!!! Remember to provide decorators or conditions so it will fail.

void goStraight::init(void* agent)
{
};
BehaviorTree::BEHAVIOR_STATUS goStraight::execute(void* agent)
{
	GameObject* ship = (GameObject*) agent;
#ifdef PHYSICS
	ship->applyForcePolar(ship->displayHeading,.005);
#else
	ws->moveObject(ship,ship->location.offsetPolar(ship->displayHeading,1));
#endif
	return BT_RUNNING;
}