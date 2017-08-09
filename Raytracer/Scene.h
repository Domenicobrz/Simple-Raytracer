#ifndef SCENE_h
#define SCENE_h

#include <vector>
#include "common_math.h"
#include "Camera.h"
#include "Primitive.h"

class Scene {
public:
	Scene();

	/* computes pixel color from index */
	vec3 compute(int);
	void addPrimitive(Primitive*);

	/* public members */
	Camera camera;

	std::vector<Primitive*> primitives;
};

#endif