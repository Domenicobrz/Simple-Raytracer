#ifndef SCENE_h
#define SCENE_h

#include <vector>
#include "common_math.h"
#include "Camera.h"
#include "Primitive.h"
#include "naiveBVH.h"
#include "Material.h"
#include "Geometry.h"
#include "nanort.h"

class Scene {
public:
	Scene();

	/* computes pixel color from index */
	vec3 compute(int);
	vec3 compute2(int);
	vec3 compute3(int);
	void addPrimitive(Primitive*);
	void loadModel(const char*, mat4 transform, Material* mat);

	// TODO: add needs to add both models and primitives
	void add(Geometry*);

	/* public members */
	Camera camera;

	std::vector<Primitive*> primitives;
	naiveBVH bvh;





	/* bvh test */
	nanort::TriangleMesh<float>* triangle_mesh;
	nanort::TriangleSAHPred<float>* triangle_pred;
	//nanort::TriangleIntersector<>* triangle_intersector;
	nanort::BVHAccel<float> accel;
	std::vector<float> vertsv;
	std::vector<unsigned int> facesv;
};

#endif