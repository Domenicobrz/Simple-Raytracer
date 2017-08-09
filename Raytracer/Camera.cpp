#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera() {

};

Camera::Camera(int screenWidth, int screenHeight) : screenWidth(screenWidth), 
													screenHeight(screenHeight),
													FOV(45) {
	screenRatio = (float)screenWidth / (float)screenHeight;
}

vec3 Camera::getCameraRayFromIndex(int index) {
	// 0 ... 0.999 ( 'left side' of the last pixel square)
	float s = (index % screenWidth) / (float)screenWidth;
	float t = (index / screenWidth) / (float)screenHeight;

	// hwh at z == 1
	float hwhz1 = tan((45.0f / 2.0f) * M_PI / 180.0f);
	float hwwz1 = hwhz1 * (screenRatio);

	// -1 ... +1
	float ns = s * 2.0f - 1.0f;
	float nt = t * 2.0f - 1.0f;

	float x = ns * hwwz1;
	float y = nt * hwhz1;
	float z = 1;

	return vec3(x, y, z);
}