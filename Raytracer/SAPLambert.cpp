#include "SAPLambert.h"

SAPLambertMaterial::SAPLambertMaterial() : Material() { };
SAPLambertMaterial::SAPLambertMaterial(vec3 color) : Material(color) { };

vec3 SAPLambertMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray, vec2 uv) {

	vec3 normal = primitive->normalAtPoint(hitPoint);
	if (dot(normal, ray.d) > 0) normal = -normal;


	float theta = asin(sqrtf(1 - rnd() * rnd()));
	float phi = 2 * M_PI * rnd();


	// create a coordinate system based on the normal - scy = spherical coordinate system y
	vec3 scy = normal;
	vec3 upv = vec3(0, 1, 0);
	if (dot(normal, upv) == 1.0f) {
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

	// isnan() check to avoid dead pixels
	if (isnan(newdir.x) || isnan(newdir.y) || isnan(newdir.z)) {
		newdir = vec3(0, 1, 0);
	}

	ray.o = hitPoint;
	ray.d = normalize(newdir);


	vec3 mask = getColor(vec3(uv, 0.0f), primitive) * dot(normal, ray.d) * 2.0f;

	//return color;
	return mask;
}
