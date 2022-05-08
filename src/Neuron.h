#pragma once
#include "MathLib.h"


//Todo: Refactor code

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
inline float activationSigmoid(Vector weights, Vector x) {
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
	Vector weights = Vector(x.column + 1);
	Matrix xTrain = Matrix(x.row, x.column + 1);
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
				correction += 0.01f * (y.array[k] - activationSigmoid(weights, x.array[k])) * getMatrixElement(xTrain, k, j);
			}
			weights.array[j] += correction;
		}
	}
	neuron->weights = weights;
}

inline float predict(Neuron neuron, Vector x) {
	return activationSigmoid(neuron.weights, x);
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
	layer.neurons = (Neuron *)malloc(sizeof(Neuron) * size);
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
	Vector* bias;
	Matrix *z;
	Matrix *a;
}Network;

Network initNetwork(const unsigned int size) {
	Network net;
	net.size = size;
	net.layers = (Layer*)malloc(sizeof(Layer) * size);
	return net;
}


/*
	Initialize the parameters for the network based on size of the layers 
*/
static void initializeParameters(Network* network) {
	//initialize parameters from 1 to n layers
	//size of 1st parameters depends upon the data
	for (int i = 0; i < network->size; i++) {
		if(i != 0)
			network->parameters[i] = Matrix(network->layers[i].size, network->layers[i - 1].size);
		for (int j = 0; j < network->parameters[i].row; j++) {
			for (int k = 0; k < network->parameters[i].column; k++) {
				setMatrixElement(&network->parameters[i], j, k, 0.5f);
			}
		}
		network->bias[i] = Vector(network->layers[i].size);
		for (int j = 0; j < network->bias[i].size; j++) {
			network->bias[i].array[j] = (float)rand() / RAND_MAX;
		}
	}
}


/*
	set the size(num of neurons) for each layer
	Input:
		network: Network for which size of layer is to be set
		size: array of integers for size of each layers
*/
void setLayerSize(Network* network, int* size) {
	for (int i = 0; i < network->size; i++) {
		network->layers[i] = initLayer(size[i]);
	}
	network->parameters = (Matrix*)malloc(sizeof(Matrix) * network->size);
	network->bias = (Vector *)malloc(sizeof(Vector) * network->size);
	network->z = (Matrix *)malloc(sizeof(Matrix) * network->size);
	network->a = (Matrix *)malloc(sizeof(Matrix) * (network->size));
}


void trainNetwork(Network* network, Matrix x, Vector y, int iterations, float alpha) {

	/*
		Initialize tha delta parameters for the back propagation, delta = dJ / dW
	*/
	Matrix* delta = (Matrix*)malloc((network->size) * sizeof(Matrix));
			

	//divide the traing data into matrix of matrix(n * 1)
	Matrix* xTrain = (Matrix *)malloc(sizeof(Matrix) * x.row);
	for (int i = 0; i < x.row; i++) {
		//set the size of the matrix
		xTrain[i] = Matrix(x.column, 1);


		//set  the element for each element
		for (int j = 0; j < x.column; j++) {
			setMatrixElement(&xTrain[i], j, 0, getMatrixElement(x, i, j));
		}
		print(xTrain[i]);
	}

	//initilize the size of the parameters
	network->parameters[0] = Matrix(network->layers[0].size, x.column);
	network->bias[0] = Vector(network->layers[0].size);
	initializeParameters(network);
	for(int i = 0; i < iterations; i++){
		//forward propagate through a network
		for (int j = 0; j < y.size; j++) {
			float finalResult = 0.0f;


			/*
				use the parameters to calculate output of each layers and apply sigmoid function as activation function
			*/

			//for initial parameters using training data
			network->z[0] = addMatrixVector(network->parameters[0] * xTrain[j], network->bias[0]);
			network->a[0] = sigmoidMatrix(network->z[0]);


			//traverse through all the layers of the network
			for (int k = 1; k < network->size; k++) {
				//calculate the Z's of each layer Z[k] = W[k] * A[k - 1] + b[2]
				/*
					Z: linear part of the activation function
					a: non-linear part of the activation function(a = sigmoid(Z))
				*/
				network->z[k] = addMatrixVector(matrixMultiply(network->parameters[k], network->a[k - 1]), network->bias[k]);
				network->a[k] = sigmoidMatrix(network->z[k]);
			}

			finalResult = getMatrixElement(network->a[network->size - 1], 0, 0);

			//back propagate through the network to update the parameters



			//deviation from the true value
			float error = finalResult - y.array[j];
			
			for (int k = network->size - 1; k >= 0; k--) {
				if(k == network->size - 1){
					delta[k] = Matrix(1, 1);
					setMatrixElement(&delta[k], 0, 0, error);
				}
				else{
					delta[k] = elementWiseProduct(matrixMultiply(transpose(network->parameters[k + 1]), delta[k + 1]), derivativeSigmoidMatrix(network->z[k]));
				}
			}


			for (int k = network->size - 1; k >= 0; k--) {
				if (k == 0) {
					network->parameters[k] = matrixSubtract(network->parameters[k], scalematrix(matrixMultiply(delta[k], transpose(xTrain[j])), alpha));
					network->bias[k] = vectorSubtract(network->bias[k], matrixToVector(scalematrix(delta[k], alpha)));
				}
				else {
					network->parameters[k] = matrixSubtract(network->parameters[k], scalematrix(matrixMultiply(delta[k], transpose(network->a[k - 1])), alpha));
					network->bias[k] = vectorSubtract(network->bias[k], matrixToVector(scalematrix(delta[k], alpha)));
				}
			}
		}
		printf("Epoch: %d     Loss: %f\n", i + 1, abs(getMatrixElement(delta[network->size - 1], 0, 0)));
	}
}

