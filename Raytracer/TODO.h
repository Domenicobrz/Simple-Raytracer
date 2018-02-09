// Implement spheres intersections with nanoRT
	// Implement sphere uvAtPoint();
	// Implement getInterpolatedUVs for spheres

// Implement multi texture support
	// Implement blendable textures

// FreeImage needs an error Handler - see Texture2D.cpp's constructor
// FreeImage default's color order is BGR - TextureSkybox is affected by this - REBUILD FREEIMAGE to accept only RGB as the
//     color order: https://stackoverflow.com/questions/31103121/why-freeimage-loads-image-in-bgr-format


// FIX:
// in nanort convention, the second vertex specified represents u, the third represents v - 
//		we're specifying vertices in planeGeometry the wrong way
// If the scene has no primitives stored in nanort's arrays, Accel.Traverse() will crash

// Implement #define DEBUG release unit testing for texture2D.cpp


// BUGS:
// since the seed for Random is always the same at start, when we save a scene and start rendering it again later, it uses the same 
// random numbers used in the previous scene, so the result we get in the end is identical to the first one and the integration is incorrect
// in the PHONG BRDF floating points error may occur in the code which converts the spherical coordinate system to world coordinates,
//     since the cross product may result close to infinity



// Colore del materiale : kd * col
// perchè kd ? perchè ci permette di abbassare la riflettanza dell'albedo senza dover cambiare il colore
// 
// es.buona fortuna ad abbassare di 0.8 la riflettanza di : (234, 177, 14)



// change the  "return mask;"   in BRDFs with  "return absorbtion * angleDispersion;