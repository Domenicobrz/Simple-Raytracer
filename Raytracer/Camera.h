#ifndef CAMERA_h
#define CAMERA_h

#include "common_math.h"
#include "Ray.h"

class Camera {
public:
	Camera();
	Camera(int, int);
	Camera(int, int, vec3, vec3);

	/* pixel index is passed in this function */
	Ray getCameraRayFromIndex(int index);

	float FOV;

	vec3 eye;
	vec3 lookAt;
	vec3 w, u, v;

private:
	int screenWidth;
	int screenHeight;

	float screenRatio;
};

#endif