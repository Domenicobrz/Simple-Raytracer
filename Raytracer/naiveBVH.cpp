#include "naiveBVH.h"


naiveBVH::naiveBVH() {

}

void naiveBVH::createBVH(std::vector<Primitive*> * primitives) {
	rootNode = createBVHRecursive(primitives->data(), primitives->size());
}

naiveBVHNode* naiveBVH::createBVHRecursive(Primitive** primitives, int n) {
	axis = (++axis) % 3;

	/* sorting primtive - TODO:   STD::SORT IS *HIGHLY* RECOMMENDED */
	switch (axis) {
		case 0: {
			qsort(primitives, n, sizeof(Primitive*), &naiveBVH::box_x_compare);
			break;
		}

		case 1: {
			qsort(primitives, n, sizeof(Primitive*), &naiveBVH::box_y_compare);
			break;
		}

		case 2: {
			qsort(primitives, n, sizeof(Primitive*), &naiveBVH::box_z_compare);
			break;
		}
	}

	naiveBVHNode* returnNode = new naiveBVHNode();
	
	if (n == 1) {
		returnNode->flags = NAIVEBVH_LEAFNODE;
		returnNode->left = returnNode->right = primitives[0];
		returnNode->boundingBox = primitives[0]->getAABB();
	}
	
	if (n == 2) {
		returnNode->flags = NAIVEBVH_LEAFNODE;
		returnNode->left  = primitives[0];
		returnNode->right = primitives[1];
		returnNode->boundingBox = primitives[0]->getAABB() + primitives[1]->getAABB();
	}

	if (n > 2) {
		returnNode->flags = NAIVEBVH_INTERNALNODE;
		returnNode->nodeleft  = createBVHRecursive(primitives, n / 2);
		returnNode->noderight = createBVHRecursive(primitives + n / 2, n - n / 2);
		returnNode->boundingBox = returnNode->nodeleft->boundingBox + returnNode->noderight->boundingBox;
	}

	return returnNode;
}

int naiveBVH::box_x_compare(const void* p1, const void* p2) {
	AABB a1 = (*(Primitive**)p1)->getAABB();
	AABB a2 = (*(Primitive**)p2)->getAABB();
	
	if (a1.c0.x <  a2.c0.x) return -1;
	if (a1.c0.x == a2.c0.x) return 0;
	if (a1.c0.x >  a2.c0.x) return 1;
}

int naiveBVH::box_y_compare(const void* p1, const void* p2) {
	AABB a1 = (*(Primitive**)p1)->getAABB();
	AABB a2 = (*(Primitive**)p2)->getAABB();

	if (a1.c0.y <  a2.c0.y) return -1;
	if (a1.c0.y == a2.c0.y) return 0;
	if (a1.c0.y >  a2.c0.y) return 1;
}

int naiveBVH::box_z_compare(const void* p1, const void* p2) {
	AABB a1 = (*(Primitive**)p1)->getAABB();
	AABB a2 = (*(Primitive**)p2)->getAABB();

	if (a1.c0.z <  a2.c0.z) return -1;
	if (a1.c0.z == a2.c0.z) return 0;
	if (a1.c0.z >  a2.c0.z) return 1;
}

naiveBVHHitRecord naiveBVH::traverse(Ray ray) {
	float t = INFINITY;
	return traverseRecursive(rootNode, ray);
}

naiveBVHHitRecord naiveBVH::traverseRecursive(naiveBVHNode* node, Ray ray) {
	
	naiveBVHHitRecord rec;
	
	float t = node->boundingBox.intersect(ray);
	
	/* missed this node - don't bother checking it's child */
	if (t == INFINITY) {
		rec = { INFINITY, nullptr };
		return rec;
	}

	/* if we got here at least one of the children got hit */
	if (node->flags & NAIVEBVH_INTERNALNODE) {
		naiveBVHHitRecord p1 = traverseRecursive(node->nodeleft, ray);
		naiveBVHHitRecord p2 = traverseRecursive(node->noderight, ray);

		if (p1.t < p2.t) return p1;
		else             return p2;
	}


	/* if we got here at least one of the leaves got hit */
	float l1t = node->left->intersect(ray);
	float l2t = node->right->intersect(ray);

	
	
	if (l1t == INFINITY && l2t == INFINITY) {
		rec = { INFINITY, nullptr };
		return rec;
	}



	if (l1t < l2t) rec = { l1t, node->left };
	else           rec = { l2t, node->right };
	return rec;
}
