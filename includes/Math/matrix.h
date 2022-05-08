#pragma once
#include <vector>
#include "vec.h"
#include <iostream>


namespace MathLib {

	struct mat4;

	struct mat3{
		float mat[3][3];

		mat3() {
			
		}

		mat3(float x){
			for (int i = 0; i < 3; i++) {
				mat[i][i] = x;
			}
		}

		mat3(mat4 mat4);

		float& operator [](const std::pair<int, int>& index) {
			return mat[index.first][index.second];
		}
 
		mat3 operator +(mat3 m) {
			mat3 result;

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					result.mat[i][j] = mat[i][j] + m.mat[i][j];
				}
			}
			return result;
		}



		mat3 operator -(mat3 m) {
			mat3 result;

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					result.mat[i][j] = mat[i][j] - m.mat[i][j];
				}
			}
			return result;
		}

		bool operator ==(mat3 m) {
			// bool flag = true;
			// for (int i = 0; i < 3; i++) {
			// 	flag = this->mat[i] == m[i];
			// 	if (!flag)
			// 		return flag;
			// }
			// return flag;
			return false;
		}

		float* value_ptr() {
			return &mat[0][0];
		}


		mat3 transpose(){
			mat3 result;

			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					result.mat[i][j] = mat[j][i];
				}
			}
			return result;
		}
	};

	 inline std::ostream& operator<<(std::ostream& os, mat3 matrix){
    	std::cout << "\nOutput in Row major order -> \n";
    	for (int i = 0; i < 3; ++i)
    	{
    	  for (int j = 0; j < 3; ++j)
    	  {
	os 	<< '[' << i << ']' << '[' << j << ']' <<" -> " << matrix.mat[j][i] << "   "; 
    	  }
    	  os << '\n';
    	}
    	return os << std::endl;
	};


	struct mat4 {
		float mat[4][4] =  {0.0f};

		mat4() {
			
		}
		

		mat4(mat3 mat3){
			for (int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					mat[i][j] = mat3.mat[i][j];
				}
			}
			mat[3][3] = 1.0f;
		}

		mat4(float x){
			for (int i = 0; i < 4; i++) {
				mat[i][i] = x;
			}
		}

		float* operator [](const int index){
			return mat[index];
		}

		float& operator [](const std::pair<int, int>& index) {
			return mat[index.first][index.second];
		}
 
		mat4 operator +(mat4 m) {
			mat4 result;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.mat[i][j] = mat[i][j] + m.mat[i][j];
				}
			}
			return result;
		}



		mat4 operator -(mat4 m) {
			mat4 result;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.mat[i][j] = mat[i][j] - m.mat[i][j];
				}
			}
			return result;
		}

		bool operator ==(mat4 m) {
			// bool flag = true;
			// for (int i = 0; i < 4; i++) {
			// 	flag = this->mat[i] == m[i];
			// 	if (!flag)
			// 		return flag;
			// }
			// return flag;
			return false;
		}

		float* value_ptr() {
			return &mat[0][0];
		}


		mat4 transpose(){
			mat4 result;

			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					result.mat[i][j] = mat[j][i];
				}
			}
			return result;
		}
	};

	 inline std::ostream& operator<<(std::ostream& os, mat4 matrix){
    	for (int i = 0; i < 4; ++i)
    	{
    	  for (int j = 0; j < 4; ++j)
    	  {
	os 	<< '[' << i << ']' << '[' << j << ']' <<" -> " << matrix.mat[j][i] << "   "; 
    	  }
    	  os << '\n';
    	}
    	return os << std::endl;
  	}

	mat4 operator *(mat4 a, mat4 b);

	mat4 translate(const mat4& mat, float tx, float ty, float tz);

	mat4 translate(const mat4& mat, vec3 vec);

	mat4 scale(const mat4& mat, vec3 vec);

	mat4 rotateX(const mat4& matrix, float angle);

	mat4 rotateY(const mat4& matrix, float angle);

	mat4 rotateZ(const mat4& matrix, float angle);

	mat4 rotate(const mat4& matrix, const float angle, const vec3& vec);

	mat4 lookAt(const vec3& cameraPos, const vec3& target, const vec3& up);

	mat4 perspective(float fov, float aspectRatio, float near = 0.1, float far = 100.0f);

	mat4 ortho(float left, float right, float bottom, float top, float near = 0.1, float far = 100.0f);
}