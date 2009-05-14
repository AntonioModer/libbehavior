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

struct ProjectileSetup : GameObjectSetup
{
	ProjectileSetup():GameObjectSetup(){}
};

class Projectile: public GameObject {

public:

	bool doActions();
	bool registerCollision(GameObjectList collisions);
	bool registerWallCollision();
	Projectile::Projectile(xerces DOMNode* rootNode);
	Projectile::Projectile(Projectile& proj);
	Projectile::Projectile(ProjectileSetup p);
	virtual bool move();


};

}
#endif