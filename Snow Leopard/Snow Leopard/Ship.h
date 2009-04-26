#ifndef SHIP_H_
#define SHIP_H_
#include <list>
#include <vector>
#include <string>
#include "point.h"
#include <set>
#include "GameObject.h"
#include "WorldState.h"
#include "xerces.h"

class CL_ResourceManager;

namespace SL
{
class WorldState;

class Ship: public GameObject {

public:
	
	bool doActions();
	bool registerCollision(GameObjectList collisions);
	bool registerWallCollision();
	Ship::Ship(xerces DOMNode* rootNode);
	virtual bool move();
	bool move(AbsoluteDirection dir);
	bool move(RelativeDirection dir);
	virtual bool shoot();
	void (*AI_Movement) (Ship* ship);
	void (*AI_Shooting) (Ship* ship);



};

}
#endif