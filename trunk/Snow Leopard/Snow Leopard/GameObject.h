#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include <list>
#include <string>
#include "point.h"
#include <set>
#include "GameObjectPriorities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "globals.h"
#include "xerces.h"

class CL_Sprite;
class CL_CollisionOutline;

namespace SL
{
class WorldState; //need to forward declare class to avoid crazy recursion
class DOMNode;
class BehaviorTreeNode;

struct GameObjectSetup
{
	std::string displayName; 
	bool isPlayer;
	CL_Angle movementHeading;
	CL_Angle displayHeading;
	CL_CollisionOutline* collisionOutline;
	double accelHeading;
	double accelMagnitude;
	int faction;
	int displaySize;
	point location;
	int actionPriority;
	int renderPriority;
	double speed;
	bool usesPhysics;
GameObjectSetup():
	isPlayer(false),
	displayName("default"),
	displayHeading(CL_Angle::from_degrees(0)),
	faction(0),
	speed(0),
	movementHeading(CL_Angle::from_degrees(0)),
	accelMagnitude(0),
	accelHeading(0),
	actionPriority(DefActionPriority),
	renderPriority(DefRenderPriority),
	usesPhysics(true)
	{}
};

class GameObject{

public:

	typedef std::vector<GameObject*> GameObjectList;
	typedef std::set <GameObject*>::iterator GameObjectIter;
	bool operator<(GameObject &b);
	virtual bool doActions();
	virtual bool registerCollision(GameObject* collidedObject);
	virtual bool registerWallCollision();
	GameObject::GameObject(xerces DOMNode* rootNode);
	GameObject::GameObject(GameObjectSetup setup);
	CL_Sprite* sprite;
	GameObject::~GameObject();
	void applyForceRect(double x, double y);
	void applyForcePolar(CL_Angle heading, double magnitude);

	BehaviorTreeNode* brain;

	GameObjectList* targetPriorities;

	static int GameObject::getID()
{
	IDCount++;
	return IDCount;
}

	std::string displayName; 
	bool isPlayer; //right now only one ship can be a player
	
	//polar coordinates
	unsigned int HP;
	CL_Angle movementHeading;
	CL_Angle displayHeading;
	CL_CollisionOutline* collisionOutline;
	double accelHeading;
	double accelMagnitude;
	int faction;
	int displaySize;
	point location;
	int actionPriority;
	int renderPriority;
	std::string ID;
	double speed;
	bool usesPhysics;

	//stupid hack to convert between the CL_Origin enum and string values
	
	//static std::string origin_type_map[] = {"origin_top_left","origin_top_center","origin_top_right","origin_center_left","origin_center","origin_center_right","origin_bottom_left","origin_bottom_center","origin_bottom_right"};
	static CL_Origin GameObject::getOriginfromString(std::string str)
	{
		if      (str== "top_left")
			return origin_top_left;
		else if (str== "top_center")
			return origin_top_center;
		else if (str== "top_right")
			return origin_top_right;
		else if (str== "center_left")
			return origin_center_left;
		else if (str== "center")
			return origin_center;
		else if (str== "center_right")
			return origin_center_right;
		else if (str== "bottom_left")
			return origin_bottom_left;
		else if (str== "bottom_center")
			return origin_bottom_center;
		else if (str== "bottom_right")
			return origin_bottom_right;
		throw std::exception("invalid origin string");
	}

 protected:
	
	 bool processMovementPhysics();
 private:
	
	 static int IDCount;
	 

};
}
#endif