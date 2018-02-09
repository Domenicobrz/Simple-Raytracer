#include "Phong.h"
#include <stdlib.h>
#include <iostream>

PhongMaterial::PhongMaterial() : Material() { };
PhongMaterial::PhongMaterial(vec3 color) : Material(color) { };
PhongMaterial::PhongMaterial(vec3 color, float kd, float ks, float shininess) : Material(color), ks(ks), kd(kd), shininess(shininess) { };

vec3 PhongMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray, vec2 uv) {

	vec3 normal = primitive->normalAtPoint(hitPoint);
	if (dot(normal, ray.d) > 0) normal = -normal;

	vec3 reflectionDir = reflect(ray.d, normal);


	// sample a shininess texture if we have it
	float _kd = kd;
	float _ks = ks;
	float _shininess = shininess;

	if (specular != nullptr) {
		vec3 shininessFromTexture = getShininess(vec3(uv, 0.0f), primitive);
		_kd = 1.0f - shininessFromTexture.r;
		_ks = 1.0f - _kd;
	}


	// draw a random uniform variable to decide whether this is going to be a diffuse (u < kd) 
	// or specular (kd <= u < kd + ks) sample.
	// in case (u >= kd + ks) return nothing.
	float u = rnd();

	// theta and phi are drawn from an emisphere where the normal points upward
	float theta = 0.0f;
	float phi = 0.0f;
	float brdf = 0.0f;
	float pdf = 0.0f;

	// diffuse sample
	if (u < _kd) {
		phi   =  2.0f * M_PI * rnd();
		theta =  acos(sqrt(rnd()));
		pdf   =  (1.0f / M_PI) * cos(theta);
		brdf  += (1.0f / M_PI) * _kd;
	}

	// specular sample - check the reference to understand where these calculations are derived
	if ((u >= _kd) && (u < _kd + _ks)) {
		// angle between normal and outgoing ray
		//theta = acos(dot(normal, -ray.d));

		float theta = acos(  pow(rnd(),       1 / (_shininess + 1)  ));
		// Eric Lafortune - Using Modified Phong Reflectance model for PBR.pdf
		// values larger than pi/2 are clamped to pi/2 to prevent the brdf from being negative (resulting from a negative cos(a))
		if (theta > M_PI * 0.5) theta = M_PI * 0.5;

		phi = 2.0f * M_PI * rnd();

		pdf  = ((_shininess + 1) / (2.0f * M_PI)) * pow(cos(theta), _shininess);
		brdf = ((_shininess + 2) / (2.0f * M_PI)) * pow(cos(theta), _shininess) * _ks;
	}

	// return zero contribution
	if (u > (_ks + _kd)) {
		return vec3(0, 0, 0);
	}


	// create a coordinate system based on the REFLECTED DIRECTION - scy = spherical coordinate system y
	vec3 scy;
	if (u < _kd) scy = normal;			// diffuse sample
	else		 scy = reflectionDir;	// specular sample
	
	vec3 upv = vec3(0, 1, 0);
	if (dot(scy, upv) > 0.99f) {
		upv = vec3(1, 0, 0);
	}
	// THIS CROSS PRODUCT MAY GET CLOSE TO INFINITY
	// THIS CROSS PRODUCT MAY GET CLOSE TO INFINITY
	// THIS CROSS PRODUCT MAY GET CLOSE TO INFINITY
	vec3 scx = normalize(cross(upv, scy));
	vec3 scz = normalize(cross(scx, scy));

	// ssx = spherical sample x
	float ssx = cos(phi) * sin(theta);
	float ssy = cos(theta);
	float ssz = sin(phi) * sin(theta);

	// construct transformation matrix
	mat3 rotmatrix = mat3(0.0f);
	// setting each column (http://www.c-jump.com/bcc/common/Talk3/Math/GLM/GLM.html#W01_0020_vector_and_matrix_con)
	rotmatrix[0] = scx;
	rotmatrix[1] = scy;
	rotmatrix[2] = scz;

	vec3 newdir = rotmatrix * vec3(ssx, ssy, ssz);





	/**  Integrity checks **/
	/**  Integrity checks **/

	// direction points under the surface, return the normal as the direction
	if (dot(newdir, normal) < 0) {
		newdir = normal;
	}

	// isnan() check to avoid dead pixels
	if (isnan(newdir.x) || isnan(newdir.y) || isnan(newdir.z)) {
		newdir = vec3(0, 1, 0);
	}
	
	// brdf / pdf sanity check
	if (pdf <= 0.0f || brdf <= 0.0f) {
		pdf = 1.0f;
		brdf = 1.0f;
	}
	// avoids divisions by a too small of a number
	if (pdf < 0.0001f) {
		pdf = 0.0001f;
	}

	/**  Integrity checks - END **/
	/**  Integrity checks - END **/






	ray.o = hitPoint;
	ray.d = normalize(newdir);

	vec3 albedo;
	if (!procTextureFromPointPosition)
		albedo = getColor(vec3(uv, 0.0f), primitive);
	else
		albedo = getColor(hitPoint, primitive);


	vec3 mask = (albedo * dot(normal, ray.d) * brdf) / pdf;
	

	if (isnan(mask.x) || isnan(mask.y) || isnan(mask.z)) {
		printf("nan");
	}
	if (isinf(mask.x) || isinf(mask.y) || isinf(mask.z)) {
		printf("inf");
	}
	if (mask.x < -10 || mask.y < -10 || mask.z < -10) {
		printf("negative");
	}

	return mask;
}
