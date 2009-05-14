#include "turnTowardsTarget.h"
#include "point.h"
#include "globals.h"

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
	GameObject* target = ws->getCamera();
	object->targetPriorities->push_back(target);
	cout << "found target" << endl;
	cout << target->location.toString() << endl;
	cout << "my location: " << object->location.toString() << endl;
	point resultantVector(object->location.x - target->location.x,object->location.y - target->location.y);
	cout << resultantVector.toString() << endl;
	resultantVector.normalize();
	cout << resultantVector.toString() << endl;
	
	CL_Angle angle = CL_Angle::from_radians(atan2(resultantVector.y,resultantVector.x));

	cout << "ANGLE: " << angle.to_degrees() << endl ;
	
	CL_Angle angleBetween = angle - target->displayHeading;
	if ( angleBetween.to_degrees() < -2)
	{
		ws->rotateObject(object,1); //clockwise
		return SL_RUNNING;
	}
	else if (angleBetween.to_degrees() > 2)
	{
		ws->rotateObject(object,-1);
		return SL_RUNNING;
	}

	else return SL_SUCCESS;

}