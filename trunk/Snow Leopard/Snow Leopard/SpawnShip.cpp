#include "SpawnShip.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Ship.h"
#include <iostream>
using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;
using namespace std;

void SpawnShip::init(void* agent)
{
};

SpawnShip::SpawnShip()
{
}
BEHAVIOR_STATUS SpawnShip::execute(void* agent)
{
	Ship* opponent = new Ship();
	opponent->displayHeading.set_degrees(180);
	opponent->setSprite("Hulls\\drone");
	
	ws->insertObject(opponent,point(100,100));
	return BT_SUCCESS;
}