#ifndef DISPLAY_h
#define DISPLAY_h

#define GLEW_STATIC
#include <GL/glew.h>
#include <thread>
#include <mutex>
#include "Scene.h"
#include "Database.h"
#include <GLFW/glfw3.h>


class Display {

public:
	Display(int, int);
	void update();
	void saveRender();
	void loadRender();
	void runLoop();


	int width;
	int height;
	int samples = 0;
	float RandomData[1000 * 800 * 4];
	Database Render;



private:

	bool createWindow();
	void createProgram();
	void createDisplayTexture();
	
	static void key_callback(GLFWwindow*, int, int, int, int);

	void buildScene();
	
	void runRenderThread();
	std::thread t1;
	std::thread t2;
	std::thread t3;
	std::thread t4;


	GLFWwindow* window;


	GLuint DisplayProgramID;
	GLuint DisplayProgramVAO;
	GLuint DisplayProgramVBO;

	GLuint uDisplayTexture;
	GLuint uSamples;

	
	GLuint DisplayTexture;
	float* SampledData;

	bool updateRequested = false;
	bool blockRenderThreads = false;
	std::mutex updateMutex;


	Scene scene;


	float lastTime;
	float deltaTime;
};

#endif