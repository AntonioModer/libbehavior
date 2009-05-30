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
	
	return BT_RUNNING;
}