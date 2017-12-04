#include "Phong.h"

PhongMaterial::PhongMaterial() : Material() { };
PhongMaterial::PhongMaterial(vec3 color) : Material(color) { };
PhongMaterial::PhongMaterial(vec3 color, float kd, float ks, float shininess) : Material(color), ks(ks), kd(kd), shininess(shininess) { };

int pass() {
	return 0;
}

vec3 PhongMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray, vec2 uv) {

	vec3 normal = primitive->normalAtPoint(hitPoint);
	if (dot(normal, ray.d) > 0) normal = -normal;



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


	// diffuse sample
	if (u < _kd) {
		theta = acos(sqrt(rnd()));
		phi = 2 * M_PI * rnd();
	}

	// specular sample - check the reference to understand where these calculations are derived
	if ((u >= _kd) && (u < _kd + _ks)) {
		// angle between normal and outgoing ray
		theta = acos(dot(normal, -ray.d));

		float alpha = acos(pow(rnd(), 1 / (_shininess + 1)));
		phi = 2.0f * M_PI * rnd();

		// new angle of the incoming ray
		theta += alpha;
	}

	// return zero contribution
	if (u > (_ks + _kd)) {
		return vec3(0, 0, 0);
	}



	// create a coordinate system based on the normal - scy = spherical coordinate system y
	vec3 scy = normal;
	vec3 scx = normalize(cross(vec3(0, 1, 0), scy));
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

	// isnan() check to avoid dead pixels
	if (isnan(newdir.x) || isnan(newdir.y) || isnan(newdir.z)) {
		newdir = vec3(0, 1, 0);
	}

	ray.o = hitPoint;
	ray.d = normalize(newdir);


	vec3 mask = getColor(vec3(uv, 0.0f), primitive) * dot(normal, ray.d);

	//return color;
	return mask;
}
