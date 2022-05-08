#pragma once

#include <cstddef>
#include <cstdlib>
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
struct Vector {
	float* array = {};
	int size;

	Vector() {
		size = 0;	
	}

	Vector(int _size) {
		size = _size;
		array = (float *)malloc(sizeof(float) * size);
		if(array == NULL) {
			exit(-1);
		}
		for (int i = 0; i < size; i++) {
			array[i] = 0.0f;
		}
	}

	// copy constructors
	Vector(const Vector& v) {
		size = v.size;
		array = (float *)malloc(sizeof(float) * size);
		for (int i = 0; i < size; i++) {
			array[i] = v.array[i];
		}
	}

	// move constructors, no except just to please visual studio
	Vector(Vector&& v) noexcept {
		size = v.size;
		array = v.array;
		v.array = NULL;
	}

	Vector& operator =(const Vector& v) {
		if(this != &v) {
			free(array);
			size = v.size;
			array = (float *)malloc(sizeof(float) * size);

			for(int i = 0; i < size; i++) {
				array[i] = v[i];
			}
		}
		return *this;
	} 

	Vector& operator =(Vector&& v) {
		if (this != &v) {
			free(array);
			size = v.size;
			array = v.array;
			v.array = NULL;
		}
		return *this;
	} 

	float& operator [](int index) const{
		return array[index];
	}

	~Vector (){
		free(array);
	} 
};

/*
	Inititialize a vector of size
*/
// Vector Vector(int size) {
// 	Vector vec;
// 	vec.size = size;
// 	vec.array = (float *)malloc(sizeof(float) * size);
// 	if(vec.array == NULL) {
// 		exit(-1);
// 	}
// 	for (int i = 0; i < size; i++) {
// 		vec.array[i] = 0.0f;
// 	}
// 	return vec;
// }

void freeVector(Vector *vec) {
	free(vec->array);
	free(vec);
}

/* 
	returns a dot vector of two vectors vec1 and vec2
*/
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

Vector vectorSubtract(Vector vec1, Vector vec2) {
	Vector result = Vector(vec1.size);
	if (vec1.size != vec2.size) {
		printf("Vectors must be of same size\n");
		exit(-1);
	}
	for (int i = 0; i < vec1.size; i++) {
		result.array[i] = vec1.array[i] - vec2.array[i];
	}
	return result;
}



/*------------------------------------Matrix---------------------------*/
struct Matrix {
	Vector* array = {};
	int row, column;

	Matrix() {

	}

	Matrix(int _row, int col) {
		row = _row;
		column = col;
		array = (Vector *)malloc(sizeof(Vector) * row);
		for (int i = 0; i < row; i++) {
			array[i] = Vector(column);
		}
	}

	// copy assignemnt
	Matrix& operator = (const Matrix &mat) noexcept{
		if(this != &mat) {
			free(array);
			row = mat.row;
			column = mat.column;
			free(array);
			array = (Vector *)malloc(sizeof(Vector) * row);
			for (int i = 0; i < row; i++) {
				array[i] = Vector(column);
			}

			for(int i = 0; i < row; i++) {
				for(int j = 0; j < column; j++) {
					array[i].array[j] = mat.array[i].array[j];
				}
			}
		}

		return *this;
	}

	// move assignment 
	Matrix& operator = (Matrix &&mat) noexcept{
		if(this != &mat) {
			free(this->array);
			row = mat.row;
			column = mat.column;
			array = mat.array;
			mat.array = NULL;
			mat.row = 0;
			mat.column = 0;
		}

		return *this;
	}

	// copy constructors
	Matrix(const Matrix& mat) {
		row = mat.row;
		column = mat.column;
		array = (Vector *)malloc(sizeof(Vector) * row);
		for (int i = 0; i < row; i++) {
			array[i] = Vector(column);
		}

		for(int i = 0; i < row; i++) {
			for(int j = 0; j < column; j++) {
				array[i].array[j] = mat[i][j];
			}
		}
	}

	// move constructors, no except just to please visual studio
	Matrix(Matrix&& mat) noexcept {
		row = mat.row;
		column = mat.column;
		array = mat.array;
		mat.array = nullptr;
	}

	Vector& operator [](const int index) const{
		return array[index];
	}

	~Matrix() {
		free(array);
	}
};

void print(Matrix mat);

/*
* Matrix
*/
Matrix initMatrix(int row, int col) {
	Matrix mat;
	mat.row = row;
	mat.column = col;
	mat.array = (Vector *)malloc(sizeof(Vector) * mat.row);
	for (int i = 0; i < mat.row; i++) {
		mat[i] = Vector(mat.column);
	}
	return mat;
}

