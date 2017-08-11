#include "Glossy.h"

GlossyMaterial::GlossyMaterial() : Material(), factor(1.0) { }
GlossyMaterial::GlossyMaterial(vec3 color, float factor) : Material(), color(color), factor(factor) { }

vec3 GlossyMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray) {
	vec3 normal = primitive->normalAtPoint(hitPoint);

	vec3 reflectionDir = reflect(ray.d, normal);
	float maxLenght = dot(reflectionDir, normal);
	vec3 randomPointOnUnitSphere;

	for (;;) {
		float x = rnd() * 2.0f - 1.0f;
		float y = rnd() * 2.0f - 1.0f;
		float z = rnd() * 2.0f - 1.0f;

		vec3 p(x, y, z);

		if (dot(p, p) <= 1.0f) {
			randomPointOnUnitSphere = p * factor * maxLenght;
			break;
		}
	}

	ray.o = hitPoint;
	ray.d = normalize(reflectionDir + randomPointOnUnitSphere);

	return color;
}