#include "Ship.h"
#include "Definitions.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

using namespace SL;
using namespace std;

bool Ship::doActions()
{

	move();
	shoot();
	return true;
	
}

bool Ship::move()
{
#ifdef PHYSICS
	GameObject::processMovementPhysics();
#endif
	if (!(GameObject::isPlayer))
	{
	AI_Movement(this);
	}
	return true;
}

bool Ship::shoot()
{
	if (!(GameObject::isPlayer))
	{
	AI_Shooting(this);
	}
	return true;
}

Ship::Ship(xerces DOMNode* rootNode) : GameObject(rootNode)
{
	actionPriority = ShipActionPriority;
	renderPriority = ShipRenderPriority;
	//AI_Movement = &AI_Move_DoNothing;
//	AI_Shooting = &AI_Shoot_DoNothing;
	GameObject::usesPhysics = false;
}
	
bool Ship::registerWallCollision()
{
	//cancel accelerations
	accelMagnitude = 0;
	speed = 0;

	
	return true;
}
