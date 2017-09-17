#include "PlaneGeometry.h"
#include "Triangle.h"

PlaneGeometry::PlaneGeometry() : Geometry() {
	transform = mat4();
};

PlaneGeometry::PlaneGeometry(vec3 translate, vec3 scale, vec3 rotate, float angle) : Geometry() {
	transform = mat4();

	transform = glm::translate(transform, translate);
	transform = glm::rotate(transform, angle, rotate);
	transform = glm::scale(transform, scale);
};

PlaneGeometry::PlaneGeometry(mat4 transform) : Geometry() {
	this->transform = transform;
};

void PlaneGeometry::setMaterial(Material* mat) {
	material = mat;
};

void PlaneGeometry::concatGeometry(std::vector<Primitive*> & primitives) {
	// negy
	Triangle* t1 = new Triangle(vec3(-1, 0, -1), vec3(-1, 0, 1), vec3(1, 0, -1), 
								vec2(0,0),       vec2(0,1),      vec2(1,0));
	Triangle* t2 = new Triangle(vec3(-1, 0, 1),  vec3(1, 0, 1),  vec3(1, 0, -1),
								vec2(0, 1),      vec2(1, 1),     vec2(1, 0));

	t1->transform(transform); t2->transform(transform);
	t1->material = material; t2->material = material;
	primitives.push_back(t1); primitives.push_back(t2);
};