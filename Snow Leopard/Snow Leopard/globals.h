#ifndef GLOBALS_H
#define GLOBALS_H
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "GameObject.h"
#include "WorldState.h"
#include "Renderer.h"
#include "GameLogic.h"

namespace SL
{
	//these represent the overall context
	extern CL_GraphicContext* gc;
	extern WorldState* ws;
	extern CL_InputContext* ic;
	extern Renderer* ren;
	extern GameLogic* gl;
}
#endif