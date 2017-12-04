#include "Material.h"

Material::Material() { };
Material::Material(vec3 color) : color(color), emissive(vec3(0.0f)) { };
Material::Material(vec3 color, vec3 emissive) : color(color), emissive(emissive) { };

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

vec3 Material::getShininess(vec3 uvs, Primitive* prim) {
	// if a texture manager is allocated
	if (specular != nullptr) {
		return specular->getColor(uvs, prim);
	}

	// returns all diffuse
	return vec3(0.0f, 0.0f, 0.0f);
}

vec3 Material::getEmissive(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv) {
	if (temissive != nullptr) {
		return temissive->getColor(vec3(uv, 0.0f), prim);
	}
	
	return emissive;
}