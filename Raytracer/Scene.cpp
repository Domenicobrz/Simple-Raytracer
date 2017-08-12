#include "common_math.h"
#include "Camera.h"
#include "Scene.h"
#include "Material.h"
#include "vector"

Scene::Scene() {

};

vec3 Scene::compute(int index) {
	Ray ray = camera.getCameraRayFromIndex(index);

	std::vector<vec3> radiance;
	vec3 accucolor(0.f, 0.f, 0.f);
	vec3 mask(1.f, 1.f, 1.f);

	/*int shit = 138992;
	if (index == shit) {
		int y = 0;
		return vec3(1.0f, 0.0f, 0.0f);
	}
	else {
		return vec3(0.0f);
	}*/

	const int BOUNCES = 10;

	/* bounces forloop - filling the radiance array */
	for (int b = 0; b < BOUNCES; b++) {

		float minT = INFINITY; int primIndex;
		for (int i = 0; i < primitives.size(); i++) {
			float t = primitives[i]->intersect(ray);

			if (t < minT) {
				minT = t;
				primIndex = i;
			}
		}

		if (minT != INFINITY) {
											/* error bound */
			vec3 hitPoint = ray.o + ray.d * (minT * 0.9999f);

			Material* material = primitives[primIndex]->getMaterial();
			radiance.push_back(material->compute(primitives[primIndex], hitPoint, ray));
			accucolor += material->emissive();
		}

		if (minT == INFINITY || b == BOUNCES - 1) {
			float ty = ray.d.y * 0.5f + 0.5f;
			float tx = ray.d.x * 0.5f + 0.5f;
			float r = (1.0f - tx) * 1.0f + tx * 0.0f; //* 0.5f;
			float g = (1.0f - ty) * 1.0f + ty * 0.0f; //* 0.7f;
			float b = (1.0f - ty) * 1.0f + ty * 0.2f; //* 1.0f;
			vec3 col = vec3(r, g, b);

			//if (dot(ray.d, vec3(-1, 1, 0)) > 0.99) col = vec3(r, g, b);
			//else col = vec3(0, 0, 0);

			//float dott = pow(max(dot(ray.d, vec3(-1.0f, 1.0f, 0.0f)), 0.0f), 3.0f);
			//vec3 col = vec3(1.0f, 0.5f, 0.5f) * dott;

			radiance.push_back(col + accucolor);
			break;
		}
	}

	/* radiance array filled, compute final color */
	for (int i = radiance.size() - 1; i >= 0; i--) {
		mask *= radiance[i];
	}

	return mask;
}

vec3 Scene::compute2(int index) {
	Ray ray = camera.getCameraRayFromIndex(index);

	std::vector<vec3> radiance;
	vec3 accucolor(0.f, 0.f, 0.f);
	vec3 mask(1.f, 1.f, 1.f);

	const int BOUNCES = 10;

	/* bounces forloop - filling the radiance array */
	for (int b = 0; b < BOUNCES; b++) {

		float minT = INFINITY; int primIndex;
		for (int i = 0; i < primitives.size(); i++) {
			float t = primitives[i]->intersect(ray);

			if (t < minT) {
				minT = t;
				primIndex = i;
			}
		}

		if (minT != INFINITY) {
			/* error bound */
			vec3 hitPoint = ray.o + ray.d * (minT * 0.9999f);

			Material* material = primitives[primIndex]->getMaterial();
			accucolor += mask * material->emissive();

			mask *= material->compute(primitives[primIndex], hitPoint, ray);
			mask *= 1.2f; //fudge factor
		}

		if (minT == INFINITY || b == BOUNCES - 1) {
			float ty = ray.d.y * 0.5f + 0.5f;
			float tx = ray.d.x * 0.5f + 0.5f;
			float r = (1.0f - tx) * 1.0f + tx * 0.0f; //* 0.5f;
			float g = (1.0f - ty) * 1.0f + ty * 0.0f; //* 0.7f;
			float b = (1.0f - ty) * 1.0f + ty * 0.2f; //* 1.0f;
			//vec3 col = vec3(r, g, b);


			//float dott = pow(max(dot(ray.d, vec3(-1.0f, 1.0f, 0.0f)), 0.0f), 3.0f);
			//vec3 col = vec3(1.0f, 0.5f, 0.5f) * dott;
			//accucolor += col * mask;
			break;
		}
	}

	return accucolor;
}

void Scene::addPrimitive(Primitive* prim) {
	primitives.push_back(prim);
}
