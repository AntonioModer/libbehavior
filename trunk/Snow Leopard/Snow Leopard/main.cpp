#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <string>
#include <iostream>
#include "GameObject.h"
#include "WorldState.h"
#include "Renderer.h"
#include "point.h"
#include "GameLogic.h"
#include <ctime>
#include "xerces.h"
#include "GFrameHandler.h"
#include "globals.h"
#include "Definitions.h"

using namespace SL;
CL_GraphicContext* SL::gc;

class DisplayProgram : public CL_ClanApplication
{

public:
	static int main(const std::vector<CL_String> &args);
};

// Create global application object:
// You MUST include this line or the application start-up will fail to
// locate your application object.
CL_ClanApplication app(&DisplayProgram::main);


int DisplayProgram::main(const std::vector<CL_String> &args)
{
	// Setup clanlib modules:

	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	
	XMLPlatformUtils::Initialize();

	CL_DisplayWindow window("Snow Leopard", 640, 480);
	CL_GraphicContext graphics_context = window.get_gc();
	SL::gc = &graphics_context;
	CL_InputContext ic = window.get_ic();

	CL_InputDevice keyboard = ic.get_keyboard();
	CL_InputDevice mouse = ic.get_mouse();

	CL_ConsoleWindow console_window("Console");
	/*CL_ConsoleWindow* console = new CL_ConsoleWindow("Debugging");
#ifdef DEBUG
	console->redirect_stdio();
#endif*/
	
	
	std::cout << "Now Debugging...";
	
	static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
	DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(gLS);
    DOMBuilder* parser = ((DOMImplementationLS*)impl)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS,0);

    parser->setFeature(XMLUni::fgDOMNamespaces,true);
    parser->setFeature(XMLUni::fgXercesSchema, true);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
	
#define xerces XERCES_CPP_NAMESPACE_QUALIFIER
	std::string xmlFile = "Resources\\XML\\example.xml";
	xerces DOMNode* doc;
	doc = parser->parseURI(XercesString(xmlFile).xmlCh());

	WorldState *state = new WorldState(doc);
	Renderer* renderer = new Renderer(&window,gc,state);
	GameLogic *logic = new GameLogic(state,&ic,renderer);

	renderer->setCameraZoomLevel(1.0);

    //CL_FramerateCounter* framerate = new CL_FramerateCounter();
	//framerate->set_fps_limit(60);

	GFrameHandler frame_handler;
	frame_handler.set_fps_limit(60);

	while (true)
	{
	renderer->Render();
	window.flip();
	frame_handler.limit_frame();
	frame_handler.calculate();
	if (CL_DisplayMessageQueue::has_messages())
		CL_DisplayMessageQueue::process();
 	logic->step();
	}
	
	XMLPlatformUtils::Terminate();
	return 0;
}