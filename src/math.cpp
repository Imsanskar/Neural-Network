#include "Math/matrix.h"
#include "Math/vec.h"

namespace MathLib {
	//mat3 constructor
	mat3::mat3(mat4 mat4){
		for (int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				mat[i][j] = mat4.mat[i][j];
			}
		}
		
	}



	//matrix multiplication
	mat4 operator *(mat4 a, mat4 b) {
		mat4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					result.mat[i][j] += a.mat[i][k] * b.mat[k][j];
				}
			}
		}
		return result;
	}


	mat4 translate(const mat4& mat, float tx, float ty, float tz) {
		mat4 result = mat;
		result.mat[3][0] += tx;
		result.mat[3][1] += ty;
		result.mat[3][2] += tz;
		return result * mat;
	}


	mat4 scale(const mat4& mat, vec3 vec) {
		mat4 result = mat;
		result.mat[0][0] *= vec.x;
		result.mat[1][1] *= vec.y;
		result.mat[2][2] *= vec.z;
		return result * mat;
	}



	mat4 translate(const mat4& mat, vec3 vec) {
		mat4 result = mat;
		result.mat[3][0] += vec.x;
		result.mat[3][1] += vec.y;
		result.mat[3][2] += vec.z;

		return result * mat;
	}

	mat4 rotateX(const mat4& matrix, float angle) {
		mat4 result(1.0f);

		result.mat[1][1] = cos(angle);
		result.mat[1][2] = -sin(angle);
		result.mat[2][1] = sin(angle);
		result.mat[2][2] = cos(angle);

		result = result.transpose();

		return result.transpose() * matrix;
	}


	mat4 rotateY(const mat4& matrix, float angle) {
		mat4 result(1.0f);

		result.mat[0][0] = cos(angle);
		result.mat[0][2] = sin(angle);
		result.mat[2][0] = -sin(angle);
		result.mat[2][2] = result.mat[0][0];

		result = result.transpose();

		return result * matrix;
	}


	mat4 rotateZ(const mat4& matrix, float angle) {
		mat4 result(1.0f);

		result.mat[0][0] = cos(angle);
		result.mat[0][1] = -sin(angle);
		result.mat[1][0] = sin(angle);
		result.mat[1][1] = cos(angle);
		
		result = result.transpose();

		return result * matrix;
	}


	mat4 rotate(const mat4& matrix, const vec3& vec, const vec3& point, const float angle) {
		mat4 result(1.0f);
		vec3 axis = vec.unitVector();

		result = translate(result, -point);
		result = translate(result, point);


		return result * matrix;
	}

	mat4 rotate(const mat4& matrix, const float angle, const vec3& vec) {
		mat4 result(1.0f);
		vec3 axis = vec.unitVector();

		const float sinVal = sin(angle);
		const float cosVal = cos(angle);

		const float x = axis.x;
		const float y = axis.y;
		const float z = axis.z;

		result.mat[0][0] = cosVal + x * x * (1 - cosVal);
		result.mat[0][1] = x * y * (1 - cosVal) - z * sinVal;
		result.mat[0][2] = x * z * (1 - cosVal) + y * sinVal;

		result.mat[1][0] = x * y * (1 - cosVal) + z * sinVal;
		result.mat[1][1] = cosVal + y * y * (1 - cosVal);
		result.mat[1][2] = z * y * (1 - cosVal) - x * sinVal;

		result.mat[2][0] = x * z * (1 - cosVal) - y * sinVal;
		result.mat[2][1] = z * y * (1 - cosVal) + x * sinVal;
		result.mat[2][2] = cosVal + z * z * (1 - cosVal);

		result = result.transpose();

		return result * matrix;
	}

	mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
		mat4 result(1.0f);

		auto f = (-center + eye).unitVector();
		auto s = cross(up, f).unitVector();
		auto u = cross(f, s).unitVector();

		result.mat[0][0] = s.x;
		result.mat[1][0] = s.y;
		result.mat[2][0] = s.z;


		result.mat[0][1] = u.x;
		result.mat[1][1] = u.y;
		result.mat[2][1] = u.z;


		result.mat[0][2] = f.x;
		result.mat[1][2] = f.y;
		result.mat[2][2] = f.z;


		result.mat[3][0] = -s.dot(eye);
		result.mat[3][1] = -u.dot(eye);
		result.mat[3][2] = -f.dot(eye);

		return result;
	}


	mat4 perspective(float fov, float aspectRatio, float near, float far){
		mat4 result;

		float scale = 1.0 / tan(fov / 2);
        result[0][0] = scale/aspectRatio; // scale the x coordinates
        result[1][1] = scale; // scale the y coordinates
        result[2][2] = -(far+near) / (far - near); // remap z to [0,1] 
        result[2][3] = -1; 
        result[3][2] = (2*far*near)/(near - far); 

		return result;
	}


	mat4 ortho(float left, float right, float bottom, float top, float near, float far){
		mat4 result(1.0f);

		result[0][0] = 2.0f / (right - left);
		result[1][1] = 2.0f / (top - bottom);
		result[2][2] = - 2.0f / (far - near);
		result[3][0] = - (right + left) / (right - left);
		result[3][1] = - (top + bottom) / (top - bottom);
		result[3][2] = - (far + near) / (far - near);

		return result;
	}
}