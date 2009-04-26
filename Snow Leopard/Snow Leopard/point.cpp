#include "point.h"
#include "boost/lexical_cast.hpp"

using namespace SL;
using namespace std;

int nround(double a) {
return int(a + 0.5);
}



point::point(double i, double j)
{
	x = i;
	y = j;
}

point::point()
{
	x=0;
	y=0;
}

point point::offsetRect(double ox, double oy)
{
	return point(x+ox,y+oy);
}

point point::offsetPolar(double heading, double distance)
{
	return point(x - distance * sin(-heading * 3.14159/180),y - distance * cos(-heading * 3.14159/180));
}

double point::angleTo(point p)
{
	return atan2(p.y,p.x) - atan2(y,x);
}

void point::normalize()
{
	double length = x*x + y*y;
	x /= length;
	y /= length;
}

std::string point::toString()
{
	return boost::lexical_cast<std::string>((nround(x))) + "," + boost::lexical_cast<std::string>(nround(y));
}