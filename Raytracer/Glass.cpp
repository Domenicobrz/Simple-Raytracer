#include "Glass.h"

GlassMaterial::GlassMaterial() : Material() { }
GlassMaterial::GlassMaterial(vec3 color, float factor, float refractionIndex) : Material(), color(color), 
																							factor(factor), 
																							refractionIndex(refractionIndex) {
}

vec3 GlassMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray) {
	vec3 normal = primitive->normalAtPoint(hitPoint);

	if (dot(normal, ray.d) > 0) {
		normal = -normal;
	}

	vec3 refractedDir = refract(ray.d, normal, 1.0f / refractionIndex);


	//vec3 refrTest = refract(vec3(0.7f, -0.7f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 1.4f);

	vec3 randomPointOnUnitSphere;
	for (;;) {
		float x = rnd() * 2.0f - 1.0f;
		float y = rnd() * 2.0f - 1.0f;
		float z = rnd() * 2.0f - 1.0f;

		vec3 p(x, y, z);

		if (dot(p, p) <= 1.0f) {
			randomPointOnUnitSphere = p * factor;
			break;
		}
	}

					   // corretting bounded error and move 
					   // hitpoint inside the primitive
	ray.o = hitPoint + ray.d * 1.0002f;
	ray.d = normalize(refractedDir + randomPointOnUnitSphere);

	vec3 mask = color * dot(normal, ray.d);

	return mask;
}

vec3 GlassMaterial::emissive() {
	return vec3(0.0f);
}