#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//macro for round off
#define round(x) (x > 0.5?1:0)
//macro for pi
#define PI 3.141519f

/*
* Vector for neural networks
*/
typedef struct Vector {
	float* array;
	int size;
}Vector;

Vector initVector(int size) {
	Vector vec;
	vec.size = size;
	vec.array = (float*)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++) {
		vec.array[i] = 0.0f;
	}
	return vec;
}

float dotVector(Vector vec1, Vector vec2) {
	float result = 0.0f;
	if (vec1.size != vec2.size) {
		printf("Vectors must be of same size\n");
		exit(-1);
	}
	for (int i = 0; i < vec1.size; i++) {
		result += vec1.array[i] * vec2.array[i];
	}
	return result;
}

typedef struct Matrix {
	Vector* array;
	int row, column;
}Matrix;



/*
* Matrix
*/
Matrix initMatrix(int row, int col) {
	Matrix mat;
	mat.row = row;
	mat.column = col;
	mat.array = (Vector*)malloc(sizeof(Vector) * mat.row);
	for (int i = 0; i < mat.row; i++) {
		mat.array[i] = initVector(mat.column);
	}
	return mat;
}


/*
* returns the element of matrirx for ith and jth element
*/
inline float getMatrixElement(Matrix mat, unsigned int i, unsigned int j) {
	Vector vec = mat.array[i];
	return vec.array[j];
}

/*
* set the element of matrirx for ith and jth element
*/
inline void setMatrixElement(Matrix* mat, int i, int j, float element) {
	mat->array[i].array[j] = element;
}

void setMatrixArray(Matrix* mat, float* array) {
	for (int i = 0; i < mat->row; i++) {
		for (int j = 0; j < mat->column; j++) {
			setMatrixElement(mat, i, j, array[mat->column * i + j]);
		}
	}
}

/*
* returns the transpose of the matrix
*/
Matrix transpose(Matrix mat) {
	Matrix result = initMatrix(mat.column, mat.row);
	for (int i = 0; i < result.row; i++) {
		for (int j = 0; j < result.column; j++) {
			Vector* vec = &mat.array[i];
			vec->array[j] = getMatrixElement(mat, j, i);
		}
	}
	return result;
}


/*
	Calculates the product of two matrices
*/
Matrix matrixMultiply(Matrix mat1, Matrix mat2) {
	Matrix result = initMatrix(mat1.row, mat2.column);
	if (mat1.column != mat2.row) {
		printf("Dimension error\n");
		exit(-1);
	}
	for(int i = 0; i < mat1.row; i++) {
		for (int j = 0; j < mat2.column; j++) {
			for (int k = 0; k < mat1.column; k++) {
				setMatrixElement(&result, i,j, getMatrixElement(result, i, j) + getMatrixElement(mat1, i, k) * getMatrixElement(mat2, k, j));
			}
		}
	}
	return result;
}


/*
	Calculates the product of matrix and vector
*/
Vector multiplyMatrixVector(Matrix mat, Vector vec) {
	Vector result = initVector(vec.size);
	if (mat.column != vec.size) {
		printf("Please check the dimension\n");
		exit(-1);
	}
	for (int i = 0; i < mat.row; i++) {
		for (int j = 0; j < mat.column; j++) {
			result.array[i] += getMatrixElement(mat, i, j) * vec.array[j];
		}
	}
	return result;
}

/*
	Add vector and matrix using broadcasting
*/
Matrix addMatrixVector(Matrix mat, Vector vec){
	Matrix result = initMatrix(mat.row, mat.column);
	if (mat.row != vec.size) {
		printf("Please check the dimension\n");
		exit(-1);
	}
	for (int i = 0; i < mat.row; i++) {
		for (int j = 0; j < mat.column; j++) {
			float sum = getMatrixElement(mat, i, j) * vec.array[i];
			setMatrixElement(&result, i, j, sum);
		}
	}
	return result;
}

Matrix vectorToMatrix(Vector vec){
	Matrix result = initMatrix(vec.size, 1);
	for(int i = 0; i < vec.size; i++){
		setMatrixElement(&result, i, 0, vec.array[i]);
	}
	return result;
}


/*-----------------------------math function----------------------------*/

/*
	Sigmoid functions
*/
float sigmoid(float x){
	return (float)(1 / (exp(-x) + 1));
}


Vector sigmoidVector(Vector vec) {
	Vector result = initVector(vec.size);
	for(int i = 0; i < vec.size; i++){
		result.array[i] = sigmoid(vec.array[i]);
	}
	return result;
}



Matrix sigmoidMatrix(Matrix mat) {
	Matrix result = initMatrix(mat.row, mat.column);
	for(int i = 0; i < mat.row; i++){
		for(int j = 0; j < mat.column; j++){
			setMatrixElement(&result, i, j, sigmoid(getMatrixElement(mat, i, j)));
		}
	}
	return result;
}


// derivative of a sigmoid
float derivativeSigmoid(float x) {
	return sigmoid(x) * (1 - sigmoid(x));
}


Vector derivativeSigmoidVector(Vector vec) {
	Vector result = initVector(vec.size);
	for (int i = 0; i < vec.size; i++) {
		result.array[i] = derivativeSigmoid(vec.array[i]);
	}
	return result;
}


Matrix derivativeSigmoidMatrix(Matrix mat) {
	Matrix result = initMatrix(mat.row, mat.column);
	for (int i = 0; i < mat.row; i++) {
		for (int j = 0; j < mat.column; j++) {
			setMatrixElement(&result, i, j, derivativeSigmoid(getMatrixElement(mat, i, j)));
		}
	}
	return result;
}