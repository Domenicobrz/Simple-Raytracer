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

#include "TextureManager.h"
#include "Texture2D.h"

#include "CubeGeometry.h"
#include "PlaneGeometry.h"
#include "CornellBoxGeometry.h"
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
			vec3 color = scene.compute3(i);

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

	vec3 eye = vec3(0.0f, 100.0f, -280.0f);
	vec3 lookAt = vec3(0.0f, 100.0f, 0.0f);
	Camera camera(width, height, eye, lookAt);

	camera.aperture = 0.f;
	camera.focusDistance = length(eye - vec3(0,0,-35));

	scene.camera = camera;



	Sphere* sphere = new Sphere(vec3(20,40, 100), 20);
	sphere->material = new LightMaterial(vec3(2,2,2));
	//scene.addPrimitive(sphere);

	{
		Sphere* sphere = new Sphere(vec3(20, 10, 0), 5);
		sphere->material = new LightMaterial(vec3(1.5, 1.5, 1.5));
		//scene.addPrimitive(sphere);
	}
	
	
	for (int i = 0; i < 3; i++) {
		Sphere* sphere = new Sphere(vec3(rnd() * 100.0f - 50.0f, rnd() * 15.0f + 5.0f, rnd() * 100.0f), 5);
		sphere->material = new LambertMaterial(vec3(0.3f + rnd(), 0.3f + rnd(), 0.3f + rnd()));

	//	if (i % 7 == 0) {
	//		sphere->material = new LightMaterial(vec3(0.3f + rnd(), 0.3f + rnd(), 0.3f + rnd()));
	//	}

		//scene.addPrimitive(sphere);
	}


	Material* modelMaterial = new LambertMaterial(vec3(1, 1, 1));// , 1.3f);
	//Material* modelMaterial = new GlassMaterial(vec3(1.0, 1.0, 1.0), 0.05f, 1.4f);// , 1.3f);
	mat4 modelMatrix = mat4();
	modelMatrix = glm::translate(modelMatrix, vec3(-6, 0, 20));
	modelMatrix = glm::scale(modelMatrix, vec3(19, 19, 19));
	modelMatrix = glm::rotate(modelMatrix, 3.1415f, vec3(0, 1, 0));
	scene.loadModel("C:\\Users\\Domenico\\desktop\\archangel.obj", modelMatrix, modelMaterial);

	//{ Material* modelMaterial = new LambertMaterial(vec3(0.2, 0.2, 0.2));// , 1.3f);
	//mat4 modelMatrix = mat4();
	//modelMatrix = glm::translate(modelMatrix, vec3(0, 0, 0));
	//modelMatrix = glm::scale(modelMatrix, vec3(100, 100, 100));
	//modelMatrix = glm::translate(modelMatrix, vec3(0, 1, 0));
	//scene.loadModel("C:\\Users\\Domenico\\desktop\\cub3.obj", modelMatrix, modelMaterial);
	//}

	/* creating a plane */
	float m = 2.0f;
	mat4 plane_transform = mat4();
	plane_transform = glm::translate(plane_transform, vec3(-99, 60, 0));
	plane_transform = glm::scale(plane_transform, vec3(65));
	plane_transform = glm::rotate(plane_transform, -(float)AI_MATH_PI / 2.0f, vec3(1, 0, 0));
	plane_transform = glm::rotate(plane_transform, (float)AI_MATH_PI / 2.0f, vec3(0, 0, 1));
	Geometry* plane = new PlaneGeometry(plane_transform);
	LightMaterial* planeMat = new LightMaterial(vec3(8 * m, 1 * m, 8 * m));
	planeMat->tm = new Texture2D("C:\\Users\\Domenico\\Desktop\\Crystal castle - plague.png", 1.6f);
	plane->setMaterial(planeMat);
	scene.add(plane);



	Geometry* cbox = new CornellBoxGeometry(vec3(0, 99, 20), vec3(100, 100, 100));
	cbox->setMaterial(new LambertMaterial(vec3(0.2, 0.2, 0.2)));
	scene.add(cbox);



	for (int i = 0; i < scene.primitives.size(); i++) {
		vec3 v0 = scene.primitives[i]->getV0();
		vec3 v1 = scene.primitives[i]->getV1();
		vec3 v2 = scene.primitives[i]->getV2();
		scene.vertsv.push_back(v0.x);
		scene.vertsv.push_back(v0.y);
		scene.vertsv.push_back(v0.z);

		scene.vertsv.push_back(v1.x);
		scene.vertsv.push_back(v1.y);
		scene.vertsv.push_back(v1.z);

		scene.vertsv.push_back(v2.x);
		scene.vertsv.push_back(v2.y);
		scene.vertsv.push_back(v2.z);

		scene.facesv.push_back(i * 3 + 0);
		scene.facesv.push_back(i * 3 + 1);
		scene.facesv.push_back(i * 3 + 2);
	}

	scene.triangle_mesh = new nanort::TriangleMesh<float>(scene.vertsv.data(), scene.facesv.data(), sizeof(float) * 3);
	scene.triangle_pred = new nanort::TriangleSAHPred<float>(scene.vertsv.data(), scene.facesv.data(), /* stride */sizeof(float) * 3);
	//scene.triangle_intersector = new nanort::TriangleIntersector<>(scene.vertsv.data(), scene.facesv.data(), sizeof(float) * 3);
	bool ret = scene.accel.Build(scene.facesv.size() / 3, *scene.triangle_mesh, *scene.triangle_pred);

	nanort::BVHBuildStatistics stats = scene.accel.GetStatistics();

	printf("  BVH statistics:\n");
	printf("    # of leaf   nodes: %d\n", stats.num_leaf_nodes);
	printf("    # of branch nodes: %d\n", stats.num_branch_nodes);
	printf("  Max tree depth   : %d\n",   stats.max_tree_depth);
	
	//scene.bvh.createBVH(&scene.primitives);
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
	//t4.join();

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