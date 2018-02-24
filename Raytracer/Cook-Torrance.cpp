#include "Cook-Torrance.h"
#include <stdlib.h>
#include <iostream>

CookTorranceMaterial::CookTorranceMaterial() : Material() { };
CookTorranceMaterial::CookTorranceMaterial(vec3 color) : Material(color) { };
CookTorranceMaterial::CookTorranceMaterial(vec3 color, float kd, float ks, float roughness) : Material(color), ks(ks), kd(kd), roughness(roughness) { };


/*  Follows from: http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx   

	CookTorrance BRDF:    D*G*F   /   ( 4 * (wo dot n ) * (wi dot n))

	Our Distribution function will be GGX
*/
vec3 CookTorranceMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray, vec2 uv) {

	vec3 normal = primitive->normalAtPoint(hitPoint);
	if (dot(normal, ray.d) > 0) normal = -normal;

	vec3 reflectionDir = reflect(ray.d, normal);

	float  NoV = clamp(dot(normal, -ray.d), 0.0f, 1.0f);

	// sample a shininess texture if we have it
	float _kd = kd;
	float _ks = ks;
	float _roughness = roughness;

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
		phi = 2.0f * M_PI * rnd();
		theta = acos(sqrt(rnd()));
		pdf = (1.0f / M_PI) * cos(theta);
		brdf += (1.0f / M_PI) * _kd;
	}

	// specular sample - check the reference to understand where these calculations are derived
	if ((u >= _kd) && (u < _kd + _ks)) {

		//float r1 = rnd();
		//float theta = atan(    (_roughness*sqrt(r1)) / (sqrt(1.0f - r1))    );
		//// Eric Lafortune - Using Modified Phong Reflectance model for PBR.pdf
		//// values larger than pi/2 are clamped to pi/2 to prevent the brdf from being negative (resulting from a negative cos(a))
		//if (theta > M_PI * 0.5) theta = M_PI * 0.5;

		//phi = 2.0f * M_PI * rnd();

		float phongalpha = (1.0f - _roughness) * 18.0f;
		theta = acos(pow(rnd(), 1 / (phongalpha + 1)));
		// Eric Lafortune - Using Modified Phong Reflectance model for PBR.pdf
		// values larger than pi/2 are clamped to pi/2 to prevent the brdf from being negative (resulting from a negative cos(a))
		if (theta > M_PI * 0.5) theta = M_PI * 0.5;
		phi = 2.0f * M_PI * rnd();

		pdf = ((phongalpha + 1) / (2.0f * M_PI)) * pow(cos(theta), phongalpha);


		//phi = 2.0f * M_PI * rnd();
		//theta = acos(sqrt(rnd()));
		//pdf = (1.0f / M_PI) * cos(theta);
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

	vec3 newdir = normalize(rotmatrix * vec3(ssx, ssy, ssz));




 

	// direction points under the surface, return the normal as the direction
	if (dot(newdir, normal) < 0) {
		newdir = normal;
	}

	// isnan() check to avoid dead pixels
	if (isnan(newdir.x) || isnan(newdir.y) || isnan(newdir.z)) {
		newdir = vec3(0, 1, 0);
	}






	vec3 albedo;
	if (!procTextureFromPointPosition)
		albedo = getColor(vec3(uv, 0.0f), primitive);
	else
		albedo = getColor(hitPoint, primitive);



	vec3 mask;
	// if specular sample
	if ((u >= _kd) && (u < _kd + _ks)) {

		float cosT = clampDot(newdir, normal);

		vec3 halfVector = HalfwayVector(-ray.d, newdir);
		
		float distribution = Distribution(normal, halfVector, _roughness);
		vec3 fresnel = FresnelColor(newdir, halfVector, clampDot(halfVector, -ray.d));
		float geometry = Geometry(-ray.d, newdir, normal, halfVector);

		float denominator = 4.0f * clampDot(normal, -ray.d) * clampDot(normal, newdir);

		vec3 brdf = (distribution * fresnel * geometry) / (denominator);
		// using cosine weighted distribution instead -- for now
		// float pdf = 1.0f / (2.0f * M_PI);

		mask = (albedo * brdf * cosT) / pdf;
	}
	else {
		mask = (albedo * dot(normal, ray.d) * brdf) / pdf;
	}


	if (isnan(mask.x) || isnan(mask.y) || isnan(mask.z)) {
		printf("nan");
		mask = vec3(0.5f, 0.5f, 0.5f);
	}
	if (isinf(mask.x) || isinf(mask.y) || isinf(mask.z)) {
		printf("inf");
		mask = vec3(0.5f, 0.5f, 0.5f);
	}
	if (mask.x < -10 || mask.y < -10 || mask.z < -10) {
		printf("negative");
		mask = vec3(0.5f, 0.5f, 0.5f);
	}


	ray.o = hitPoint;
	ray.d = newdir;

	return mask;
}






