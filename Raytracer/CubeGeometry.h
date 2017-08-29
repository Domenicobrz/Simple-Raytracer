#pragma once

#include "Geometry.h"
#include "Material.h";

class CubeGeometry : public Geometry {
public:
	CubeGeometry();
	CubeGeometry(vec3 translate, vec3 scale = vec3(1, 1, 1), vec3 rotate = vec3(0, 1, 0), float angle = 0);
	CubeGeometry(mat4);

	void concatGeometry(std::vector<Primitive*> &);
	void setMaterial(Material* mat);

private:
	mat4 transform;
	Material* material;
};