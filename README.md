
Simple C++ Raytracer
======
<p align="center">
	<img src="/Raytracer/renders/33.png" style="max-width:100%;" width="650"/>
</p>

## Features

#### BRDFs
1. Modified Phong BRDF
2. Oren-Nayar Reflectance Model
3. Glossy and Glass BRDFs (though these are not yet physically accurate)
#### Textures
1. Albedo / emissive / specular textures for models
2. Textured Light Sources
3. Procedural Textures (marble noise, perlin) 
#### Misc
1. Depth Of Field
2. Fog scattering (though it needs a rewrite)
3. Save your renders in an intermediate format and resume rendering later (from sqlite)
#### Models and Geometry
1. Uses Assimp to load and store models in memory
2. Classes for cornell-box geometry, plane and cubes
3. Fast BVH traversal thanks to [NanoRT](https://github.com/lighttransport/nanort)


## Pictures rendered with this simple Path Tracer

<p align="center">
    <img src="/Raytracer/renders/33.png" style="max-width:100%;" width="800"/>
	
	<br>
	
	<img src="/Raytracer/renders/29_final.png" style="max-width:100%;" width="400"/>
	<img src="/Raytracer/renders/23_2.png" style="max-width:100%;" width="400"/>
	
	<br>

	<img src="/Raytracer/renders/17_f.png" style="max-width:100%;" width="400"/>
	<img src="/Raytracer/renders/12_f.png" style="max-width:100%;" width="400"/>
</p>


 
## How to build
**Work In Progress** - This project requires sqlite, assimp and FreeImage


