#include "WorldState.h"
#include "GameObject.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "xerces.h"
#include "util.h"

using namespace SL;
using namespace std;

WorldState::WorldState()
{
	description = "";
	CoordinateSizeX = 640.0;
	CoordinateSizeY = 480.0;

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
	cout<<p.toString()<<endl;
	//replace with pixel perfect collision code
	if (pointOutofBounds(p))
	{
		gameObject->registerWallCollision();
		return false;
	}

	//test for and report potential collisions
	//currently tests against all objects (does not scale well)
	//update with more sophisticated filtering in the future


	gameObject->collisionOutline->set_translation(p.x,p.y);
	GameObjectIter itr;
	if (gameObject->usesPhysics)
	{
		for (itr = allObjectList->begin();itr != allObjectList->end(); itr++)
		{
			if ((*itr) == gameObject)
				continue;
			if ((*itr)->usesPhysics)
			{
				cout << "testing for collision" << endl;
				if ((*itr)->collisionOutline->collide(*(gameObject->collisionOutline)))
				{
					cout << "got a collision" << endl;
					cout << (*itr)->displayName <<endl;
					cout << gameObject->displayName << endl;
					(*itr)->registerCollision(gameObject);
					gameObject->registerCollision(*itr);
					gameObject->collisionOutline->set_translation(gameObject->location.x,gameObject->location.y);
					return false; //TODO support colliding with multiple entities
				}
			}

		}
	}


	GameObjectList* currentList = getListFromPoint(gameObject->location);
		GameObjectList* newList = getListFromPoint(p);
		gameObject->location=p;
		if (currentList==newList)
			{}
		else
		{
			newList->push_front(gameObject);
		currentList->remove(gameObject);
		}
		gameObject->collisionOutline->set_translation(p.x,p.y);
		return true;

}

bool WorldState::rotateObject(GameObject* gameObject, double angle)
{
	gameObject->collisionOutline->rotate(CL_Angle::from_degrees(angle));
	if (gameObject->usesPhysics)
	{
		GameObjectIter itr;
		for (itr = allObjectList->begin();itr != allObjectList->end(); itr++)
		{
			if ((*itr) == gameObject)
				continue;
			if ((*itr)->usesPhysics)
			{
				cout << "testing for collision" << endl;
				if ((*itr)->collisionOutline->collide(*(gameObject->collisionOutline)))
				{
					cout << "got a collision" << endl;
					cout << (*itr)->displayName <<endl;
					cout << gameObject->displayName << endl;
					(*itr)->registerCollision(gameObject);
					gameObject->registerCollision(*itr);
					gameObject->collisionOutline->set_angle(gameObject->displayHeading);
					return false; //TODO support colliding with multiple entities
				}
			}
		}
	}

	gameObject->displayHeading += CL_Angle::from_degrees(angle);
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
	std::cout << CoordinateSizeX << "," << CoordinateSizeY << "---" << p.x << "," << p.y << endl;
	if (p.x >= CoordinateSizeX || p.y >= CoordinateSizeY || p.x <= 0 || p.y <= 0)
		return true;
 	//if (round(p.x / coarseGraining)>=CellSizeX || p.x<0 || round(p.y / coarseGraining)>=CellSizeY || p.y<0)
	//	return true;
	return false;

}

GameObjectList* WorldState::getListFromPoint(point p)
{
	return worldMatrix[(int)p.x / coarseGraining][(int)p.y / coarseGraining];
}