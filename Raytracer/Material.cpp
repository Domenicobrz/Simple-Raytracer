#include "Material.h"

Material::Material() { };

vec3 Material::compute(Primitive* prim, vec3 hitPoint, Ray& ray) {
	return vec3();
};

vec3 Material::emissive() {
	return vec3(0.0f);
}