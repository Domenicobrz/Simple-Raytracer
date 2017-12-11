#include "common_math.h"
#include "Camera.h"
#include "Scene.h"
#include "Material.h"
#include "vector"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Triangle.h"





Scene::Scene() {
	fogMaterial = new FogMaterial();
};

vec3 Scene::compute2(int index) {
	//Ray ray = camera.getCameraRayFromIndex(index);

	//std::vector<vec3> radiance;
	//vec3 accucolor(0.f, 0.f, 0.f);
	//vec3 mask(1.f, 1.f, 1.f);

	//const int BOUNCES = 10;

	///* bounces forloop - filling the radiance array */
	//for (int b = 0; b < BOUNCES; b++) {
	//	
	//	/*float minT = INFINITY; int primIndex;
	//	for (int i = 0; i < primitives.size(); i++) {
	//		float t = primitives[i]->intersect(ray);

	//		if (t < minT) {
	//			minT = t;
	//			primIndex = i;
	//		}
	//	}*/

	//	naiveBVHHitRecord rec = bvh.traverse(ray);

	//	if (rec.t != INFINITY) {
	//		/* error bound */
	//		vec3 hitPoint = ray.o + ray.d * (rec.t * 0.9999f);

	//		Material* material = rec.prim->getMaterial();
	//		accucolor += mask * material->emissive(rec.prim, hitPoint, ray);

	//		mask *= material->compute(rec.prim, hitPoint, ray);
	//		mask *= 1.0f; //fudge factor
	//	}

	//	if (rec.t == INFINITY || b == BOUNCES - 1) {
	//		float ty = ray.d.y * 0.5f + 0.5f;
	//		float tx = ray.d.x * 0.5f + 0.5f;

	//		float r = (1.0f - tx) * 1.0f + tx * 0.5f; //tx * 0.0f;
	//		float g = (1.0f - ty) * 1.0f + ty * 0.7f; //ty * 0.0f;
	//		float b = (1.0f - ty) * 1.0f + ty * 1.0f; //ty * 0.2f;
	//		vec3 col = vec3(r, g, b);


	//		//float dott = pow(max(dot(ray.d, normalize(vec3(-1.0f, 1.0f, -1.0f))), 0.0f), 17.0f);
	//		//vec3 col = vec3(1.0f, 0.25f, 0.25f) * dott * 10.0f;

	//		//float dott2 = pow(max(dot(ray.d, normalize(vec3(1.0f, 1.0f, -0.5f))), 0.0f), 17.0f);
	//		//vec3 col2 = vec3(0.3f, 0.3f, 1.0f) * dott2 * 10.0f;


	//		accucolor += col * mask; // +col2 * mask;
	//		break;
	//	}
	//}

	//return accucolor;
	return vec3(); // delete this line if you're re-enabling this integrator
}

vec3 Scene::compute3(int index) {
	Ray cray = camera.getCameraRayFromIndex(index);

	std::vector<vec3> radiance;
	vec3 accucolor(0.f, 0.f, 0.f);
	vec3 mask(1.f, 1.f, 1.f);

	const int BOUNCES = 10;

	//if (index >= (600 * 436 + 121) && index < (600 * 436 + 135)) {
	//	int debug = 0;
	//	return vec3(0, 0, 1);
	//}
	//else {
	//	//return vec3();
	//}

	/* bounces forloop - filling the radiance array */
	for (int b = 0; b < BOUNCES; b++) {
		nanort::Ray<float> ray;
		ray.min_t = 0.000001f;
		ray.max_t = INFINITY;

		ray.org[0] = cray.o.x;
		ray.org[1] = cray.o.y;
		ray.org[2] = cray.o.z;

		ray.dir[0] = cray.d.x;
		ray.dir[1] = cray.d.y;
		ray.dir[2] = cray.d.z;

		nanort::TriangleIntersector<> triangle_intersector(vertsv.data(), facesv.data(), sizeof(float) * 3);
		nanort::TriangleIntersection<> isect = nanort::TriangleIntersection<>();
		bool hit = accel.Traverse(ray, triangle_intersector, &isect);
		bool hitfog = false;

		
		#ifdef FOG_SCATTERING
		if(fogScattering(cray, isect.t, hit)) {
			// fogScattering modified the t value at this point
			vec3 hitPoint = cray.o + cray.d * (isect.t * 0.9999f);

			accucolor += mask * fogMaterial->emissive(nullptr, hitPoint, cray, vec2(0));
			mask *= fogMaterial->compute(nullptr, hitPoint, cray, vec2(0));

			// prevents the material from computing this light's bounce
			hitfog = true;
		}
		#endif
		


		/* if we hit a primitive */
		/* in case there is fog scattering, hit will be set to false */
		if (hit && !hitfog) {
			                                  /* v error bound v */
			vec3 hitPoint = cray.o + cray.d * (isect.t * 0.9999f);
			vec2 uv(isect.u, isect.v);

			Primitive* prim = primitives[isect.prim_id];
			Material* material = prim->getMaterial();

			accucolor += mask * material->getEmissive(prim, hitPoint, cray, uv);
			mask *= material->compute(prim, hitPoint, cray, uv);
	

			//mask *= 2.0f; //fudge factor
		}


		/* if we didn't a surface or a fog particle, or we're at the max amount of bounces */
		if (  (!hit && !hitfog)          || b == BOUNCES - 1) {
			if (skybox != nullptr) {
				accucolor += skybox->getColor(cray.d) * mask;
				//accucolor += vec3(1.0f) * mask;  furnace test
				break;
			}

			float ty = cray.d.y * 0.5f + 0.5f;
			float tx = cray.d.x * 0.5f + 0.5f;

			//float r = (1.0f - tx) * 1.0f + tx * 0.0f; //tx * 0.5f;
			//float g = (1.0f - ty) * 1.0f + ty * 0.0f; //ty * 0.7f;
			//float b = (1.0f - ty) * 1.0f + ty * 0.2f; //ty * 1.0f;
			float r = (1.0f - tx) * 1.0f + tx * 0.5f; //tx * 0.5f;
			float g = (1.0f - ty) * 1.0f + ty * 0.7f; //ty * 0.7f;
			float b = (1.0f - ty) * 1.0f + ty * 1.0f; //ty * 1.0f;
			vec3 col = vec3(r, g, b) * 1.0f;
			//col = vec3(0, 0, 0);

			//float dott = pow(max(dot(cray.d, normalize(vec3(1.0f, -0.4f, -1.0f))), 0.0f), 13.0f);
			//vec3 col = vec3(r, g, b) * dott * 18.0f;

			//float dott2 = pow(max(dot(ray.d, normalize(vec3(1.0f, 1.0f, -0.5f))), 0.0f), 17.0f);
			//vec3 col2 = vec3(0.3f, 0.3f, 1.0f) * dott2 * 10.0f;

			//accucolor += col * mask; // +col2 * mask;
			break;
		}
	}


	return accucolor;
}

