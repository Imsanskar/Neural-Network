#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include<stdio.h>
#include "MathLib.h"
#include "Neuron.h"

typedef struct Screen {
	int width, height;
}Screen;


void render_plot(Network *network, int width, int height) {
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, 1);

	glBegin(GL_POINTS);

	Vector inp = Vector(2);
	Matrix mat = vectorToMatrix(inp);
	for(int i = 0; i < width; i++) {
		double x1 = (double) i / (double) width;
		inp.array[0] = x1;
		for(int j  = 0; j < height; j++){
			double x2 = (double) j / (double) height;
			inp.array[1] = x2;
			double pred;
			pred = predictNetwork(network, &mat);
			
			if (pred > 0.6) {
				glColor3f(0.0f, 1.0f, 0.0f);
			}
			else {
				glColor3f(0.0f, 0.0f, 1.0f);
			}

			glVertex2f(i, j);
		}
	}

	glEnd();
}


int main() {
	Screen screen;

	glfwInit();

	const int width = 800, height = 800;
	screen.width = width;
	screen.height = height;
	float aspectRatio = float(width) / float(height);
	GLFWwindow* window = glfwCreateWindow(width, height, "Neural Network", NULL, NULL);
	glfwMakeContextCurrent(window);

	// glfwSetWindowUserPointer(window, &screen);


	Vector vec(3);
	float weights[3] = {0.01f, 0.01f, 0.01f};
	// vec.array = &weights[0];
	float temp = vec[0];
	float xTrain[8] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};
	Matrix xMat(4, 2);
	setMatrixArray(&xMat, xTrain);
	float y[4] = {0, 1, 1, 0};
	Vector yVec = Vector(4);
	yVec[0] = y[0];
	yVec[1] = y[1];
	yVec[2] = y[2];
	yVec[3] = y[3];
	// yVec.array = y;

	// // neural network code
	Network network = initNetwork(2);
	int arch[] = { 2, 1 };
	setLayerSize(&network, arch);

	init_network(&network, xMat, yVec);

	trainNetwork(&network, xMat, yVec, 100000, 0.01);

	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render_plot(&network, width, height);
		trainStep(&network, xMat, yVec, 0.0001);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}
