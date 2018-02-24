#include "Oren-Nayar.h"
#include <stdlib.h>
#include <iostream>

OrenNayarMaterial::OrenNayarMaterial() : Material() { };
OrenNayarMaterial::OrenNayarMaterial(vec3 color) : Material(color) { };
OrenNayarMaterial::OrenNayarMaterial(vec3 color, float roughness) : Material(color), roughness(roughness) { };








float OrenNayarFormula(vec3 wo, vec3 wi);









/*  Follows from: http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx

CookTorrance BRDF:    D*G*F   /   ( 4 * (wo dot n ) * (wi dot n))

Our Distribution function will be GGX
*/
vec3 OrenNayarMaterial::compute(Primitive* primitive, vec3 hitPoint, Ray& ray, vec2 uv) {

	vec3 normal = primitive->normalAtPoint(hitPoint);
	if (dot(normal, ray.d) > 0) normal = -normal;

	vec3 reflectionDir = reflect(ray.d, normal);

	float  NoV = clamp(dot(normal, -ray.d), 0.0f, 1.0f);

	// sample a shininess texture if we have it
	float _roughness = roughness;

	if (specular != nullptr) {
		vec3 shininessFromTexture = getShininess(vec3(uv, 0.0f), primitive);
	}


	// create a coordinate system based on the REFLECTED DIRECTION - scy = spherical coordinate system y
	vec3 scy;
	scy = normal; // diffuse sample

	//if (u < _kd) scy = normal;			// diffuse sample
	//else		 scy = reflectionDir;	// specular sample

	vec3 upv = vec3(0, 1, 0);
	if (dot(scy, upv) > 0.99f) {
		upv = vec3(1, 0, 0);
	}
	// THIS CROSS PRODUCT MAY GET CLOSE TO INFINITY
	// THIS CROSS PRODUCT MAY GET CLOSE TO INFINITY
	// THIS CROSS PRODUCT MAY GET CLOSE TO INFINITY
	vec3 scx = normalize(cross(upv, scy));
	vec3 scz = normalize(cross(scx, scy));


	// construct transformation matrix
	mat3 rotmatrix = mat3(0.0f);
	// setting each column (http://www.c-jump.com/bcc/common/Talk3/Math/GLM/GLM.html#W01_0020_vector_and_matrix_con)
	rotmatrix[0] = scx;
	rotmatrix[1] = scy;
	rotmatrix[2] = scz;
	
	mat3 invrotmatrix = mat3(0.0f);
	invrotmatrix = transpose(rotmatrix);

	vec3 wo_on_nc = normalize(invrotmatrix * -ray.d);
	//vec2 wo_projected = vec2(wo_on_nc.x, wo_on_nc.z);





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
	phi = 2.0f * M_PI * rnd();
	theta = acos(sqrt(rnd()));
	pdf = (1.0f / M_PI) * cos(theta);

	// ssx = spherical sample x
	float ssx = cos(phi) * sin(theta);
	float ssy = cos(theta);
	float ssz = sin(phi) * sin(theta);

	vec3 newdir = normalize(rotmatrix * vec3(ssx, ssy, ssz));
	vec3 wi_on_nc = vec3(ssx, ssy, ssz);
	//vec2 wi_projected = vec2(wi_on_nc.x, wi_on_nc.z);

	brdf = OrenNayarFormula(wo_on_nc, wi_on_nc) * (1.0f / M_PI);



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



	ray.o = hitPoint;
	ray.d = newdir;



	vec3 mask;
	mask = (albedo * dot(normal, ray.d) * brdf) / pdf;



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

	return mask;
}


/* 
	ATTENZIONE!!!

	PBRT  USA UNA CONVENZIONE NELLA QUALE QUELLO CHE IO DEFINISCO L'ASSE 'Y' PER LORO E' L'ASSE 'Z',
	per cui ho swappato le variabili in cui compariva y con z e z con y dalle formule originali del libro

	Nel loro sistema di coordinate il normal punta nell'asse   'z'
	Qui invece per convenzione il normal punta nell'asse       'y'
*/

float AbsCosTheta(vec3 w) { return std::abs(w.y); }

float Cos2Theta(vec3 w) { return w.y * w.y; }

float Sin2Theta(vec3 w) {
	return max((float)0, (float)1 - Cos2Theta(w));
}
float SinTheta(vec3 w) {
	return std::sqrt(Sin2Theta(w));
}
float CosPhi(vec3 w) {
	float sinTheta = SinTheta(w);
	return (sinTheta == 0) ? 1 : clamp(w.x / sinTheta, -1.0f, 1.0f);
}
float SinPhi(vec3 w) {
	float sinTheta = SinTheta(w);
	return (sinTheta == 0) ? 0 : clamp(w.z / sinTheta, -1.0f, 1.0f);
}


float OrenNayarFormula(vec3 wo, vec3 wi) {

	float sigma = 0.5f;
	float sigma2 = sigma * sigma;
	float A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
	float B = 0.45f * sigma2 / (sigma2 + 0.09f);

	 
	float sinThetaI = SinTheta(wi);
	float sinThetaO = SinTheta(wo);



	float maxCos = 0.0f;
	if (sinThetaI > 0.0001f && sinThetaO > 0.00001f) {
		float SinPhiI = SinPhi(wi);
		float CosPhiI = CosPhi(wi);
		float SinPhiO = SinPhi(wo);
		float CosPhiO = CosPhi(wo);

		float dCos = CosPhiI * CosPhiO + SinPhiI * SinPhiO;
		maxCos = max((float) 0.0f, dCos);
	}

	float sinAlpha, tanBeta;
	if (AbsCosTheta(wi) > AbsCosTheta(wo)) {
		sinAlpha = sinThetaO;
		tanBeta = sinThetaI / AbsCosTheta(wi);
	}
	else {
		sinAlpha = sinThetaI;
		tanBeta = sinThetaO / AbsCosTheta(wo);
	}
	return (A + B*maxCos * sinAlpha * tanBeta);
}