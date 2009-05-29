#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include <list>
#include <string>
#include "point.h"
#include <set>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "globals.h"
#include "BehaviorTree.h"

class CL_Sprite;
class CL_CollisionOutline;

namespace SL
{
class WorldState;
class BehaviorTreeNode;
class GameObject;

/// Base class for any item in the game
class GameObject{

public:
	/// Called every frame to update the object
	virtual bool doActions();
	/// Notify the object of a collision with another object. Only called if usesPhysics is true
	virtual bool registerCollision(GameObject* collidedObject);
	/// Notify the object it has hit the edge of the level. Responding improperly to this can cause out of bounds errors
	virtual bool registerWallCollision();
	/// Default constructor
	GameObject::GameObject();
	/// The graphical representation of the object
	CL_Sprite* sprite;
	/// Cleans up resources in use by the object when it is deleted
	GameObject::~GameObject();
	/// If Newtonian physics is enabled for the world, this imparts a force on the object
	void applyForceRect(float x, float y);
	/// If Newtonian physics is enabled for the world, this imparts a force on the object
	void applyForcePolar(CL_Angle heading, float magnitude);
	/// The "AI" for the object.
	BehaviorTree::ParallelNode* brain;
	/// Unique for each object instance
	int ID;
	/// Generate a unique ID for an object
	static int GameObject::getID()
{
	IDCount++;
	return IDCount;
}

	/// The human friendly name of the object
	std::string displayName; 
	/// True if this object is the player. Undefined behavior if more than one object has this flag set to true.
	bool isPlayer;
	
	/// The number of "hit points" the object has.
	unsigned int HP;
	/// The directional component of the object's velocity
	CL_Angle movementHeading;
	/// The way the object's sprite is "turned"
	CL_Angle displayHeading;
	/// The collision outline generated from the sprite
	CL_CollisionOutline* collisionOutline;
	/// The directional component of the object's acceleration
	float accelHeading;
	/// The scalar component of the object's acceleration
	float accelMagnitude;
	/// Used to divide objects into different categories to customize interactions between them
	int faction;
	/// Used to scale how objects are displayed. Not currently implemented
	int displaySize;
	/// Absolute coordinates of the object in the world.
	point location;
	/// The scalar component of the object's velocity
	float speed;
	/// Determines whether the object collides with others
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