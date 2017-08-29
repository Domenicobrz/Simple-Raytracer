#pragma once

#include "Geometry.h"
#include "Material.h";

class CornellBoxGeometry : public Geometry {
public:
	CornellBoxGeometry();
	CornellBoxGeometry(vec3 translate, vec3 scale = vec3(1, 1, 1), vec3 rotate = vec3(0, 1, 0), float angle = 0);
	CornellBoxGeometry(mat4);

	void concatGeometry(std::vector<Primitive*> &);
	void setMaterial(Material* mat);

private:
	mat4 transform;
	Material* material;
};