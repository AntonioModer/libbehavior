#include "WorldState.h"
#include "GameObject.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "xerces.h"

using namespace SL;

inline int round(double x)
{
return int(x > 0.0 ? x + 0.5 : x - 0.5);
}

WorldState::WorldState()
{
	description = "";
	CoordinateSizeX = 1024.0;
	CoordinateSizeY = 768.0;

	CellSizeX = (int)CoordinateSizeX / coarseGraining ;
	CellSizeY = (int)CoordinateSizeY / coarseGraining ;

	allObjectList = new GameObjectList();
	deleteList = new GameObjectList();
	worldMatrix = new GameObjectList**[CellSizeX];
	for (int i = 0; i < CellSizeX; ++i)
        worldMatrix[i] = new GameObjectList*[CellSizeY];

	for (int i=0;i<CellSizeX;i++)
	{
		for (int j=0;j<CellSizeY;j++)
		{
			worldMatrix[i][j] = new GameObjectList();
		}
	}
}

GameObject* WorldState::getCamera() //returns a random object marked as being a player. Need to add code to verify there is only one
{
	GameObjectIter itr;
	for (itr = allObjectList->begin();itr != allObjectList->end(); itr++)
	{
		if ((*itr)->isPlayer)
			return *itr;
	}
	throw std::exception("no camera defined");
	return NULL;
}

WorldState::WorldState(xerces DOMNode* rootNode)
{
	xerces DOMNode* worldStateNode = rootNode->getFirstChild();
	xerces DOMNamedNodeMap* attributes =  worldStateNode->getAttributes();


	CoordinateSizeX = getAttributeDouble ("CoordinateSizeX",attributes);
	CoordinateSizeY = getAttributeDouble ("CoordinateSizeY",attributes);
	time = getAttributeInt ("time",attributes);
	id = getAttributeStr("id",attributes);
	std::cout <<id;
	name = getAttributeStr("name",attributes);
	std::cout <<name;
	description = getAttributeStr("description",attributes);
	std::cout <<description;



	CellSizeX = (int)CoordinateSizeX / coarseGraining ;
	CellSizeY = (int)CoordinateSizeY / coarseGraining ;

	allObjectList = new GameObjectList();
	deleteList = new GameObjectList();
	worldMatrix = new GameObjectList**[CellSizeX];
	for (int i = 0; i < CellSizeX; ++i)
        worldMatrix[i] = new GameObjectList*[CellSizeY];

	for (int i=0;i<CellSizeX;i++)
	{
		for (int j=0;j<CellSizeY;j++)
		{
			worldMatrix[i][j] = new GameObjectList();
		}
	}
	xerces DOMNodeList* entities = ((xerces DOMElement*)worldStateNode)->getElementsByTagName(XercesString("Entity").xmlCh());
	for (int x = 0;x<entities->getLength();x++)
	{
		xerces DOMElement* entityNode = (xerces DOMElement*)entities->item(x);
		std::cout << "now reading node" << entityNode->getAttribute(XercesString("id").xmlCh());
		xerces DOMNamedNodeMap* entityAttributes = entityNode->getAttributes();
		point p(getAttributeDouble("xPosition",entityAttributes),getAttributeDouble("yPosition",entityAttributes));
		insertObject(new GameObject(entityNode),p);
	}
}


bool WorldState::insertObject(GameObject* gameObject, point p)
{
	if (pointOutofBounds(p))
		return false;

	GameObjectList* currentList = getListFromPoint(p);
	currentList->push_front(gameObject);
	gameObject->location = p;
	allObjectList->push_front(gameObject);
	gameObject->worldState = this;
	gameObject->collisionOutline->set_translation(p.x,p.y);
	return true;
}

bool WorldState::deleteObject(GameObject* gameObject)
{
	GameObjectList* currentList = getListFromPoint(gameObject->location);
	allObjectList->remove(gameObject);
	currentList->remove(gameObject);
	registerForDeletion(gameObject);
	return true;
}

bool WorldState::moveObject(GameObject* gameObject, point p)
{
	//replace with pixel perfect collision code
	if (pointOutofBounds(p))
	{
		gameObject->registerWallCollision();
		return false;
	}

	//test for and report collisions
	//currently tests against all objects (does not scale well)
	//update with more sophisticated filtering in the future

	GameObjectIter itr;
	if (gameObject->usesPhysics)
	{
		for (itr = allObjectList->begin();itr != allObjectList->end(); itr++)
		{
			if ((*itr)->usesPhysics)
			{
				if ((*itr)->collisionOutline->collide(*(gameObject->collisionOutline)))
				{
					(*itr)->registerCollision(gameObject);
					gameObject->registerCollision(*itr);
				}
			}

		}
	}


	GameObjectList* currentList = getListFromPoint(gameObject->location);
		GameObjectList* newList = getListFromPoint(p);
		gameObject->location=p;
		if (currentList==newList)
			return true;
		newList->push_front(gameObject);
		currentList->remove(gameObject);

		gameObject->collisionOutline->set_translation(p.x,p.y);
		return true;

}

void WorldState::registerForDeletion(GameObject* obj)
{
	deleteList->push_front(obj);
}

void WorldState::deleteQueued()
{
	ConstGameObjectIter itr;
	deleteList->unique();
	for(itr = deleteList->begin();itr !=deleteList->end();)
	{
		delete (*itr++);
	}
	deleteList->clear();
}

GameObjectList* WorldState::getAtCell(point p)
{
	if (pointOutofBounds(p))
		return NULL;
	return getListFromPoint(p);
}


const GameObjectList* WorldState::getAllGameObjects(SortPreference p)
{
	switch (p)
	{
	case ACTION_SORTED:  allObjectList->sort(Action());break;
	case RENDER_SORTED:  allObjectList->sort(Render());break;
	}
	return  allObjectList; //can't change the contents of the list, but _can_ change the properties of the objects
}


bool WorldState::pointOutofBounds(point p)
{
 	if (round(p.x / coarseGraining)>=CellSizeX || p.x<0 || round(p.y / coarseGraining)>=CellSizeY || p.y<0)
		return true;
	return false;

}

GameObjectList* WorldState::getListFromPoint(point p)
{
	return worldMatrix[(int)p.x / coarseGraining][(int)p.y / coarseGraining];
}