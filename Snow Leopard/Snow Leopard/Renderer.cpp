#include "Renderer.h"

#include "GameObject.h"
#include "WorldState.h"

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

using namespace SL;

Renderer::Renderer(CL_DisplayWindow* window,CL_GraphicContext* gc_, WorldState* state_)
{
	gc = gc_;
	state = state_;
	screenStartX = 0.0;
	screenStartY = 0.0;
	screenWidth = 640; //TODO get dynamically
	screenHeight = 480; //TODO get dynamically
	CL_FontDescription desc;
	desc.set_typeface_name("Times New Roman");
	font = new CL_Font(*gc,desc);
	camera = state->getCamera();
}
bool Renderer::setCamera(GameObject* obj)
{
	camera = obj;
	return true;
}

bool Renderer::Render()
{
	const GameObjectList* objects = state->getAllGameObjects(WorldState::RENDER_SORTED);
	gc->clear();

	ConstGameObjectIter itr;
	for (itr = objects->begin(); itr!=objects->end();itr++)
	{
		GameObject* obj = *itr;
		CL_Sprite* sprite = obj->sprite;
		sprite->set_angle(CL_Angle::from_degrees(obj->displayHeading));
		sprite->draw(*gc,obj->location.x,obj->location.y);
		obj->collisionOutline->draw(0,0,CL_Colorf::azure,*gc);
	}
	
	return true;

}