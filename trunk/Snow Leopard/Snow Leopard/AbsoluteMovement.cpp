#include "AbsoluteMovement.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;

//goStraight will never return SUCCESS!!! Remember to provide decorators or conditions so it will fail.

void AbsoluteMovement::init(void* agent)
{
};

AbsoluteMovement::AbsoluteMovement(ABSOLUTE_DIRECTION _direction,int _speed)
{
	direction = _direction;
	speed = _speed;
}
BEHAVIOR_STATUS AbsoluteMovement::execute(void* agent)
{
	GameObject* ship = (GameObject*) agent;
#ifdef PHYSICS
	ship->applyForcePolar(ship->displayHeading,.005);
#else
	switch(direction)
	{
	case UP:
		ws->moveObject(ship,ship->location.offsetRect(0,-speed));
		break;
	case DOWN:
		ws->moveObject(ship,ship->location.offsetRect(0,speed));
		break;
	case LEFT:
		ws->moveObject(ship,ship->location.offsetRect(-speed,0));
		break;
	case RIGHT:
		ws->moveObject(ship,ship->location.offsetRect(speed,0));
		break;
	}
	
#endif
	return BT_RUNNING;
}