void init_network(Network *network, Matrix x, Vector y) {
	//initilize the size of the parameters
	network->parameters[0] = Matrix(network->layers[0].size, x.column);
	network->bias[0] = Vector(network->layers[0].size);
	initializeParameters(network);
}


void trainStep(Network* network, Matrix x, Vector y, float alpha) {

	/*
		Initialize tha delta parameters for the back propagation, delta = dJ / dW
	*/
	Matrix* delta = (Matrix*)malloc((network->size) * sizeof(Matrix));
			

	//divide the traing data into matrix of matrix(n * 1)
	Matrix* xTrain = (Matrix *)malloc(sizeof(Matrix) * x.row);
	for (int i = 0; i < x.row; i++) {
		//set the size of the matrix
		xTrain[i] = Matrix(x.column, 1);


		//set  the element for each element
		for (int j = 0; j < x.column; j++) {
			setMatrixElement(&xTrain[i], j, 0, getMatrixElement(x, i, j));
		}
		// print(xTrain[i]);
	}


	//forward propagate through a network
	for (int j = 0; j < y.size; j++) {
		float finalResult = 0.0f;


		/*
			use the parameters to calculate output of each layers and apply sigmoid function as activation function
		*/

		//for initial parameters using training data
		network->z[0] = addMatrixVector(matrixMultiply(network->parameters[0], xTrain[j]), network->bias[0]);
		network->a[0] = sigmoidMatrix(network->z[0]);


		//traverse through all the layers of the network
		for (int k = 1; k < network->size; k++) {
			//calculate the Z's of each layer Z[k] = W[k] * A[k - 1] + b[2]
			/*
				Z: linear part of the activation function
				a: non-linear part of the activation function(a = sigmoid(Z))
			*/
			network->z[k] = addMatrixVector(matrixMultiply(network->parameters[k], network->a[k - 1]), network->bias[k]);
			network->a[k] = sigmoidMatrix(network->z[k]);
		}

		finalResult = getMatrixElement(network->a[network->size - 1], 0, 0);

		//back propagate through the network to update the parameters

		//deviation from the true value
		float error = finalResult - y.array[j];
		
		for (int k = network->size - 1; k >= 0; k--) {
			if(k == network->size - 1){
				delta[k] = Matrix(1, 1);
				setMatrixElement(&delta[k], 0, 0, error);
			}
			else{
				delta[k] = elementWiseProduct(matrixMultiply(transpose(network->parameters[k + 1]), delta[k + 1]), derivativeSigmoidMatrix(network->z[k]));
			}
		}


		for (int k = network->size - 1; k >= 0; k--) {
			if (k == 0) {
				network->parameters[k] = matrixSubtract(network->parameters[k], scalematrix(matrixMultiply(delta[k], transpose(xTrain[j])), alpha));
				network->bias[k] = vectorSubtract(network->bias[k], matrixToVector(scalematrix(delta[k], alpha)));
			}
			else {
				network->parameters[k] = matrixSubtract(network->parameters[k], scalematrix(matrixMultiply(delta[k], transpose(network->a[k - 1])), alpha));
				network->bias[k] = vectorSubtract(network->bias[k], matrixToVector(scalematrix(delta[k], alpha)));
			}
		}
	}

	free(delta);
	free(xTrain);
}



float predictNetwork(Network *network, Matrix *x) {
	float finalResult = 0.0f;
	// Matrix multiplyResult = matrixMultiply(network->parameters[0], *x);
	Matrix z = addMatrixVector(matrixMultiply(network->parameters[0], *x), network->bias[0]);
	z = sigmoidMatrix(z);
	for (int k = 1; k < network->size; k++) {
		z = network->parameters[k] * z;
		z = addMatrixVector(z, network->bias[k]);
		z = sigmoidMatrix(z);
 	}
	finalResult = z[0][0];
	return finalResult;
}