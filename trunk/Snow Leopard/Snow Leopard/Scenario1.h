#include "Ship.h"
#include "GoStraight.h"
#include "AbsoluteMovement.h"
#include "Fire.h"
#include "Cooldown.h"
#include "IntCondition.h"
#include "BoolCondition.h"
#include "WorldState.h"

#ifndef SCENARIO1_H_
#define SCENARIO1_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
WorldState* loadScenario1()
{
	WorldState* state = new WorldState();

	GameObject* background = new GameObject();
	background->usesPhysics = false;
	background->setSprite("Backgrounds\\background.png");
	state->addBackgroundObject(background);

	Ship* player = new Ship();
	player->setSprite("Hulls\\Sample Hull.png");
	player->isPlayer = true;
	state->insertObject(player,point(100,100));

	Ship* opponent = new Ship();
	opponent->setSprite("Hulls\\Sample Hull.png");
	//opponent->brain->addChild(new AbsoluteMovement(DOWN,5));
	opponent->brain->addChild(
		(new SequentialNode())
		//->addChild(new IntCondition<Ship>(&Ship::test,EQUAL,100))
		->addChild(new IntCondition<>(&Ship::test2,EQUAL,100))
		//->addChild(new Fire())
		->addChild(new Cooldown(1000))
		->addChild(new BoolCondition<Ship>(&Ship::test3,true))
		->addChild(new BoolCondition<>(&Ship::test4,true)));
	state->insertObject(opponent,point(300,300));
	return state;
}}
#endif