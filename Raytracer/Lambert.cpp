#include "Lambert.h"

LambertMaterial::LambertMaterial() : Material() { }; 
LambertMaterial::LambertMaterial(vec3 color) : Material(color) { };
LambertMaterial::LambertMaterial(TextureManager* tm) : Material(tm) { };

vec3 LambertMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray) {
	
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

	vec3 mask = getColor(vec3(0.0f)) * dot(normal, ray.d);

	//return color;
	return mask;
}

vec3 LambertMaterial::emissive() {
	return vec3(0.0f);
}