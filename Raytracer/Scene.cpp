#include "common_math.h"
#include "Camera.h"
#include "Scene.h"

Scene::Scene() {

};

vec3 Scene::compute(int index) {
	Ray ray = camera.getCameraRayFromIndex(index);

	return ray.d;
}