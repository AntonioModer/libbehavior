#ifndef PROJECTILE_H_
#define PROJECTILE_H_
#include <list>
#include <string>
#include "point.h"
#include <set>
#include "GameObject.h"
#include "Definitions.h"


namespace SL
{
class WorldState;
class GameObject;

class Projectile: public GameObject {

public:

	//the following properties characterize a projectile type
	//std::vector<ProjectileProperty> ProjectileProperties;
	//the sprite
	//name
	//description


	bool doActions();
	bool registerCollision(GameObjectList collisions);
	bool registerWallCollision();
	Projectile::Projectile(xerces DOMNode* rootNode);
	Projectile::Projectile(Projectile& proj);
	virtual bool move();


};

}
#endif