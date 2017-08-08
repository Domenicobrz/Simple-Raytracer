#include "Display.h"
#include "Shader.h"
#include <GLFW/glfw3.h>


Display::Display(int width, int height) {
	this->width  = width;
	this->height = height;
	

	RandomData  = new float[width * height * 4];
	SampledData = new float[width * height * 4];

	createProgram();
	createDisplayTexture();
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

	double currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);



	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glUseProgram(DisplayProgramID);


	samples++;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, RandomData);
	glUniform1i(uSamples, samples);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DisplayTexture);
	glUniform1i(uDisplayTexture, 0);


	glBindVertexArray(DisplayProgramVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(NULL);
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