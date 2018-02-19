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
		return tm->getColor(uvs, prim, true);
	}
	// otherwise return the color
	return color;
}

vec3 Material::getShininess(vec3 uvs, Primitive* prim) {
	if (specular != nullptr) {
		// emissive and specular textures shouldn't be gamma corrected
		return specular->getColor(uvs, prim, false);
	}
	return vec3(0.0f, 0.0f, 0.0f);
}

vec3 Material::getEmissive(Primitive* prim, vec3 hitPoint, Ray& ray, vec2 uv) {
	if (temissive != nullptr) {
		// emissive and specular textures shouldn't be gamma corrected
		return temissive->getColor(vec3(uv, 0.0f), prim, false);
	}
	return emissive;
}

vec3 Material::HalfwayVector(vec3 wo, vec3 wi) {
	return normalize(wo + wi);
}

float Material::clampDot(vec3 wo, vec3 wi) {
	return clamp(dot(wo, wi), 0.0f, 1.0f);
}