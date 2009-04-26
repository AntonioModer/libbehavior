#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

class CL_InputContext;
class CL_InputDevice;

namespace SL
{
	
class WorldState;
class GameObject;
class Ship;
class Renderer;


class GameLogic {

private:
	Ship* playerShip;
	CL_InputDevice* keyboard;
	CL_InputDevice* mouse;
	void handleInput();
	
	

public:
		GameLogic::GameLogic();
		bool GameLogic::step();


};
}
#endif