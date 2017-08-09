#include "Scene.h"

Scene::Scene() {

};

vec3 Scene::compute(int index) {
	return camera.getCameraRayFromIndex(index);
}