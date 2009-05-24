#include "turnTowardsTarget.h"
#include "point.h"
#include "globals.h"

using namespace SL;
using namespace BehaviorTree;

void turnTowardsTarget::init(void* agent)
{
};
BEHAVIOR_STATUS turnTowardsTarget::execute(void* agent)
{
	GameObject* object = (GameObject*) agent;
	GameObject* target = ws->getCamera();
	cout << "found target" << endl;
	cout << target->location.toString() << endl;
	cout << "my location: " << object->location.toString() << endl;
	point resultantVector(object->location.x - target->location.x,object->location.y - target->location.y);
	cout <<"resultant vector" << endl;
	cout << resultantVector.toString() << endl;
	resultantVector.normalize();
	cout << "normalized resultant vector" << endl;
	cout << resultantVector.toString() << endl;
	
	CL_Angle angle = CL_Angle::from_radians(atan2(resultantVector.y,resultantVector.x));

	cout << "ANGLE: " << angle.to_degrees() << endl ;

	CL_Angle angleBetween = angle - object->displayHeading + CL_Angle::from_degrees(-90.0f);
	angleBetween = CL_Angle::from_degrees(fmod(angleBetween.to_degrees(),360.0f)); 
	cout << "Angle Between" << endl;
	cout << angleBetween.to_degrees() << endl;
	
	
	if ( angleBetween.to_degrees() < -2)
	{
		ws->rotateObject(object,-1); //clockwise
		return BT_RUNNING;
	}
	else if (angleBetween.to_degrees() > 2)
	{
		ws->rotateObject(object,1);
		return BT_RUNNING;
	}

	else return BT_RUNNING;
	
}