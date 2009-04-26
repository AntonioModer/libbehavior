#ifndef RENDERER_H_
#define RENDERER_H_

#include <map>
#include "boost/lexical_cast.hpp"
#include <stdlib.h>

class CL_DisplayWindow;
class CL_GraphicContext;
class CL_ResourceManager;
class CL_Font;

namespace SL
{
class WorldState;
class GameObject;

class Renderer{
public:
	Renderer::Renderer();
	bool Renderer::Render();
	int screenWidth;
	int screenHeight;
	bool Renderer::setCamera(GameObject* obj);

private:
	GameObject* camera;
	CL_ResourceManager* resources;
	double screenStartX;
	double screenStartY;
	CL_Font* font;

};
}
#endif