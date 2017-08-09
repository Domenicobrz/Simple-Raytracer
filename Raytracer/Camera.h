#pragma once

#include <glm\glm.hpp>

using namespace glm;

class Camera {
public:
	Camera();
	Camera(int, int);

	/* pixel index is passed in this function */
	vec3 getCameraRayFromIndex(int index);

	int FOV;

private:
	int screenWidth;
	int screenHeight;
	float screenRatio;

};