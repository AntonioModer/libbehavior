#include "GameLogic.h"

#include "GameObject.h"
#include "Ship.h"
#include "Definitions.h"
#include "globals.h"

using namespace SL;
using namespace std;

GameLogic::GameLogic()
{
	keyboard = &ic->get_keyboard();
	mouse = &ic->get_mouse();
	playerShip = (Ship*) ws->getCamera();
}

bool GameLogic::step()
{

	ws->deleteQueued();
	unsigned int currentTime = CL_System::get_time();
	ws->timeElapsed = currentTime - ws->time;
	ws->time = currentTime;

	//should handle input with event callbacks, but I can't get them to work
	handleInput();
	
	const GameObjectList* objects = ws->getAllGameObjects();
	ConstGameObjectIter itr;
	for(itr = objects->begin();itr !=objects->end();)
	{
 		(*itr++)->doActions(); //if the object gets deleted during this, it's ok because the iterator is already incremented
	}
	return true;
}

void GameLogic::handleInput()
{
	 if (keyboard->get_keycode(CL_KEY_SPACE))
	 {
		 //fire weapon
	 }


#ifdef ABSOLUTE_MOVEMENT
	 if (keyboard->get_keycode(CL_KEY_UP))
	 {
		 ws->moveObject(playerShip,playerShip->location.offsetRect(0,-10));
	 }

	 if (keyboard->get_keycode(CL_KEY_DOWN))
	 {
		 ws->moveObject(playerShip,playerShip->location.offsetRect(0,10));
	 }

	 if (keyboard->get_keycode(CL_KEY_LEFT))
	 {
		ws->moveObject(playerShip,playerShip->location.offsetRect(-10,0));
	 }

	 if (keyboard->get_keycode(CL_KEY_RIGHT))
	 {
		ws->moveObject(playerShip,playerShip->location.offsetRect(10,0));
	 }
#endif

#ifdef RELATIVE_MOVEMENT
#ifdef NOPHYSICS
	 if (keyboard->get_keycode(CL_KEY_UP))
	 {
		 ws->moveObject(playerShip,playerShip->location.offsetPolar(playerShip->displayHeading,2));
	 }

	 if (keyboard->get_keycode(CL_KEY_DOWN))
	 {
		 ws->moveObject(playerShip,playerShip->location.offsetPolar(playerShip->displayHeading,-2));
	 }

	 if (keyboard->get_keycode(CL_KEY_LEFT))
	 {
		 ws->rotateObject(playerShip,-2);
	 }

	 if (keyboard->get_keycode(CL_KEY_RIGHT))
	 {
		 ws->rotateObject(playerShip,2);
	 }
#endif
#ifdef PHYSICS
	 if (keyboard->get_keycode(CL_KEY_UP))
	 {
		 playerShip->applyForcePolar(playerShip->displayHeading,.005);
	 }

	 if (keyboard->get_keycode(CL_KEY_DOWN))
	 {
		 playerShip->applyForcePolar(playerShip->displayHeading,-.005);
	 }

	 if (keyboard->get_keycode(CL_KEY_LEFT))
	 {
		 playerShip->rotate(-2);
	 }

	 if (keyboard->get_keycode(CL_KEY_RIGHT))
	 {
		 playerShip->rotate(2);
	 }
#endif
#endif


	 if (keyboard->get_keycode(CL_KEY_ESCAPE))
	 {
		exit(0);
	 }

	  if (keyboard->get_keycode(CL_KEY_LSHIFT))
	  {
		  playerShip->accelMagnitude = 0;
		  playerShip->speed = 0;
	  }

}