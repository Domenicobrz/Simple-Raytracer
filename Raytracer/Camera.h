#ifndef CAMERA_h
#define CAMERA_h

#include "common_math.h"
#include "Ray.h"

class Camera {
public:
	Camera();
	Camera(int, int);

	/* pixel index is passed in this function */
	Ray getCameraRayFromIndex(int index);

	int FOV;

private:
	int screenWidth;
	int screenHeight;
	float screenRatio;

};

#endif