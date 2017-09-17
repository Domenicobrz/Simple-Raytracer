#include "Material.h"

Material::Material() { };
Material::Material(vec3 color) : color(color) { };
Material::Material(TextureManager* tmp) : tm(tmp) { };

vec3 Material::compute(Primitive* prim, vec3 hitPoint, Ray& ray) {
	return vec3(0.0f);
};

vec3 Material::getColor(vec3 hitPoint, Primitive* prim) {
	// if a texture manager is allocated
	if (tm != nullptr) {
		vec3 uvs = prim->uvAtPoint(hitPoint);
		return tm->getColor(uvs);
	}
	// otherwise return the color
	return color;
}

vec3 Material::emissive(Primitive* prim, vec3 hitPoint, Ray& ray) {
	return vec3(0.0f);
}