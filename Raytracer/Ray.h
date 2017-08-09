#ifndef RAY_h
#define RAY_h

#include "common_math.h"

struct Ray {
	Ray(vec3 origin, vec3 dir) : 
								o(origin), 
								d(dir) { };

	vec3 o;
	vec3 d;
};

#endif