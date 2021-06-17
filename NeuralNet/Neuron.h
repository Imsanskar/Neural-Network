#pragma once
#include "MathLib.h"


//A single neuron
typedef struct Neuron {
	Vector weights;
}Neuron;


static float compute(Vector weight, Vector x) {
	float result = weight.array[0];
	for (int i = 1; i < weight.size; i++) {
		result += weight.array[i] * x.array[i - 1];
	}
	return result;
}

/*
	Sigmoid function for optimization
	Inputs:
		weights->weight of the neural net to be optimized
		x->input value
	Returns:
		Result of sigmoid function
*/
float sigmoid(Vector weights, Vector x) {
	return (float)(1 / (exp(-compute(weights, x)) + 1));
}

/*
	Trains a neuron using logistic regression
	Inputs: 
		neuron:pointer to a neuron to be trained
		x	  : Training x labels
		y     : Training y labels
*/
void train(Neuron* neuron, Matrix x, Vector y) {
	Vector weights = initVector(x.column + 1);
	Matrix xTrain = initMatrix(x.row, x.column + 1);
	for (int i = 0; i < xTrain.row; i++) {
		for (int j = 0; j < xTrain.column; j++) {
			if (j == 0)
				setMatrixElement(&xTrain, i, j, 1.0f);
			else
				setMatrixElement(&xTrain, i, j, getMatrixElement(x, i, j - 1));
		}
	}

	//iteration loop
	for (int i = 0; i < 10000; i++) {
		float correction = 0.0f;
		//for all the weights
		for (int j = 0; j < weights.size; j++) {
			//for all the data set
			for (int k = 0; k < y.size; k++) {
				correction += 0.01f * (y.array[k] - sigmoid(weights, x.array[k])) * getMatrixElement(xTrain, k, j);
			}
			weights.array[j] += correction;
		}
	}
	neuron->weights = weights;
}

float predict(Neuron neuron, Vector x) {
	return sigmoid(neuron.weights, x);
}

/*
* Code for a single layer of neural net
*/
typedef struct Layer {
	Neuron* neurons;
	int size;
}Layer;

/*
	Initialize a layer for neural net
	Inputs:
		size: number of neurons in a layer
*/
Layer initLayer(const unsigned int size) {
	Layer layer;
	layer.size = size;
	layer.neurons = (Neuron*)malloc(sizeof(Neuron) * size);
	return layer;
}

Neuron* getNeuron(Layer layer, int index) {
	return &layer.neurons[index];
}

void setNeurons(Layer* layer, Neuron* neuron) {
	for (int i = 0; i < layer->size; i++) {
		layer->neurons[i] = neuron[i];
	}
}


/*
* Finally to neural network
*/
typedef struct Network {
	Layer* layers;
	int size;
	Matrix* parameters;
}Network;

Network initNetwork(const unsigned int size) {
	Network net;
	net.size = size;
	net.layers = (Layer*)malloc(sizeof(Layer) * size);
	return net;
}

void initializeParameters(Network* network) {
	//initialize parameters from 1 to n layers
	for (int i = 1; i < network->size; i++) {
		network->parameters[i] = initMatrix(network->layers[i].size, network->layers[i - 1].size);
	}
}

void setLayerSize(Network* network, int* size) {
	for (int i = 0; i < network->size; i++) {
		network->layers[i] = initLayer(size[i]);
	}
	network->parameters = (Matrix*)malloc(sizeof(Matrix) * network->size);
	initializeParameters(network);
}


//Todo:Implement back propagation
void trainNetwork(Network* network, Vector x, Vector y) {

}
