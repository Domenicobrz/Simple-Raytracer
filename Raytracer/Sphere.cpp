#include "Sphere.h"

Sphere::Sphere() : Primitive() {

};

Sphere::Sphere(vec3 center, float radius) : Primitive(), center(center), radius(radius) {

}

float Sphere::intersect(Ray ray)  {

	vec3 oc = ray.o - center;
	float a = dot(ray.d, ray.d);
	float b = 2.0f * dot(oc, ray.d);
	float c = dot(oc, oc) - radius * radius;
	
	float discriminant = b*b - 4*a*c;



	/* no intersection */
	if (discriminant <= 0) return INFINITY;
	


	if (discriminant > 0) {
		float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
		if (t1 > 0.0000001f) {
			return t1;
		}

		float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
		if (t2 > 0.0000001f) {
			int y = 0;

			return t2;
		}

		/* backward intersection */
		return INFINITY;
	}
}

vec3 Sphere::normalAtPoint(vec3 point)  {
	return normalize(point - center);
}

Material* Sphere::getMaterial()  {
	return material;
}