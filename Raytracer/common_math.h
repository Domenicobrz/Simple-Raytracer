#ifndef COMMON_h
#define COMMON_h

#include <glm\glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;

inline float rnd() {
	return (float)rand() / (float)RAND_MAX;
	//RANDOM_SEEDasfdasfas += 33.0f;
	//return fract(sin(dot(vec2(RANDOM_SEEDasfdasfas, 0.0f), vec2(12.9898, 78.233))) * 43758.5453);
}
#endif