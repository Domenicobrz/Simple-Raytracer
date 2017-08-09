#include "common_math.h"
#include "Camera.h"
#include "Scene.h"

Scene::Scene() {

};

vec3 Scene::compute(int index) {
	Ray ray = camera.getCameraRayFromIndex(index);

	for (int i = 0; i < primitives.size(); i++) {
		if (primitives[i]->intersect(ray) != INFINITY) return vec3(1, 0, 0);
	}

	return ray.d;
}

void Scene::addPrimitive(Primitive* prim) {
	primitives.push_back(prim);
}