#ifndef SOCKET_H_
#define SOCKET_H_
#include <string>
#include "point.h"

namespace SL
{
enum socketType{TRIANGLE_SOCKET,SQUARE_SOCKET,PENTAGON_SOCKET,HEXAGON_SOCKET,OCTAGON_SOCKET,SHELL_SOCKET};

struct socket
{
public:
	
	std::string description;
	std::string name;
	std::string id;
	point position;
	socketType socketType;
	int mass;
	int energy;


private:
	Component* myComponent;
}
}

#endif