void Scene::addPrimitive(Primitive* prim) {
	primitives.push_back(prim);
}

void Scene::loadModel(const char* path, mat4 transform, Material* mat) {
	Assimp::Importer import;                        // should remove face indexes and triangulate faces
													// so we wont have to deal with vertices and index elements
	const aiScene *assscene = import.ReadFile(path, aiProcess_Triangulate/* | aiProcess_FlipUVs*/);

	if (!assscene || assscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assscene->mRootNode)
	{
		printf("ERROR::ASSIMP::%s", import.GetErrorString());
		return;
	}

	for (int i = 0; i < assscene->mNumMeshes; i++) {
		aiMesh* mesh = assscene->mMeshes[i];

		// iterating each triangle
		for (int i = 0; i < mesh->mNumVertices / 3; i++) {
			float x1 = mesh->mVertices[i * 3 + 0].x;
			float y1 = mesh->mVertices[i * 3 + 0].y;
			float z1 = mesh->mVertices[i * 3 + 0].z;

			float x2 = mesh->mVertices[i * 3 + 1].x;
			float y2 = mesh->mVertices[i * 3 + 1].y;
			float z2 = mesh->mVertices[i * 3 + 1].z;

			float x3 = mesh->mVertices[i * 3 + 2].x;
			float y3 = mesh->mVertices[i * 3 + 2].y;
			float z3 = mesh->mVertices[i * 3 + 2].z;

			float uv0s, uv0t, uv1s, uv1t, uv2s, uv2t;
			if (mesh->HasTextureCoords(0)) {
				uv0s = mesh->mTextureCoords[0][i * 3 + 0].x;
				uv0t = mesh->mTextureCoords[0][i * 3 + 0].y;

				uv1s = mesh->mTextureCoords[0][i * 3 + 1].x;
				uv1t = mesh->mTextureCoords[0][i * 3 + 1].y;

				uv2s = mesh->mTextureCoords[0][i * 3 + 2].x;
				uv2t = mesh->mTextureCoords[0][i * 3 + 2].y;
			}

			Triangle* tri1 = new Triangle(
				vec3(transform * vec4(x1, y1, z1, 1)),
				vec3(transform * vec4(x2, y2, z2, 1)),
				vec3(transform * vec4(x3, y3, z3, 1)),
				vec2(uv0s, uv0t),
				vec2(uv1s, uv1t),
				vec2(uv2s, uv2t)
				);


			tri1->material = mat;
			this->addPrimitive(tri1);
		}
	}
}

bool Scene::fogScattering(Ray ray, float & t, bool hit) {
	if (fogDensity == 0.0f) return false;

	// if we didn't hit anything, roll fogScattering over the max probability distance
	if (!hit) t = 1.0f / fogDensity;


	float invD = 1.0f / fogDensity;
	float probability = t / invD;

	float r = rnd();

	if (r < probability) {
		t = t * rnd();
		return true;
	}

	return false;
}

void Scene::add(Geometry* geom) {
	geom->concatGeometry(primitives);
}

void Scene::setSkybox(TextureSkybox* sb) {
	skybox = sb;
}