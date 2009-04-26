#include "GameLogic.h"

#include "GameObject.h"
#include "WorldState.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Ship.h"
#include "Renderer.h"
#include "Definitions.h"

using namespace SL;

GameLogic::GameLogic(WorldState* worldState,CL_InputContext* ic,Renderer* rend)
{
	state=worldState;
	keyboard = &ic->get_keyboard();
	mouse = &ic->get_mouse();
	renderer = rend;
	playerShip = (Ship*) state->getCamera();
}

bool GameLogic::step()
{

	state->deleteQueued();
	unsigned int currentTime = CL_System::get_time();
	state->timeElapsed = currentTime - state->time;
	state->time = currentTime;

	//should handle input with event callbacks, but I can't get them to work
	handleInput();
	
	const GameObjectList* objects = state->getAllGameObjects();
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
		 state->moveObject(playerShip,playerShip->location.offsetRect(0,-10));
	 }

	 if (keyboard->get_keycode(CL_KEY_DOWN))
	 {
		 state->moveObject(playerShip,playerShip->location.offsetRect(0,10));
	 }

	 if (keyboard->get_keycode(CL_KEY_LEFT))
	 {
		state->moveObject(playerShip,playerShip->location.offsetRect(-10,0));
	 }

	 if (keyboard->get_keycode(CL_KEY_RIGHT))
	 {
		state->moveObject(playerShip,playerShip->location.offsetRect(10,0));
	 }
#endif

#ifdef RELATIVE_MOVEMENT
#ifdef NOPHYSICS
	 if (keyboard->get_keycode(CL_KEY_UP))
	 {
		 std::cout<<"going forward";
		 playerShip->move(GameObject::FORWARD);
	 }

	 if (keyboard->get_keycode(CL_KEY_DOWN))
	 {
		 std::cout << "going back";
		 playerShip->move(GameObject::BACK);
	 }

	 if (keyboard->get_keycode(CL_KEY_LEFT))
	 {
		 std::cout << "turning left";
		 //playerShip->move(GameObject::TURN_LEFT);
		 playerShip->rotate(-2);
	 }

	 if (keyboard->get_keycode(CL_KEY_RIGHT))
	 {
		 std::cout << "turning right";
		 //playerShip->move(GameObject::TURN_RIGHT);
		  playerShip->rotate(2);
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