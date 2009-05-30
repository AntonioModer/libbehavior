#include "Ship.h"
#include "Definitions.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Projectile.h"

using namespace SL;
using namespace std;

bool Ship::doActions()
{

	//move();
	//shoot();
	if (!isPlayer)
		brain->execute(this);
	return true;
	
}

void Ship::fire()
{
	Projectile* p = new Projectile();
	p->setSprite("Ammo\\test.png");
	p->displayHeading = *(new CL_Angle(displayHeading));
	ws->insertObject(p,location.offsetPolar(displayHeading,100));
}

Ship::Ship() : GameObject()
{
	GameObject::usesPhysics = true;
}
	
bool Ship::registerWallCollision()
{
	//cancel accelerations
	accelMagnitude = 0;
	speed = 0;

	
	return true;
}

bool Ship::registerCollision(GameObject* collidedObject)
 {
	cout << "ship collision" << endl;
	if (HP <= 50)
		ws->deleteObject(this);
	else
		HP -= 50;
	return true;
}
