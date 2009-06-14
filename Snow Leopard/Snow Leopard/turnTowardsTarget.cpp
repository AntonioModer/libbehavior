#include "turnTowardsTarget.h"
#include "point.h"
#include "globals.h"
#include "util.h"

using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;

void TurnTowardsTarget::init(void* agent)
{
};

TurnTowardsTarget::TurnTowardsTarget(float _speed)
{
	speed = _speed;
}
BEHAVIOR_STATUS TurnTowardsTarget::execute(void* agent)
{
	GameObject* object = (GameObject*) agent;
	GameObject* target = ws->getPlayer();
	point resultantVector(object->location.x - target->location.x,object->location.y - target->location.y);
	resultantVector.normalize();
	CL_Angle angle = CL_Angle::from_radians(atan2(resultantVector.y,resultantVector.x));
	CL_Angle angleBetween = angle - object->displayHeading + CL_Angle::from_degrees(-90.0f);
	angleBetween = CL_Angle::from_degrees(mod(angleBetween.to_degrees(),360.0f)); 
	if (angleBetween > CL_Angle::from_degrees(180.0f))
		angleBetween = CL_Angle::from_degrees(180.0f) - angleBetween;
	cout << "angleBetween" << angleBetween.to_degrees() << endl;

	if ( angleBetween.to_degrees() < -(speed+1))
	{
		ws->rotateObject(object,-speed); //clockwise
		return BT_RUNNING;
	}
	else if (angleBetween.to_degrees() > speed+1)
	{
		ws->rotateObject(object,speed);
		return BT_RUNNING;
	}

	else return BT_RUNNING;
	
}