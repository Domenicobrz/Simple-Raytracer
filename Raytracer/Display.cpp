#include "Display.h"
#include "Shader.h"
#include <thread>
#include <GLFW/glfw3.h>
#include <mutex>
#include <glm/glm.hpp>

#include "Sphere.h"
#include "Material.h"
#include "Lambert.h"
#include "Glossy.h"
#include "Glass.h"
#include "Light.h"

#include <stdlib.h>

using namespace glm;

Display::Display(int width, int height) {
	this->width  = width;
	this->height = height;
	

	RandomData  = new float[width * height * 4];
	SampledData = new float[width * height * 4];
	memset(RandomData, 0, width * height * 4 * sizeof(float));

	buildScene();

	createProgram();
	createDisplayTexture();

	t1 = std::thread([=] { srand(1352523); printf("%f", rnd()); runRenderThread(); });
	t2 = std::thread([=] { srand(234);     printf("%f", rnd()); runRenderThread(); });
	t3 = std::thread([=] { srand(3134534); printf("%f", rnd()); runRenderThread(); });
	t4 = std::thread([=] { srand(892308);  printf("%f", rnd()); runRenderThread(); });
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

	}
}


void Display::buildScene() {

	vec3 eye = vec3(0.0f, 10.0f, -15.0f);
	vec3 lookAt = vec3(0.0f, 0.0f, 50.0f);
	Camera camera(width, height, eye, lookAt);

	scene.camera = camera;

	Sphere* sphere1  = new Sphere(vec3(0, 0, 50), 10.0f);
	//sphere1->material = new GlassMaterial(vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.25f);
	sphere1->material = new GlossyMaterial(vec3(1.0f, 1.0f, 1.0f), 1.0f);
	Primitive* prim1 = sphere1;

	Sphere* sphere2 = new Sphere(vec3(0, -105, 50), 100);
	sphere2->material = new LambertMaterial(vec3(0.3, 0.3, 0.3));
	Primitive* prim2 = sphere2;

	scene.addPrimitive(prim1);
	scene.addPrimitive(prim2);

	for (int i = 0; i < 10; i++) {
		Sphere* sphere = new Sphere(vec3(rnd() * 30.0f - 15.0f, rnd() * 12.0f, 45.0f - rnd() * 10.0f), rnd() * 5.0f);
		//if (i % 2 == 0)
			sphere->material = new LambertMaterial(vec3(0.3f + rnd(), 0.3f + rnd(), 0.3f + rnd()));
		//else
			//sphere->material = new GlassMaterial(vec3(rnd() * 0.2f + 0.8f, rnd() * 0.2f + 0.8f, rnd() * 0.2f + 0.8f), rnd() * 0.25f, 1.2f);

			//if (i == 0)
			//	sphere->material = new LightMaterial(vec3(2.0f, 0.3f, 0.4f));

			//if (i == 9)
			//	sphere->material = new LightMaterial(vec3(0.2f, 0.25f, 2.1f));

			if (i == 5) {
				sphere = new Sphere(vec3(1000.0f, 1000.f, -1000.0f), 500.0f);
				sphere->material = new LightMaterial(vec3(rnd() * 23.0f, rnd() * 23.0f, rnd() * 23.0f));
			}

		scene.addPrimitive(sphere);
	}
} 