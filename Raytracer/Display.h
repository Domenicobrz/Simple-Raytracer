#ifndef DISPLAY_h
#define DISPLAY_h

#define GLEW_STATIC
#include <GL/glew.h>
#include <thread>
#include <mutex>
#include "Scene.h"
#include "Database.h"

class Display {

public:
	Display(int, int);
	void update();
	void saveResult();

private:
	void createProgram();
	void createDisplayTexture();
	
	void buildScene();
	
	void runRenderThread();
	std::thread t1;
	std::thread t2;
	std::thread t3;
	std::thread t4;

	Database Render;

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
	bool blockRenderThreads = false;
	std::mutex updateMutex;

	int width;
	int height;


	Scene scene;


	float lastTime;
	float deltaTime;
};

#endif