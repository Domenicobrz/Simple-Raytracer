#include "Display.h"
#include "Shader.h"
#include <thread>
#include <GLFW/glfw3.h>
#include <mutex>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <stdlib.h>
#include <iostream>
#include <string>

#include "Sphere.h"
#include "Triangle.h"

#include "Material.h"
#include "Lambert.h"
#include "Glossy.h"
#include "Glass.h"
#include "Light.h"

#include "naiveBVH.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "sqlite3.h"

using namespace glm;
extern Display* displayProgram;


Display::Display(int _width, int _height) {
	this->width = _width;
	this->height = _height;
	


	Render = Database();
	if (Database::database_exist(Render.databasePath)) {
		loadRender();
	} else {
		RandomData = new float[_width * _height * 4];
		SampledData = new float[_width * _height * 4];
		memset(RandomData, 0, _width * _height * 4 * sizeof(float));
	}



	buildScene();

	/* needs to be before any openGL call since we create the context here */
	createWindow();
	createProgram();
	createDisplayTexture();



	t1 = std::thread([=] { srand(1352523); printf("%f - ", rnd()); runRenderThread(); });
	t2 = std::thread([=] { srand(234);     printf("%f - ", rnd()); runRenderThread(); });
	t3 = std::thread([=] { srand(3134534); printf("%f \n", rnd()); runRenderThread(); });
	//t4 = std::thread([=] { srand(892308);  printf("%f", rnd()); runRenderThread(); });
}

void Display::createProgram() {
	DisplayProgramID = LoadShaders("displayv.glsl", "displayf.glsl");
	
	uDisplayTexture = glGetUniformLocation(DisplayProgramID, "uDisplayTexture");
	uSamples		= glGetUniformLocation(DisplayProgramID, "uSamples");




	const GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		+1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, +1.0f, 0.0f, 0.0f,

		-1.0f, +1.0f, 0.0f, 0.0f,
		+1.0f, -1.0f, 0.0f, 0.0f,
		+1.0f, +1.0f, 0.0f, 0.0f,
	};
	
	glGenBuffers(1, &DisplayProgramVBO);
	glBindBuffer(GL_ARRAY_BUFFER, DisplayProgramVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	


	glGenVertexArrays(1, &DisplayProgramVAO);
	glBindVertexArray(DisplayProgramVAO);
	
		glBindBuffer(GL_ARRAY_BUFFER, DisplayProgramVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(NULL);
}

bool Display::createWindow() {
	/* initialize window */
	if (!glfwInit()) {
		fprintf(stderr, "failed to initialize GLFW");
		return false;
	}
	else {
		fprintf(stderr, "GLFW initialized");
	}

	glfwWindowHint(GLFW_SAMPLES, 1); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 




	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Tutorial 01", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window); // Initialize GLEW

	/* initialize openGL */
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}



	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Set callback for keyboard events
	glfwSetKeyCallback(window, &Display::key_callback);
}

void Display::runLoop() {
	do {
		displayProgram->update();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void Display::update() {

	//if (!updateRequested) return;

	double currentTime = glfwGetTime();
	if (float(currentTime - lastTime) < 0.5) return;

	deltaTime = float(currentTime - lastTime);



	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glUseProgram(DisplayProgramID);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, RandomData);
	glUniform1i(uSamples, samples);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DisplayTexture);
	glUniform1i(uDisplayTexture, 0);



	glBindVertexArray(DisplayProgramVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(NULL);



	updateRequested = false;
}

void Display::createDisplayTexture() {

	glGenTextures(1, &DisplayTexture);
	glBindTexture(GL_TEXTURE_2D, DisplayTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, RandomData.data());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, RandomData);
}

void Display::runRenderThread() {

	float* buffer = new float[width * height * 4];

	for (;;) {
		memset(buffer, 0, width * height * 4 * sizeof(float));

		for (int i = 0; i < width * height; i++) {
			vec3 color = scene.compute2(i);

			buffer[i * 4 + 0] = color.r;
			buffer[i * 4 + 1] = color.g;
			buffer[i * 4 + 2] = color.b;
			buffer[i * 4 + 3] = 1;
		}

		/* attach mutex */
		updateMutex.lock();
		for (int i = 0; i < width * height * 4; i++) {
			RandomData[i] += buffer[i];
		}

		samples++;
		updateRequested = true;
		printf("%d \n", samples);

		//for (;;) {
		//	if (!updateRequested) break;
		//}
		updateMutex.unlock();

		/* needs to be out of the mutex otherwise deadlock */
		if (blockRenderThreads) break;
	}
}

