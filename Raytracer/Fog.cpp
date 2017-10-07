#include "Fog.h"

FogMaterial::FogMaterial() : Material() { };
FogMaterial::FogMaterial(vec3 color) : Material(color) { };
FogMaterial::FogMaterial(TextureManager* tm) : Material(tm) { };

vec3 FogMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray, vec2 uv) {

	// generate random valid normal
	vec3 normal;
	float dotni;
	
	for (;;) {
		float x = rnd() * 2.0f - 1.0f;
		float y = rnd() * 2.0f - 1.0f;
		float z = rnd() * 2.0f - 1.0f;

		vec3 p(x, y, z);

		if (dot(p, p) <= 1.0f) {
			normal = normalize(p);

			dotni = dot(normal, -ray.d);
			if (dotni <= 0.0f) continue;

			break;
		}
	}

	vec3 outward_normal;
	vec3 reflected = reflect(ray.d, normal);
	float ni_over_nt;
	vec3 refracted;
	float reflect_prob;
	float cosine;

	if (dot(ray.d, normal) > 0) {
		outward_normal = -normal;
		ni_over_nt = refractionIndex;
		cosine = refractionIndex * dot(ray.d, normal) / length(ray.d);
	}
	else {
		outward_normal = normal;
		ni_over_nt = 1.0f / refractionIndex;
		cosine = -dot(ray.d, normal) / length(ray.d);
	}





	if (refract(ray.d, outward_normal, ni_over_nt, refracted)) {
		float r0 = (1.0f - refractionIndex) / (1.f + refractionIndex);
		r0 *= r0;
		reflect_prob = r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
	}
	else {
		reflect_prob = 1.0f;
	}

	if (rnd() < reflect_prob) {
		// corretting bounded error and move 
		// hitpoint OUTSIDE the primitive - already done in scene.cpp
		ray.d = normalize(reflected);
	}
	else {
		// hitpoint INSIDE the primitive
		ray.o = hitPoint + ray.d * 0.2003f;
		//refracted = glm::refract(ray.d, normal, refractionIndex);
		ray.d = normalize(refracted);
	}


	//vec3 mask = getColor(vec3(uv, 0.0f), primitive) * dot(normal, ray.d);
	//vec3 mask = vec3(1.0f, 1.0f, 1.0f); // simply reflect light without absorbtion
	vec3 mask = vec3(1.0f, 0.7f, 0.7f); // simply reflect light without absorbtion

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

vec3 FogMaterial::emissive(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv) {
	return vec3(0.0f);
}