#include "Light.h"

LightMaterial::LightMaterial() : Material() { };
LightMaterial::LightMaterial(vec3 color) : Material(color) { };
LightMaterial::LightMaterial(TextureManager* tm) : Material(tm) { };

vec3 LightMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray) {

	vec3 normal = primitive->normalAtPoint(hitPoint);
	if (dot(normal, ray.d) > 0) normal = -normal;

	vec3 hitplusnormal = hitPoint + normal;

	vec3 randomPointOnUnitSphere;

	for (;;) {
		float x = rnd() * 2.0f - 1.0f;
		float y = rnd() * 2.0f - 1.0f;
		float z = rnd() * 2.0f - 1.0f;

		vec3 p(x, y, z);

		if (dot(p, p) <= 1.0f) {
			randomPointOnUnitSphere = p;
			break;
		}
	}

	
	//vec3 randomPointOnUnitSphere = normalize(vec3(x, y, z)) * rnd();
	ray.o = hitPoint;
	ray.d = normalize((hitplusnormal + randomPointOnUnitSphere) - hitPoint);

			// remember: vec3(1.0) means the light didn't "absorb" anything
			// the color of he light will be decided by the emissive 
	vec3 mask = vec3(1.0) * dot(normal, ray.d);

	//return vec3(1.0);
	return mask;
}

vec3 LightMaterial::emissive(Primitive* prim, vec3 hitPoint, Ray& ray) {
	return getColor(hitPoint);
}