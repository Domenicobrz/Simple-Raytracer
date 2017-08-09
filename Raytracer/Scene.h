#ifndef SCENE_h
#define SCENE_h

#include "common_math.h"
#include "Camera.h"

class Scene {
public:
	Scene();

	/* computes pixel color from index */
	vec3 compute(int);

	/* public members */
	Camera camera;
};

#endif