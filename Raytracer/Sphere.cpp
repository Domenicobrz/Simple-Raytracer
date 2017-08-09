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
		float t = (-b - sqrt(discriminant)) / (2.0f * a);
		if (t > 0.000001f) {
			return t;
		}

		t = (-b + sqrt(discriminant)) / (2.0f * a);
		if (t > 0.000001f) {
			return t;
		}

		/* backward intersection */
		if (t < 0.0f) return INFINITY;
	}
}