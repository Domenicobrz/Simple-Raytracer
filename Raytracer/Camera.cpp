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

	aperture = 0.0f;
	focusDistance = 50.0f;
}

Camera::Camera(int screenWidth, int screenHeight, vec3 argeye, vec3 argLookAt) : screenWidth(screenWidth),
																				 screenHeight(screenHeight), 
																				 FOV(45.0f), eye(argeye), 
																				 lookAt(argLookAt) {
	screenRatio = (float)screenWidth / (float)screenHeight;

	w = normalize(vec3(lookAt - eye));
	u = cross(vec3(0, 1, 0), w);
	v = cross(w, u);

	aperture = 0.0f;
	focusDistance = 50.0f;
}

Ray Camera::getCameraRayFromIndex(int index) {

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

	/* if DOF is active */
	if (aperture > 0.0f) {
		vec3 w = normalize(dir);
		vec3 u = vec3(1, 0, 0);//cross(vec3(0, 1, 0), w);
		vec3 v = vec3(0, 1, 0);//cross(w, u);

		u = u * (rnd() * 2.0f - 1.0f) * aperture;
		v = v * (rnd() * 2.0f - 1.0f) * aperture;

		vec3 pointOnFocusPlane = w * focusDistance;
		dir = pointOnFocusPlane - (u + v);
		return Ray(eye + u + v, normalize(dir));
	}

	return Ray(eye, normalize(dir));
}