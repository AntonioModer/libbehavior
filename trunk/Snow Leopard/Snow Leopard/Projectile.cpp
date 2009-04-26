#include "Projectile.h"
#include "globals.h"

using namespace SL;

bool Projectile::doActions()
{

	move();
	return true;
	
}

bool Projectile::move()
{
	return true;
}

Projectile::Projectile(xerces DOMNode* rootNode) : GameObject(rootNode)
{
	actionPriority = ProjectileActionPriority;
	renderPriority = ProjectileRenderPriority;
}
	
bool Projectile::registerWallCollision()
{
	ws->deleteObject(this);
	return true;
}
