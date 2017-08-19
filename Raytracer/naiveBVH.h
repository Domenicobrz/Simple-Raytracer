#pragma once

#include "Primitive.h"
#include "AABB.h"
#include "Ray.h"
#include <vector>

struct naiveBVHNode {
#define NAIVEBVH_INTERNALNODE 0x0001
#define NAIVEBVH_LEAFNODE	  0x0002

	/* 0x0001 internal node */
	/* 0x0002 primitive */
	short flags;
	/* could be an internal node or a primitive */
	Primitive* left;
	Primitive* right;
	naiveBVHNode* nodeleft;
	naiveBVHNode* noderight;
	AABB boundingBox;
};

struct naiveBVHHitRecord {
	float t;
	Primitive* prim;
};

class naiveBVH {
public:	
	naiveBVH();
	void createBVH(std::vector<Primitive*> * Primitives);
	naiveBVHHitRecord traverse(Ray);

private:
	naiveBVHNode* createBVHRecursive(Primitive**, int n);
	naiveBVHHitRecord traverseRecursive(naiveBVHNode*, Ray);
	naiveBVHHitRecord traverseStack(naiveBVHNode*, Ray);

	static int box_x_compare(const void* p1, const void* p2);
	static int box_y_compare(const void* p1, const void* p2);
	static int box_z_compare(const void* p1, const void* p2);

	int axis = -1;
	naiveBVHNode* rootNode;
};