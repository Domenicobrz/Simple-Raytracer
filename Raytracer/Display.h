#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class Display {

public:
	Display(int, int);
	void update();

private:
	void createProgram();
	void createDisplayTexture();
	
	GLuint DisplayProgramID;
	GLuint DisplayProgramVAO;
	GLuint DisplayProgramVBO;

	GLuint uDisplayTexture;
	GLuint uSamples;

	
	GLuint DisplayTexture;
	float* RandomData;
	float* SampledData;

	int samples = 0;

	int width;
	int height;

	float lastTime;
	float deltaTime;
};