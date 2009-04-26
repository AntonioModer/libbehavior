#ifndef point_h_
#define point_h_
#include <iostream>
#include <math.h>
#include <cmath>
#include <ClanLib/core.h>

namespace SL
{
	


struct point
{
public:
	point::point(double i,double j);
	point::point();
	point point::offsetRect(double ox, double oy);
	point point::offsetPolar(CL_Angle heading, double distance);
	double angleTo(point p);
	double x;
	double y;
	std::string point::toString();
	void normalize();
};

}
#endif