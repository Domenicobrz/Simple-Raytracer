#pragma once

#include <glm/glm.hpp>
#include "Camera.h"

using namespace glm;

class Scene {
public:
	Scene();

	/* computes pixel color from index */
	vec3 compute(int);

	/* public members */
	Camera camera;
};