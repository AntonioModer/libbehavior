#include "GameObject.h"
#include "WorldState.h"
#include "turnTowardsTarget.h"
#include "goStraight.h"
#include "BehaviorTree.h"
#include "globals.h"

using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;

int GameObject::IDCount = 0;

GameObject::GameObject()
{
	HP = 100;
	displayName = "default";
	displayHeading = CL_Angle();
	faction = 0;
	ID = getID();
	speed=0;
	movementHeading=CL_Angle::from_degrees(0);
	accelMagnitude = 0;
	accelHeading = 0;
	isPlayer = false;
	usesPhysics = true;

	//set up the base sprite
	CL_PixelBuffer image(100,100,5,CL_PixelFormat());
	//CL_PixelBuffer image = CL_PNGProvider::load(CL_String("Resources\\Ammo\\test.png"));
	CL_SpriteDescription desc;
	desc.add_frame(image);
	sprite = new CL_Sprite(*SL::gc,desc);
	
	CL_Origin translationOrigin = origin_center;
	CL_Origin rotationOrigin = origin_center;
	int translation_offset_x =0;
	int translation_offset_y = 0;
	int rotation_offset_x = 0;
	int rotation_offset_y = 0;
	sprite->set_alignment(translationOrigin,translation_offset_x,translation_offset_y);
	sprite->set_rotation_hotspot(rotationOrigin,rotation_offset_x,rotation_offset_y);
	sprite->set_base_angle(CL_Angle::from_degrees(0));

	displayHeading =CL_Angle::from_degrees(0);

	// will only need a collision outline if it's going to be colliding with stuff
	if (usesPhysics)
	{
	cout << "creating a new collision outline" << endl;
	collisionOutline = new CL_CollisionOutline(image);
	collisionOutline->set_alignment(translationOrigin,(float)translation_offset_x,(float)translation_offset_y);
	collisionOutline->set_rotation_hotspot(rotationOrigin,(float)rotation_offset_x,(float)rotation_offset_y);
	collisionOutline->set_angle(displayHeading);
	}

	brain = new ParallelNode();
};


bool GameObject::doActions()
{
#ifdef PHYSICS
	if (usesPhysics)
		processMovementPhysics();
#endif
	if (!(isPlayer))
		brain->execute(this);
	return true;
}

bool GameObject::registerCollision(GameObject* collidedObject)
{
	std::cout << displayName <<  " Collided with " << collidedObject->displayName << "\n";
	std::cout << "MyPosition: " << collisionOutline->get_translation()<< "\n";
	std::cout << "TheirPosition: " << collidedObject->collisionOutline->get_translation()<< "\n";
	return true;
}

void GameObject::setSprite(std::string resource)
{
	CL_PixelBuffer image = CL_PNGProvider::load(CL_String("Resources\\" + resource));
	CL_SpriteDescription desc;
	desc.add_frame(image);
	delete sprite;
	sprite = new CL_Sprite(*SL::gc,desc);

	if (usesPhysics)
	{
		cout << "creating a new collision outline" << endl;
		collisionOutline = new CL_CollisionOutline(image);
		collisionOutline->set_alignment(origin_center,0,0);
		collisionOutline->set_rotation_hotspot(origin_center,0,0);
		collisionOutline->set_angle(displayHeading);
	}

}

bool GameObject::processMovementPhysics()
{
	//new velocity components
	float newX = .5 * accelMagnitude * cos(accelHeading * 3.14159/180) * ws->timeElapsed
		+ speed * cos(movementHeading.to_radians());
	float newY = .5 * accelMagnitude * sin(accelHeading *3.14159/180) * ws->timeElapsed
		+ speed * sin(movementHeading.to_radians()); 

	speed = sqrt((newX)*(newX) + (newY)*(newY));
	movementHeading = CL_Angle::from_radians(atan2(newY,newX));

	/*std::cout << this->displayName << "Physics Update: \n";
	std::cout << "Velocity Heading: " << this->heading << "\n";
	std::cout << "Speed: " << this->speed << "\n";
	std::cout << "AccelHeading: " << this->accelHeading << "\n";
	std::cout << "AccelMagnitude: " << this->accelMagnitude << "\n\n";*/

	ws->moveObject(this,location.offsetPolar(movementHeading,speed * ws->timeElapsed));
	
	accelHeading = 0; //should allow applied forces to have durations. Until then, everything gets re-composited every tick
	accelMagnitude = 0;
	return true;

}

bool GameObject::registerWallCollision()
{
	//move a little away from the wall if too close
	accelMagnitude = 0;
	speed = 0;
	//std::cout << location.x << "," << location.y << "\n";
	if (GameObject::location.x < 0)
	{
		GameObject::location.x=0;
	}
	if (GameObject::location.y < 0)
	{
		GameObject::location.y = 0;
	}
	//for some reason it works fine without this code
	//if (GameObject::location.x > (WorldState.CoordinateSizeX - 1))
	//	GameObject::location.x = WorldState.CoordinateSizeX - 1;
	//if (GameObject::location.y > (WorldState.CoordinateSizeY - 1))
	//	GameObject::location.y = WorldState.CoordinateSizeY - 1;
	return true;
}

void GameObject::applyForceRect(float x,float y)
{
	float oldX = accelMagnitude * cos(accelHeading * 3.14159/180);
	float oldY = accelMagnitude * sin(accelHeading *3.14159/180);
	//should probably be using vectors everywhere
	float newX = oldX + x;
	float newY = oldY + y;
	accelMagnitude = sqrt(newX*newX + newY*newY);
	accelHeading = (180.0 * atan2(newY,newX)) / 3.14159;
}

void GameObject::applyForcePolar(CL_Angle heading, float magnitude)
{
	float X = magnitude * cos(heading.to_radians());
	float Y = magnitude * sin(heading.to_radians());
	applyForceRect(X,Y);
}

GameObject::~GameObject()
{
	std::cout << displayName << " destructor" << endl;
	delete sprite;
}