void freeMatrix(Matrix *mat) {
	for(int i = 0; i < mat->row; i++) {
		free(mat->array[i].array);
	}

	free(mat->array);
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



/*convets a vector to a matrix of size(n, 1)*/
Matrix vectorToMatrix(Vector vec) {
	Matrix result = initMatrix(vec.size, 1);
	for (int i = 0; i < vec.size; i++) {
		setMatrixElement(&result, i, 0, vec.array[i]);
	}
	return result;
}


Vector matrixToVector(Matrix mat) {
	Vector result = Vector(mat.row);
	for (int i = 0; i < mat.row; i++) {
		result.array[i] = getMatrixElement(mat, i, 0);
	}
	return result;
}



void setMatrixArray(Matrix* mat, float* array) {
	for (int i = 0; i < mat->row; i++) {
		for (int j = 0; j < mat->column; j++) {
			(*mat)[i][j] = array[mat->column * i + j];
		}
	}
}

/*
* returns the transpose of the matrix
*/
Matrix transpose(Matrix mat) {
	Matrix result = initMatrix(mat.column, mat.row);
	for (int i = 0; i < result.row; i++) {
		Vector* vec = mat.array + i;
		for (int j = 0; j < result.column; j++) {
			//vec->array[j] = getMatrixElement(mat, j, i);
			setMatrixElement(&result, i, j, getMatrixElement(mat, j, i));
		}
	}
	return result;
}



Matrix matrixSubtract(Matrix mat1, Matrix mat2) {
	Matrix result = initMatrix(mat1.row, mat1.column);
	if (mat1.row != mat2.row && mat1.column != mat2.column) {
		printf("Dimension error\n");
		exit(-1);
	}
	for (int i = 0; i < mat1.row; i++) {
		for (int j = 0; j < mat1.column; j++) {
			setMatrixElement(&result, i, j, getMatrixElement(mat1, i, j) - getMatrixElement(mat2, i, j));
		}
	}
	return result;
}

/*
	Calculates the product of two matrices
*/
Matrix operator *(Matrix mat1, Matrix mat2) {
	Matrix result(mat1.row, mat2.column);
	if (mat1.column != mat2.row) {
		printf("Dimension error\n");
		exit(-1);
	}
	for(int i = 0; i < mat1.row; i++) {
		for (int j = 0; j < mat2.column; j++) {
			for (int k = 0; k < mat1.column; k++) {
				result[i][j] = getMatrixElement(result, i, j) + getMatrixElement(mat1, i, k) * getMatrixElement(mat2, k, j);
			}
		}
	}
	return result;
}

Matrix matrixMultiply(Matrix mat1, Matrix mat2) {
	Matrix result = initMatrix(mat1.row, mat2.column);
	if (mat1.column != mat2.row) {
		printf("Dimension error\n");
		exit(-1);
	}
	for(int i = 0; i < mat1.row; i++) {
		for (int j = 0; j < mat2.column; j++) {
			for (int k = 0; k < mat1.column; k++) {
				result[i][j] = getMatrixElement(result, i, j) + getMatrixElement(mat1, i, k) * getMatrixElement(mat2, k, j);
			}
		}
	}
	return result;
}


/*
	Calculates the product of matrix and vector
*/
Vector multiplyMatrixVector(Matrix mat, Vector vec) {
	Vector result = Vector(vec.size);
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
	Matrix result(mat.row, mat.column);
	if (mat.row != vec.size) {
		printf("Please check the dimension\n");
		exit(-1);
	}
	for (int i = 0; i < mat.row; i++) {
		for (int j = 0; j < mat.column; j++) {
			float sum = getMatrixElement(mat, i, j) + vec.array[i];
			setMatrixElement(&result, i, j, sum);
		}
	}
	return result;
}

Matrix elementWiseProduct(Matrix mat1, Matrix mat2) {
	Matrix result = initMatrix(mat1.row, mat1.column);
	if (mat1.row != mat2.row || mat1.column != mat2.column) {
		printf("Dimension error\n");
		exit(-1);
	}
	for (int i = 0; i < mat1.row; i++) {
		for (int j = 0; j < mat1.column; j++) {
			setMatrixElement(&result, i, j, getMatrixElement(mat1, i, j) * getMatrixElement(mat2, i, j));
		}
	}
	return result;
}

Matrix scalematrix(Matrix mat1, float scale) {
	Matrix result = initMatrix(mat1.row, mat1.column);
	for (int i = 0; i < mat1.row; i++) {
		for (int j = 0; j < mat1.column; j++) {
			setMatrixElement(&result, i, j, getMatrixElement(mat1, i, j) * scale);
		}
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
	Vector result = Vector(vec.size);
	for(int i = 0; i < vec.size; i++){
		result.array[i] = sigmoid(vec.array[i]);
	}
	return result;
}



Matrix sigmoidMatrix(Matrix mat) {
	Matrix result = initMatrix(mat.row, mat.column);
	for(int i = 0; i < mat.row; i++){
		for(int j = 0; j < mat.column; j++){
			result.array[i].array[j] = sigmoid(mat.array[i].array[j]);
		}
	}
	return result;
}


// derivative of a sigmoid
float derivativeSigmoid(float x) {
	return sigmoid(x) * (1 - sigmoid(x));
}


Vector derivativeSigmoidVector(Vector vec) {
	Vector result = Vector(vec.size);
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

void print(Matrix mat) {
	for (int i = 0; i < mat.row; i++) {
		for (int j = 0; j < mat.column; j++) {
			printf("%f   ", getMatrixElement(mat, i, j));
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
	printf("\n");
}
