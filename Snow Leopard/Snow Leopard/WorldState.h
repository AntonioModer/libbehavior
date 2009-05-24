#ifndef WORLDSTATE_H_
#define WORLDSTATE_H_

#include <list>
#include <queue>
#include "GameObject.h"
#include "point.h"
#include <set>

#include "xerces.h"

namespace SL
{
class GameObject;
/// Convenience typedef for a standard list of GameObjects
typedef std::list<GameObject*> GameObjectList;
/// Convenience typedef for a GameObjectList iterator
typedef GameObjectList::iterator GameObjectIter;
/// Convenience typedef for a const GameObjectList iterator
typedef GameObjectList::const_iterator ConstGameObjectIter;
/// Determines how finely the world is broken down into cells which hold GameObjects
/// A larger value means larger cells, and thus lower memory usage
#define coarseGraining 10


class WorldState
{
public:

/// Width of a cell. Computed from the coarseGraining value
int CellSizeX;
/// Height of a cell. Computed from the coarseGraining value
int CellSizeY;
/// Width of the world
double CoordinateSizeX;
/// Height of the world
double CoordinateSizeY;
/// Current time in milliseconds. The absolute value doesn't mean anything, only differences
unsigned long long time;
/// Stores the amount of real time between frames for e.g. physics updates
unsigned int timeElapsed;
/// Description of the level
std::string description;
/// Name of the level
std::string name;
/// ID of the level
std::string id;

/// Standard Constructor
WorldState::WorldState();
/// Constructor from an XML node
WorldState::WorldState(xerces DOMNode* rootNode);
bool insertObject(GameObject* gameObject, point p);
GameObject* getCamera();
bool deleteObject(GameObject* gameObject);
bool moveObject(GameObject* gameObject, point p);
bool rotateObject(GameObject* gameObject, double angle);
GameObjectList* getAtCell(point p);
const GameObjectList* WorldState::getAllGameObjects(SortPreference p = UNSORTED);
void registerForDeletion(GameObject* obj);
void deleteQueued();
static void WorldState::loadFromDOM(xerces DOMNode* node);
void WorldState::saveToDOM(xerces DOMNode* node);


private:
	GameObjectList*** worldMatrix;
	GameObjectList* allObjectList;
	bool pointOutofBounds(point p);
	GameObjectList* WorldState::getListFromPoint(point p);
	GameObjectList* deleteList;
};

}
#endif