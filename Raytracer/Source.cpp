#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Display.h"
#include "Database.h"



#define FREEIMAGE_COLORORDER_BGR 0 
#define FREEIMAGE_COLORORDER_RGB 1
#include <FreeImage.h>

const int WIDTH  = 600;
const int HEIGHT = 450;

Display* displayProgram;

std::string AssetsPath = "D:\\Projects\\Raytracer\\Assets\\";


int main() {

	displayProgram = new Display(WIDTH, HEIGHT);
	displayProgram->runLoop();
	
	return 0;
}