#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Display.h"
#include "Database.h"


const int WIDTH  = 800;
const int HEIGHT = 600;

Display* displayProgram;


int main() {

	displayProgram = new Display(WIDTH, HEIGHT);
	displayProgram->runLoop();
	
	return 0;
}