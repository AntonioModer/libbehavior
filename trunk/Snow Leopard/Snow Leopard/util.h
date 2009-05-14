#ifndef UTIL_H
#define UTIL_H

inline int round(double x)
{
return int(x > 0.0 ? x + 0.5 : x - 0.5);
}

#endif