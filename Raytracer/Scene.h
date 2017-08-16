#ifndef SCENE_h
#define SCENE_h

#include <vector>
#include "common_math.h"
#include "Camera.h"
#include "Primitive.h"
#include "naiveBVH.h"
#include "Material.h"

class Scene {
public:
	Scene();

	/* computes pixel color from index */
	vec3 compute(int);
	vec3 compute2(int);
	void addPrimitive(Primitive*);
	void loadModel(const char*, mat4 transform, Material* mat);

	/* public members */
	Camera camera;

	std::vector<Primitive*> primitives;
	naiveBVH bvh;
};

#endif