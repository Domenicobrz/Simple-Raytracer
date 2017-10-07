#include "CornellBoxGeometry.h"
#include "Triangle.h"
#include "Lambert.h"
#include "Glossy.h"

CornellBoxGeometry::CornellBoxGeometry() : Geometry() {
	transform = mat4();
};

CornellBoxGeometry::CornellBoxGeometry(vec3 translate, vec3 scale, vec3 rotate, float angle) : Geometry() {
	transform = mat4();

	transform = glm::translate(transform, translate);
	transform = glm::rotate(transform, angle, rotate);
	transform = glm::scale(transform, scale);
};

CornellBoxGeometry::CornellBoxGeometry(mat4 transform) : Geometry() {
	this->transform = transform;
};



void CornellBoxGeometry::setMaterial(Material* mat) {
	material = mat;
};

void CornellBoxGeometry::concatGeometry(std::vector<Primitive*> & primitives) {
	// negy
	Triangle* t1 = new Triangle(vec3(-1, -1, -1), vec3(-1, -1, 1), vec3(1, -1, -1));
	Triangle* t2 = new Triangle(vec3(1, -1, 1), vec3(-1, -1, 1), vec3(1, -1, -1));
	t1->transform(transform); t2->transform(transform);
	t1->material = material; t2->material = material;
	primitives.push_back(t1); primitives.push_back(t2);

	//negx
	Triangle* t3 = new Triangle(vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(-1, -1, 1));
	Triangle* t4 = new Triangle(vec3(-1, 1, 1), vec3(-1, 1, -1), vec3(-1, -1, 1));
	t3->transform(transform); t4->transform(transform);
	Material* green = new LambertMaterial(vec3(0.25, 1, 0.25));
	t3->material = green; t4->material = green;
	primitives.push_back(t3); primitives.push_back(t4);

	//posz
	Triangle* t5 = new Triangle(vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, -1, 1));
	Triangle* t6 = new Triangle(vec3(1, 1, 1), vec3(-1, 1, 1), vec3(1, -1, 1));
	t5->transform(transform); t6->transform(transform);
	t5->material = material; t6->material = material;
	primitives.push_back(t5); primitives.push_back(t6);

	//posx
	Triangle* t7 = new Triangle(vec3(1, 1, 1), vec3(1, 1, -1), vec3(1, -1, 1));
	Triangle* t8 = new Triangle(vec3(1, 1, -1), vec3(1, -1, 1), vec3(1, -1, -1));
	t7->transform(transform); t8->transform(transform);
	Material* red = new LambertMaterial(vec3(1, 0.25, 0.25));
	t7->material = red; t8->material = red;
	primitives.push_back(t7); primitives.push_back(t8);

	//posy
	Triangle* t9 = new Triangle(vec3(-1, 1, -1), vec3(-1, 1, 1), vec3(1, 1, -1));
	Triangle* t10 = new Triangle(vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, 1, 1));
	t9->transform(transform); t10->transform(transform);
	t9->material = material; t10->material = material;
	primitives.push_back(t9); primitives.push_back(t10);

	//negz
	//Triangle* t11 = new Triangle(vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(1, -1, -1));
	//Triangle* t12 = new Triangle(vec3(-1, 1, -1), vec3(1, -1, -1), vec3(1, 1, -1));
	//t11->transform(transform); t12->transform(transform);
	//t11->material = material; t12->material = material;
	//primitives.push_back(t11); primitives.push_back(t12);
};