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
	WorldState* state;
	Ship* playerShip;
	CL_InputDevice* keyboard;
	CL_InputDevice* mouse;
	void handleInput();
	Renderer* renderer;
	
	

public:
		GameLogic::GameLogic(WorldState* worldState,CL_InputContext* ic,Renderer* renderer);
		bool GameLogic::step();


};
}
#endif