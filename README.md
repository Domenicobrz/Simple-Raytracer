
Simple C++ Raytracer
======

## Features

#### BRDFs
1. Modified Phong BRDF
2. Oren-Nayar Reflectance Model
3. Glossy and Glass BRDFs, though these are not yet phisically accurate
#### Textures
1. Albedo / emissive / specular textures for models
2. Textured Light Sources
3. Procedural Textures (marble noise, perlin) 
#### Misc
1. Depth Of Field
2. Fog scattering (though it needs a rewrite)
#### Models and Geometry
1. Uses Assimp to load and store models in memory
2. Classes for cornell-box geometry, plane and cubes
3. Fast BVH traversal thanks to [NanoRT](https://github.com/lighttransport/nanort)


## Pictures rendered with this simple Raytracer

**Oren-Nayar reflectance model VS perfect lambertian material**

<img src="/Raytracer/renders/33.png" style="width: 80%; margin:auto;"/>

**Phong BRDF importance sampling & albedo textures**

<img src="/Raytracer/renders/29_final.png" style="width: 80%; margin:auto;"/>

**Refractive BRDF**

<img src="/Raytracer/renders/23_2.png" style="width: 80%; margin:auto;"/>

**Depth of Field**

<img src="/Raytracer/renders/17_f.png" style="width: 80%; margin:auto;"/>

**Lambertian Spheres**

<img src="/Raytracer/renders/12_f.png" style="width: 80%; margin:auto;"/>

## How to build
**Work In Progress** - This project requires sqlite, assimp and FreeImage


