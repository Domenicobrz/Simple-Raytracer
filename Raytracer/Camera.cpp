#include "Camera.h"
#include "Ray.h"


Camera::Camera() {

};

Camera::Camera(int screenWidth, int screenHeight) : screenWidth(screenWidth), 
													screenHeight(screenHeight),
													FOV(45.0f), eye(0.f,0.f,0.f), lookAt(0.f,0.f,1.f) {
	screenRatio = (float)screenWidth / (float)screenHeight;

	w = normalize(vec3(lookAt - eye));
	u = cross(vec3(0, 1, 0), w);
	v = cross(w, u);
}

Camera::Camera(int screenWidth, int screenHeight, vec3 argeye, vec3 argLookAt) : screenWidth(screenWidth),
																				 screenHeight(screenHeight), 
																				 FOV(45.0f), eye(argeye), 
																				 lookAt(argLookAt) {
	screenRatio = (float)screenWidth / (float)screenHeight;

	w = normalize(vec3(lookAt - eye));
	u = cross(vec3(0, 1, 0), w);
	v = cross(w, u);
}

Ray Camera::getCameraRayFromIndex(int index) {
	//// 0 ... 0.999 ( 'left side' of the last pixel square)
	//float s = (index % screenWidth) / (float)screenWidth;
	//float t = (index / screenWidth) / (float)screenHeight;

	//s += ((float)rand() / (float)RAND_MAX) / screenWidth;
	//t += ((float)rand() / (float)RAND_MAX) / screenHeight;

	//// hwh at z == 1
	//float hwhz1 = tan((45.0f / 2.0f) * M_PI / 180.0f);
	//float hwwz1 = hwhz1 * (screenRatio);

	//// -1 ... +1
	//float ns = s * 2.0f - 1.0f;
	//float nt = t * 2.0f - 1.0f;

	//float x = ns * hwwz1;
	//float y = nt * hwhz1;
	//float z = 1;
	//
	//vec3 dir(x, y, z);
	//vec3 origin(0.0f, 0.0f, 0.0f);

	//return Ray(origin, normalize(dir));









	// 0 ... 0.999 ( 'left side' of the last pixel square)
	float s = (index % screenWidth) / (float)screenWidth;
	float t = (index / screenWidth) / (float)screenHeight;

	s += ((float)rand() / (float)RAND_MAX) / screenWidth;
	t += ((float)rand() / (float)RAND_MAX) / screenHeight;

	// hwh at z == 1
	float hwhz1 = tan((45.0f / 2.0f) * M_PI / 180.0f);
	float hwwz1 = hwhz1 * (screenRatio);

	// -1 ... +1
	float ns = s * 2.0f - 1.0f;
	float nt = t * 2.0f - 1.0f;

	vec3 filmPoint = eye + w + (u * ns * hwwz1) + (v * nt * hwhz1);

	vec3 dir(filmPoint - eye);
	return Ray(eye, normalize(dir));
}