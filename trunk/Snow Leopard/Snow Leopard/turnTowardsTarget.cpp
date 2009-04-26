#include "turnTowardsTarget.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

using namespace SL;

double mod (double toMod, int op)
{
	double intpart;
	toMod = modf(toMod,&intpart);
	toMod = toMod + (int)intpart % op;
	return toMod;
}

bool turnTowardsTarget::init(SL::GameObject *object)
{
	return true;
};
BehaviorTreeNode::BEHAVIOR_STATUS turnTowardsTarget::execute(GameObject* object)
{
	if (!(object->ID == ("enemy")))
		return SL_FAILURE;
	const GameObjectList* allObjects = object->worldState->getAllGameObjects(WorldState::ACTION_SORTED);
	
	ConstGameObjectIter itr;
	GameObject* target;
	for (itr = allObjects->begin(); itr!=allObjects->end() ; itr++)
	{
		if ((*itr)->ID == "firefly")
			target = (*itr);
	}
	object->targetPriorities->push_back(target);

	point resultantVector(object->location.x - target->location.x,object->location.y - target->location.y);
	resultantVector.normalize();
	
	double angle = 57.2957795 * atan2(resultantVector.y,resultantVector.x); //the 57 is from radian/degree conversion

	std::cout << angle << "\n" ;
	
	double angleBetween = ( mod( ((object->displayHeading -180 - angle) + 180),360) - 180);

	if ( angleBetween < -2)
	{
		object->rotate(1); //clockwise
		return SL_RUNNING;
	}
	else if (angleBetween > 2)
	{
		object->rotate(-1);
		return SL_RUNNING;
	}

	else return SL_SUCCESS;

}