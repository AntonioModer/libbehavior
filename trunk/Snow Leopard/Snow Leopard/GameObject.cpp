#include "GameObject.h"
#include "WorldState.h"
#include "turnTowardsTarget.h"
#include "goStraight.h"
#include "BehaviorTreeNode.h"
#include "globals.h"


#include "xerces.h"
using namespace SL;


double mod5 (double toMod, int op)
{
	double intpart;
	toMod = modf(toMod,&intpart);
	toMod = toMod + (int)intpart % op;
	return toMod;
}


int GameObject::IDCount = 0;
GameObject::GameObject(xerces DOMNode* rootNode)
{
	xerces DOMNamedNodeMap* attributes =  rootNode->getAttributes();
	displayName = getAttributeStr("name",attributes);
	//displayHeading = getAttributeDouble("current_angle",attributes);
	faction = getAttributeInt("faction",attributes);
	ID = getAttributeStr("id",attributes);
	speed=getAttributeDouble("movementSpeed",attributes);
	heading=getAttributeDouble("movementHeading",attributes);
	accelMagnitude = getAttributeDouble("accelerationMagnitude",attributes);
	accelHeading = getAttributeDouble("accelerationHeading",attributes);
	actionPriority = DefActionPriority;
	renderPriority = DefRenderPriority;
	mass = NULL; //need to fill these two by calculations on the components
	rotationalInertia = NULL;
	isPlayer = getAttributeBool("isPlayer",attributes);
	//usesPhysics = getAttributeBool("usesPhysics",attributes);  //TODO: disabled to simplify development
	usesPhysics = false;

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
	sprite->set_base_angle(CL_Angle::from_degrees(getAttributeInt("base_angle",BaseImageAttributes)));

	displayHeading = getAttributeDouble("current_angle",BaseImageAttributes);

	//need to add code to set the scale. This requires communicating with the worldState at runtime

	collisionOutline = new CL_CollisionOutline(image);
	collisionOutline->set_alignment(translationOrigin,translation_offset_x,translation_offset_y);
	collisionOutline->set_rotation_hotspot(rotationOrigin,rotation_offset_x,rotation_offset_y);

	targetPriorities = new GameObjectList();

	brain = new ParallelNode();
	goStraight* gs = new goStraight();
	turnTowardsTarget* tt = new turnTowardsTarget();
	((SequentialNode*) brain)->addChild(tt);
	((SequentialNode*) brain)->addChild(gs);
	
	brain->init(this);
}


bool GameObject::doActions()
{
	if (usesPhysics)
		processMovementPhysics();
	if (!(isPlayer))
		brain->execute(this);
	return true;
}

bool GameObject::registerCollision(GameObject* collidedObject)
{
	/*std::cout << displayName <<  " Collided with " << collidedObject->displayName << "\n";
	std::cout << "MyPosition: " << collisionOutline->get_translation()<< "\n";
	std::cout << "TheirPosition: " << collidedObject->collisionOutline->get_translation()<< "\n";*/
	return true;
}

bool GameObject::processMovementPhysics()
{
	//new velocity components
	double newX = .5 * accelMagnitude * cos(accelHeading * 3.14159/180) * worldState->timeElapsed
		+ speed * cos(heading * 3.14159/180);
	double newY = .5 * accelMagnitude * sin(accelHeading *3.14159/180) * worldState->timeElapsed
		+ speed * sin(heading * 3.14159/180); 

	speed = sqrt((newX)*(newX) + (newY)*(newY));
	heading = (180 * atan2(newY,newX)) / 3.14159;

	/*std::cout << this->displayName << "Physics Update: \n";
	std::cout << "Velocity Heading: " << this->heading << "\n";
	std::cout << "Speed: " << this->speed << "\n";
	std::cout << "AccelHeading: " << this->accelHeading << "\n";
	std::cout << "AccelMagnitude: " << this->accelMagnitude << "\n\n";*/

	worldState->moveObject(this,location.offsetPolar(heading,speed * worldState->timeElapsed));
	
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

bool GameObject::rotate(double angle)
{
	displayHeading += angle;
	heading += angle;
	displayHeading = mod5(displayHeading,360);
	collisionOutline->rotate(CL_Angle::from_degrees(angle));
	return true;
}

void GameObject::applyForceRect(double x,double y)
{
	double oldX = accelMagnitude * cos(accelHeading * 3.14159/180);
	double oldY = accelMagnitude * sin(accelHeading *3.14159/180);
	//should probably be using vectors everywhere
	double newX = oldX + x;
	double newY = oldY + y;
	accelMagnitude = sqrt(newX*newX + newY*newY);
	accelHeading = (180 * atan2(newY,newX)) / 3.14159;
}

void GameObject::applyForcePolar(double heading, double magnitude)
{
	double X = magnitude * cos(heading * 3.14159/180);
	double Y = magnitude * sin(heading * 3.14159/180);
	applyForceRect(X,Y);
}

GameObject::~GameObject()
{
	std::cout <<"deleting" << displayName;
	delete sprite;
}