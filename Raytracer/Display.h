#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <thread>
#include <mutex>
#include "Scene.h"

class Display {

public:
	Display(int, int);
	void update();

private:
	void createProgram();
	void createDisplayTexture();
	
	void runRenderThread();
	std::thread t1;
	std::thread t2;
	std::thread t3;

	GLuint DisplayProgramID;
	GLuint DisplayProgramVAO;
	GLuint DisplayProgramVBO;

	GLuint uDisplayTexture;
	GLuint uSamples;

	
	GLuint DisplayTexture;
	float* RandomData;
	float* SampledData;

	int samples = 0;
	bool updateRequested = false;
	std::mutex updateMutex;

	int width;
	int height;


	Scene scene;


	float lastTime;
	float deltaTime;
};