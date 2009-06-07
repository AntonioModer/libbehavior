#include "Ship.h"
#include "GoStraight.h"
#include "AbsoluteMovement.h"
#include "Fire.h"
#include "Cooldown.h"
#include "IntCondition.h"
#include "BoolCondition.h"
#include "WorldState.h"

#ifndef SCENARIO2_H_
#define SCENARIO2_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	WorldState* loadScenario2()
	{
		WorldState* state = new WorldState();

		GameObject* background = new GameObject();
		background->usesPhysics = false;
		background->setSprite("Backgrounds\\background");
		state->addBackgroundObject(background);

		Ship* player = new Ship();
		player->setSprite("Hulls\\Sample Hull");
		player->isPlayer = true;
		state->insertObject(player,point(100,300));

		const int offset = 50;
		for (int i = 0 ; i<1 ; i++)
		{
			Ship* opponent = new Ship();
			opponent->displayHeading.set_degrees(180);
			opponent->setSprite("Hulls\\Fighter1");
			//opponent->brain->addChild(new AbsoluteMovement(DOWN,5));
			opponent->brain
				->addChild((new SequentialNode())
					->addChild(new BoolCondition<GameObject>(&GameObject::alignedWithPlayer,true))
					->addChild(new Fire())
					->addChild(new Cooldown(500)))
			    ->addChild((new SequentialNode())
					->addChild(new AbsoluteMovement(DOWN,5,500))
					->addChild(new Cooldown(2000)));
			state->insertObject(opponent,point(50 + i * offset,100));
		}
		return state;
	}}
#endif