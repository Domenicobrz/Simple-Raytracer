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

			//float shit = length(hitPoint - vec3(0,0,50));
			//if (shit < 10.00001f) continue;

			Material* material = primitives[primIndex]->getMaterial();

			radiance.push_back(material->compute(primitives[primIndex], hitPoint, ray));
		}

		if (minT == INFINITY || b == BOUNCES - 1) {
			float ty = ray.d.y * 0.5f + 0.5f;
			float tx = ray.d.x * 0.5f + 0.5f;
			float r = (1.0f - tx) * 1.0f + tx * 0.0f; //* 0.5f;
			float g = (1.0f - ty) * 1.0f + ty * 0.0f; //* 0.7f;
			float b = (1.0f - ty) * 1.0f + ty * 0.2f; //* 1.0f;

			//vec3 col;
			//if (dot(ray.d, vec3(-1, 1, 0)) > 0.99) col = vec3(r, g, b);
			//else col = vec3(0, 0, 0);

			//float dott = pow(max(dot(ray.d, vec3(-1.0f, 1.0f, 0.0f)), 0.0f), 3.0f);
			//vec3 col = vec3(1.0f, 0.5f, 0.5f) * dott;
			vec3 col = vec3(r,g,b);


			radiance.push_back(col);
			break;
		}
	}


	/* radiance array filled, compute final color */
	for (int i = radiance.size() - 1; i >= 0; i--) {
		mask *= radiance[i];
	}

	return mask;
}

void Scene::addPrimitive(Primitive* prim) {
	primitives.push_back(prim);
}