float CookTorranceMaterial::Distribution(vec3 n, vec3 h, float alpha) {
	float NoH = dot(n, h);
	float alpha2 = alpha * alpha;
	float NoH2 = NoH * NoH;

	float a1 = 1.0f / (M_PI * alpha2 * NoH2 * NoH2);
	float a2 = exp((NoH2 - 1.0f) / (alpha2 * NoH2));

	return a1 * a2;
}

float CookTorranceMaterial::Geometry(vec3 wo, vec3 wi, vec3 n, vec3 h) {
	float NoH = dot(n, h);
	
	float a1 = (2.0f * NoH * dot(n, wo)) / (dot(wo, h));
	float a2 = (2.0f * NoH * dot(n, wi)) / (dot(wo, h));
	
	return min(1.0f, min(a1, a2));
}

float CookTorranceMaterial::partialFresnel(vec3 wi, vec3 h, float costheta, float n, float k) {
	float costheta2 = costheta * costheta;

	float n2pk2 = n*n + k*k;

	float r2par = (n2pk2 * costheta2 - (2 * n) * costheta + 1.0f) /
		(n2pk2 * costheta2 + (2 * n) * costheta + 1.0f);
	float r2per = (n2pk2 - (2 * n) * costheta + costheta2) /
		(n2pk2 + (2 * n) * costheta + costheta2);

	return (r2par + r2per) / 2.0f;
}

vec3 CookTorranceMaterial::FresnelColor(vec3 wi, vec3 h, float costheta) {
	float rn = 1.42f, rk = 7.66f;
	float fresnelRed = partialFresnel(wi, h, costheta, rn, rk);
	
	float gn = 0.93f, gk = 6.30f;
	float fresnelGreen = partialFresnel(wi, h, costheta, gn, gk);
	
	float bn = 0.64f, bk = 5.39f;
	float fresnelBlue = partialFresnel(wi, h, costheta, bn, bk);

	return vec3(fresnelRed, fresnelGreen, fresnelBlue);
}



float CookTorranceMaterial::Fresnel(vec3 wi, vec3 h, float costheta) {
	//// Schlick's approximation
	//// https://en.wikipedia.org/wiki/Schlick's_approximation

	//vec3 cspec = vec3(0.5f);
	//return cspec + (1.0f - cspec) * (1.0f - pow(clampDot(wi, h), 5.0f));


	// from http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx
	float IOR = 2.0972f;
	float k = 4.7f;     // absorption coefficient


	// numerator parts
	float a2 = (IOR - 1.0f) * (IOR - 1.0f);
	float a3 = 4 * IOR * pow(1.0f - costheta, 5.0);
	float a4 = k * k;

	// denominator parts
	float b = (IOR + 1.0f) * (IOR + 1.0f);

	// final formula
	return (a2 + a3 + a4) / (b + a4);

	//return 1.0f;
}