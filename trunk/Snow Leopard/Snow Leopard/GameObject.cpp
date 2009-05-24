#include "GameObject.h"
#include "WorldState.h"
#include "turnTowardsTarget.h"
#include "goStraight.h"
#include "BehaviorTree.h"
#include "globals.h"


#include "xerces.h"
using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;

int GameObject::IDCount = 0;
GameObject::GameObject(xerces DOMNode* rootNode)
{
	xerces DOMNamedNodeMap* attributes =  rootNode->getAttributes();
	displayName = getAttributeStr("name",attributes);
	displayHeading = CL_Angle::from_degrees(0);
	faction = getAttributeInt("faction",attributes);
	ID = getID();
	speed=(float)getAttributeDouble("movementSpeed",attributes);
	movementHeading=CL_Angle::from_degrees((float)getAttributeDouble("movementHeading",attributes));
	accelMagnitude = (float)getAttributeDouble("accelerationMagnitude",attributes);
	accelHeading = (float)getAttributeDouble("accelerationHeading",attributes);
	isPlayer = getAttributeBool("isPlayer",attributes);
	usesPhysics = getAttributeBool("usesPhysics",attributes);
	cout << "uses physics" << usesPhysics << endl;
	//usesPhysics = false;

	//set up the base sprite
	xerces DOMNodeList* BaseImageList = ((xerces DOMElement*) rootNode)->getElementsByTagName(XercesString("BaseImage").xmlCh());
	xerces DOMNode* BaseImage = BaseImageList->item(0);
	xerces DOMNamedNodeMap* BaseImageAttributes = BaseImage->getAttributes();
	std::string filename = getAttributeStr("pictureSource",BaseImageAttributes);
	CL_PixelBuffer image = CL_PNGProvider::load(CL_String(filename));
	CL_SpriteDescription desc;
	desc.add_frame(image);
	sprite = new CL_Sprite(*SL::gc,desc);
	
	CL_Origin translationOrigin = getOriginfromString(getAttributeStr("translation_origin",BaseImageAttributes));
	CL_Origin rotationOrigin = getOriginfromString(getAttributeStr("rotation_origin",BaseImageAttributes));
	int translation_offset_x = getAttributeInt("translation_offset_x",BaseImageAttributes);
	int translation_offset_y = getAttributeInt("translation_offset_y",BaseImageAttributes);
	int rotation_offset_x = getAttributeInt("rotation_offset_x",BaseImageAttributes);
	int rotation_offset_y = getAttributeInt("rotation_offset_y",BaseImageAttributes);
	sprite->set_alignment(translationOrigin,translation_offset_x,translation_offset_y);
	sprite->set_rotation_hotspot(rotationOrigin,rotation_offset_x,rotation_offset_y);
	sprite->set_base_angle(CL_Angle::from_degrees((float)getAttributeInt("base_angle",BaseImageAttributes)));

	displayHeading = CL_Angle::from_degrees((float)getAttributeDouble("current_angle",BaseImageAttributes));

	// will only need a collision outline if it's going to be colliding with stuff
	if (usesPhysics)
	{
	collisionOutline = new CL_CollisionOutline(image);
	collisionOutline->set_alignment(translationOrigin,(float)translation_offset_x,(float)translation_offset_y);
	collisionOutline->set_rotation_hotspot(rotationOrigin,(float)rotation_offset_x,(float)rotation_offset_y);
	collisionOutline->set_angle(displayHeading);
	}

	brain = new ParallelNode();
	//goStraight* gs = new goStraight();
	TurnTowardsTarget* tt = new TurnTowardsTarget();
	brain->addChild(tt);
	//brain->addChild(gs);
	
	HP = 100;

	brain->init(this);
}

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
	CL_PixelBuffer image = CL_PNGProvider::load(CL_String("Resources\\Ammo\\test.png"));
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
	brain->init(this);
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