#ifndef point_h_
#define point_h_
#include <iostream>
#include <math.h>
#include <cmath>
#include <ClanLib/core.h>

namespace SL
{
/// Simple point / vector implementation
struct point
{
public:
	///Construct a point with the given x,y coordinates
	/** \param x the x coordinate
		\param y the y coordinate
	*/
	point::point(float x,float y);
	/// Construct a point with coordinates at 0,0
	point::point();
	/// Create a new point at the given offset from this point
	point point::offsetRect(float ox, float oy);
	/// Create a new point at the given offset from this point
	point point::offsetPolar(CL_Angle heading, float distance);
	/// Calculate the angle (in radians) to another point
	float angleTo(point p);
	/// The x coordinate of this point
	float x;
	/// The y coordinate of this point
	float y;
	/// Generate a string from the point, for easy display
	std::string point::toString();
	/// Retain the direction but make the magnitude 1
	void normalize();
};

}
#endif