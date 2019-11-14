
Simple C++ Raytracer
======
<p align="center">
	<img src="/Raytracer/renders/33.png" style="max-width:100%;" width="650"/>
</p>

## Features

#### BRDFs
1. Modified Phong BRDF
2. Oren-Nayar Reflectance Model
3. Glossy and Glass BRDFs 
#### Textures
1. Albedo / emissive / specular textures for models
2. Textured Light Sources
3. Procedural Textures (marble noise, perlin) 
#### Misc
1. Depth Of Field
2. Fog scattering 
3. Save your renders in an intermediate format and resume rendering later (from sqlite)
#### Models and Geometry
1. Uses Assimp to load and store models in memory
2. Classes for cornell-box geometry, plane and cubes
3. Fast BVH traversal thanks to [NanoRT](https://github.com/lighttransport/nanort)


## Pictures rendered with this simple Path Tracer

<p align="center">
<img src="/Raytracer/renders/33.png" style="max-width:100%;" width="800"/><br><img src="/Raytracer/renders/p4.png" style="max-width:100%;" width="400"/><img src="/Raytracer/renders/p3.png" style="max-width:100%;" width="400"/><br><img src="/Raytracer/renders/p2.png" style="max-width:100%;" width="400"/><img src="/Raytracer/renders/p1.png" style="max-width:100%;" width="400"/>
</p>


 
## How to build
**Work In Progress** - This project requires sqlite, assimp and FreeImage


