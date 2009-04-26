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
class DOMNode;
typedef std::list<GameObject*> GameObjectList;
typedef GameObjectList::iterator GameObjectIter;
typedef GameObjectList::const_iterator ConstGameObjectIter;
#define coarseGraining 100


class Render
{
public:
bool operator() (GameObject* lhs, GameObject* rhs) {
return lhs->renderPriority < rhs->renderPriority;
}
};

class Action
{
public:
bool operator() ( GameObject* lhs,  GameObject* rhs) {
return lhs->actionPriority < rhs->actionPriority;
}
};



class WorldState
{
public:

enum SortPreference {ACTION_SORTED,RENDER_SORTED,UNSORTED};
int CellSizeX;
int CellSizeY;
double CoordinateSizeX;
double CoordinateSizeY;
unsigned long long time;
unsigned int timeElapsed;
std::string description;
std::string name;
std::string id;

WorldState::WorldState();
WorldState::WorldState(xerces DOMNode* rootNode);
bool insertObject(GameObject* gameObject, point p);
GameObject* getCamera();
bool deleteObject(GameObject* gameObject);
bool moveObject(GameObject* gameObject, point p);
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