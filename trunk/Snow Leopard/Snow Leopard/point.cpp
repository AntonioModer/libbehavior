#include "point.h"
#include <ClanLib/core.h>
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

point point::offsetPolar(CL_Angle heading, double distance)
{
	return point(x - distance * sin(-heading.to_radians()),y - distance * cos(-heading.to_radians()));
}

double point::angleTo(point p)
{
	return atan2(p.y,p.x) - atan2(y,x);
}

void point::normalize()
{
	double length = sqrtf(x*x + y*y);
	cout << "length: " << length << endl;
	x /= length;
	y /= length;
}

std::string point::toString()
{
	return boost::lexical_cast<std::string>(x) + "," + boost::lexical_cast<std::string>(y);
}