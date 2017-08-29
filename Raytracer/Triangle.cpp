#include "Triangle.h"

Triangle::Triangle() : Primitive() { }

Triangle::Triangle(vec3 v0, vec3 v1, vec3 v2) : Primitive(), v0(v0), v1(v1), v2(v2) {
	
	vec3 v1minusv0 = v1 - v0;
	vec3 v2minusv0 = v2 - v0;
	
	normal = normalize(cross(v1minusv0, v2minusv0));

	/* TODO: overload += operator for AABB */
	boundingBox = AABB(v0, v0);
	boundingBox = boundingBox + v1;
	boundingBox = boundingBox + v2;
}

void Triangle::transform(mat4 transform) {
	v0 = vec3(transform * vec4(v0.x, v0.y, v0.z, 1));
	v1 = vec3(transform * vec4(v1.x, v1.y, v1.z, 1));
	v2 = vec3(transform * vec4(v2.x, v2.y, v2.z, 1));
	
	vec3 v1minusv0 = v1 - v0;
	vec3 v2minusv0 = v2 - v0;

	normal = normalize(cross(v1minusv0, v2minusv0));

	/* TODO: overload += operator for AABB */
	boundingBox = AABB(v0, v0);
	boundingBox = boundingBox + v1;
	boundingBox = boundingBox + v2;
}

float Triangle::intersect(Ray ray) {
	
	// intersection with plane 
	float planet = dot(normal, v0 - ray.o) / dot(normal, ray.d);
	if (planet < 0.0f) return INFINITY;

	// intersection point
	vec3 hitP = ray.o + planet * ray.d;
	
	vec3 u = v1 - v0;
	vec3 v = v2 - v0;
	vec3 w = hitP - v0;

	float udotv = dot(u, v);
	float wdotv = dot(w, v);
	float wdotu = dot(w, u);
	float udotu = dot(u, u);
	float vdotv = dot(v, v);
	float denominator = udotv * udotv - udotu * vdotv;

	/*
		  (u.v)(w.v) - (v.v)(w.u)
	s =   -----------------------
		   (u.v)^2 - (u.u)(v.v)
	*/
	float s = (udotv*wdotv - vdotv*wdotu) / denominator;

	/*
	      (u.v)(w.u) - (u.u)(w.v)
	t =   -----------------------
	       (u.v)^2 - (u.u)(v.v)
	*/
	float t = (udotv*wdotu - udotu*wdotv) / denominator;

	// we hit the triangle if  s >= 0, t >= 0    &    s + t <= 1
	// http://geomalgorithms.com/a06-_intersect-2.html
	if (s >= 0 && t >= 0 && (s + t) <= 1) {
		return planet;
	}

	return INFINITY;
}

vec3 Triangle::normalAtPoint(vec3 hitPoint) {
	return normal;
}

Material* Triangle::getMaterial() {
	return material;
}

AABB Triangle::getAABB() {
	return boundingBox;
}

vec3 Triangle::getV0() {
	return v0;
}

vec3 Triangle::getV1() {
	return v1;
}

vec3 Triangle::getV2() {
	return v2;
}