#include "Material.h"

Material::Material() { };
Material::Material(vec3 color) : color(color) { };
Material::Material(TextureManager* tmp) : tm(tmp) { };

vec3 Material::compute(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv) {
	return vec3(0.0f);
};

vec3 Material::getColor(vec3 uvs, Primitive* prim) {
	// if a texture manager is allocated
	if (tm != nullptr) {
		return tm->getColor(uvs, prim);
	}
	// otherwise return the color
	return color;
}

vec3 Material::emissive(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv) {
	return vec3(0.0f);
}