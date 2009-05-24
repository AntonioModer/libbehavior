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
using namespace std;

// Instantiate the globals that were declared as "extern" in globals.h
CL_GraphicContext* SL::gc;
WorldState* SL::ws;
CL_InputContext* SL::ic;
Renderer* SL::ren;
GameLogic* SL::gl;


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
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	
	XMLPlatformUtils::Initialize();

	CL_DisplayWindow window("Snow Leopard", 640, 480);
	CL_GraphicContext graphics_context = window.get_gc();
	SL::gc = &graphics_context;
	CL_InputContext input_context = window.get_ic();
	SL::ic = &input_context;

	CL_InputDevice keyboard = ic->get_keyboard();
	CL_InputDevice mouse = ic->get_mouse();

	if(AllocConsole()) {
        freopen("CONOUT$", "w", stdout); //redirect std io to it
        SetConsoleTitle("Debug Console");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);  
	}
	
	cout << "Now Debugging..." <<endl;
	
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

	WorldState* state = new WorldState(doc);
	SL::ws = state;
	Renderer* renderer = new Renderer();
	SL::ren = renderer;
	GameLogic* logic = new GameLogic();
	SL::gl = logic;

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