// Implement spheres intersections with nanoRT
	// Implement sphere uvAtPoint();
	// Implement getInterpolatedUVs for spheres

// Implement multi texture support
	// Implement blendable textures

// FreeImage needs an error Handler - see Texture2D.cpp's constructor

// FIX:
// in nanort convention, the second vertex specified represents u, the third represents v - 
// we're specifying vertices in planeGeometry the wrong way


// Implement #define DEBUG release unit testing for texture2D.cpp


// BUGS:
// since the seed for Random is always the same at start, when we save a scene and start rendering it again later, it uses the same 
// random numbers used in the previous scene, so the result we get in the end is identical to the first one and the integration is incorrect