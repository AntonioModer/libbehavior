#include "turnTowardsTarget.h"
#include "point.h"
#include "globals.h"

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
	const GameObjectList* allObjects = ws->getAllGameObjects(WorldState::ACTION_SORTED);
	
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
	
	CL_Angle angle = CL_Angle::from_radians(atan2(resultantVector.y,resultantVector.x));

	std::cout << angle.to_degrees() << "\n" ;
	
	double angleBetween = ( mod( ((object->displayHeading.to_degrees() -180 - angle.to_degrees()) + 180),360) - 180);
	if ( angleBetween < -2)
	{
		ws->rotateObject(object,1); //clockwise
		return SL_RUNNING;
	}
	else if (angleBetween > 2)
	{
		ws->rotateObject(object,-1);
		return SL_RUNNING;
	}

	else return SL_SUCCESS;

}