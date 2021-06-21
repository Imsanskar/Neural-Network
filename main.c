#include <stdlib.h>
#include <math.h>
#include<stdio.h>
#include "MathLib.h"
#include "Neuron.h"

int main() {
	Vector vec = initVector(3);
	float weights[3] = {0.01f, 0.01f, 0.01f};
	vec.array = &weights[0];
	float xTrain[8] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};
	Matrix xMat = initMatrix(4, 2); 
	setMatrixArray(&xMat, xTrain);
	float y[4] = {0, 1, 1, 0};
	Vector yVec = initVector(4);
	yVec.array = y;
	/*
	Neuron neuron;
	train(&neuron, xMat, yVec);

	float z;
	for(int i = 0; i < 4; i++){
		z = sigmoid(neuron.weights, xMat.array[i]);
		printf("(x1:%f, x2:%f) (y: %d) %f\n", getMatrixElement(xMat,i,0), getMatrixElement(xMat, i, 1), round(z), z);
	}*/

	Network network = initNetwork(2);
	int arch[] = { 2, 1 };
	setLayerSize(&network, arch);
	trainNetwork(&network, xMat, yVec, 10000, 0.1);

	vec = initVector(2);
	vec.array[0] = 0.0f;
	vec.array[1] = 0.0f;
	float a = predictNetwork(&network, vectorToMatrix(vec));
	printf("%f\n", a);
	vec.array[0] = 0.0f;
	vec.array[1] = 1.0f;
	a = predictNetwork(&network, vectorToMatrix(vec));
	printf("%f\n", a);
	vec.array[0] = 1.0f;
	vec.array[1] = 0.0f;
	a = predictNetwork(&network, vectorToMatrix(vec));
	printf("%f\n", a);
	vec.array[0] = 1.0f;
	vec.array[1] = 1.0f;
	a = predictNetwork(&network, vectorToMatrix(vec));
	printf("%f\n", a);
}