void Display::buildScene() {

	vec3 eye = vec3(0.0f, 35.0f, -20.0f);
	vec3 lookAt = vec3(-10.0f, 10.0f, 50.0f);
	Camera camera(width, height, eye, lookAt);

	camera.aperture = 3.6f;
	camera.focusDistance = length(eye - lookAt) - 15.5f; //80.0f;

	scene.camera = camera;

	Sphere* sphere1  = new Sphere(vec3(0, 0, 50), 10.0f);
	sphere1->material = new GlassMaterial(vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.3f);
	//sphere1->material = new LambertMaterial(vec3(1.0f, 1.0f, 1.0f));
	//sphere1->material = new LightMaterial(vec3(40.0f, 5.0f, 5.0f));
	Primitive* prim1 = sphere1;

	Sphere* sphere2 = new Sphere(vec3(0, -105, 50), 100);
	sphere2->material = new LambertMaterial(vec3(0.3, 0.3, 0.3));
	Primitive* prim2 = sphere2;

	//scene.addPrimitive(prim1);
	scene.addPrimitive(prim2);

	for (int i = 0; i < 15; i++) {
		//Sphere* sphere = new Sphere(vec3(rnd() * 1000.0f - 500.0f, rnd() * 500.0f + 500.0f, -1000.0f + rnd() * 500.0f), 150);
		Sphere* sphere = new Sphere(vec3(rnd() * 100.0f - 50.0f, rnd() * 15.0f + 5.0f, -40.0f + rnd() * 200.0f), 5);
		sphere->material = new LambertMaterial(vec3(0.3f + rnd(), 0.3f + rnd(), 0.3f + rnd()));
		//sphere->material = new LightMaterial(vec3(rnd() * 9.0f, rnd() * 4.0f, rnd() * 4.0f));
		scene.addPrimitive(sphere);
	}


	//Material* modelMaterial = new GlossyMaterial(vec3(1, 0.8f, 0.8f), 0.1f);// , 1.3f);
	Material* modelMaterial = new LambertMaterial(vec3(0.6, 0.7, 0.8));// , 1.3f);
	mat4 modelMatrix = mat4();
	modelMatrix = glm::translate(modelMatrix, vec3(-20, 0, 50));
	modelMatrix = glm::scale(modelMatrix, vec3(3, 3, 3));
	modelMatrix = glm::rotate(modelMatrix, 0.7f, vec3(0, 1, 0));

	scene.loadModel("C:\\Users\\Domenico\\desktop\\dragon2.obj", modelMatrix, modelMaterial);


	/* creating a plane */
	Material* planeMaterial = new LambertMaterial(vec3(1, 1, 1));
	Triangle* pl1 = new Triangle(
		vec3(-300, 0,-300),
		vec3(300,  0,-300),
		vec3(-300, 0,300)
	);
	pl1->material = planeMaterial;
	scene.addPrimitive(pl1);

	Triangle* pl2 = new Triangle(
		vec3(-300, 0, 300),
		vec3(300,  0, 300),
		vec3(300,  0, -300)
		);
	pl2->material = planeMaterial;
	scene.addPrimitive(pl2);


	scene.bvh.createBVH(&scene.primitives);
} 




void Display::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		displayProgram->saveRender();
		glfwSetWindowShouldClose(window, 1);
	}
}

void Display::saveRender() {
	blockRenderThreads = true;

	t1.join();
	t2.join();
	t3.join();
	// t4.join();

	/* save result maybe ? */
	Render.saveRender(width, height, samples, RandomData, width * height * 4 * sizeof(float));
}

void Display::loadRender() {
	// TODO: it makes more sense if Database returns a struct of render options, which will set the members of display instead of
	// handling display members inside Database since it complicates shit a lot more than it's needed
	RenderOptions options = Render.loadRenderOptions();
	width = options.width;
	height = options.height;
	samples = options.samples;

	RandomData = new float[width * height * 4];
	SampledData = new float[width * height * 4];

	Render.loadRenderBlob(RandomData);
}