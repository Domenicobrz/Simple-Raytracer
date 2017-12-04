#include "Fog.h"

FogMaterial::FogMaterial() : Material() { };
FogMaterial::FogMaterial(vec3 color) : Material(color) { };

vec3 FogMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray, vec2 uv) {

	// generate random valid normal
	vec3 randomDir;
	
	for (;;) {
		float x = rnd() * 2.0f - 1.0f;
		float y = rnd() * 2.0f - 1.0f;
		float z = rnd() * 2.0f - 1.0f;

		vec3 p(x, y, z);

		if (dot(p, p) <= 1.0f) {
			randomDir = normalize(p);
			break;
		}
	}
	

	//vec3 mask = getColor(vec3(uv, 0.0f), primitive) * dot(normal, ray.d);
	//vec3 mask = vec3(1.0f, 1.0f, 1.0f); // simply reflect light without absorbtion
	vec3 mask = vec3(0.2f); // simply reflect light without absorbtion

	ray.o = hitPoint;
	ray.d = randomDir;

	//return color;
	return mask;
}


bool FogMaterial::refract(vec3 v, vec3 n, float ni_over_nt, vec3 & refracted) {
	vec3 uv = normalize(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt*dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (v - n * dt) - n * sqrt(discriminant);
		return true;
	}

	return false;
}