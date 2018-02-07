#pragma once

#include "common_math.h"
#include "Material.h"
#include "Primitive.h"
#include "Ray.h"


/* follows from: Libri - Programming/Rendering/Raytracing/non-books/Phong BRDF Importance Sampling.pdf 
   
   "Importance Sampling of the Phong Reflectance Model"
   -Jason Lawrence
*/
class PhongMaterial : public Material {
public:
	PhongMaterial();
	PhongMaterial(vec3);
	PhongMaterial(vec3, float kd, float ks, float shininess);

	/* returns a color and changes the ray direction */
	vec3 compute(Primitive*, vec3, Ray&, vec2);





	// (ks + kd)   needs to be  <= 1
private:
	float ks;
	float kd;
	float shininess;
};