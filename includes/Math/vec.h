#pragma once
#include <math.h>
#include <iostream>

#define PI 3.141519

#define to_radians(deg) ((deg) * (PI / 180.0f))
#define to_degrees(rad) ((rad) * (180.0f / PI))

namespace MathLib {
	union vec2 {
		float m[2];

		struct {
			float x, y;
		};

		inline vec2() {
			x = 0;
			y = 0;
		}
		inline vec2(float a) { x = y = a; }
		inline vec2(float a, float b) { x = a; y = b; }
		inline float* value_ptr() {
			return &(this->x);
		}
	};


	struct vec3 {
		struct{
			float x, y, z;
		};
		vec3(float val = 0) {
			x = y = z = val;
		}

		vec3(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		inline float* value_ptr() {
			return &(this->x);
		}

		inline vec3 operator +(const vec3& v) const{
			return vec3(x + v.x, y + v.y, z + v.z);
		}

		inline vec3 operator +(float v) const{
			return vec3(x + v, y + v, z + v);
		}

		inline vec3 operator -(const vec3& v) const{
			return vec3(x - v.x, y - v.y, z - v.z);
		}

		inline vec3 operator -(float v) const{
			return vec3(x - v, y - v, z - v);
		}


		inline vec3 operator *(const vec3& v) const{
			return vec3(x * v.x, y * v.y, z * v.z);
		}

		inline vec3 operator *(float v) const{
			return vec3(x * v, y * v, z * v);
		}

		inline vec3 operator /(float v) const{
			return vec3(x / v, y / v, z / v);
		}

		inline float norm() const{
			return sqrt(x * x + y * y + z * z);
		}

		inline vec3 unitVector() const{
			float magnitude = norm();

			if(magnitude > 0)
				return vec3(x / magnitude, y / magnitude, z / magnitude);
			return vec3(0.0f);
		}

		inline vec3 operator +=(vec3 vec){
			x += vec.x;
			y += vec.y;
			z += vec.z;

			return *this; 
		}

		inline vec3 operator -=(vec3 vec){
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;

			return *this; 
		}

		inline vec3 cross(vec3 v) {
			vec3 result;

			result.x = y * v.z - v.y * z;
			result.y = - (x * v.z - v.x * z);
			result.z = x * v.z - z * v.x;

			return result;
		}


		inline float dot(vec3 vec) const{
			return x * vec.x + y * vec.y + z * vec.z;
		}
	};
	
	inline vec3 cross(vec3 vec1, vec3 v){
		vec3 result;

		float x = vec1.x;
		float y = vec1.y;
		float z = vec1.z;

		result.x = y * v.z - v.y * z;
		result.y = - (x * v.z - v.x * z);
		result.z = x * v.y - y * v.x;

		return result;
	}

	inline vec3 normalize(vec3 vec){
		float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

		if(magnitude > 0)
			return vec3(vec.x / magnitude, vec.y / magnitude, vec.z / magnitude);
		return vec3(0.0f);
	}


	inline vec3 operator -(const vec3& vec) {
		return vec3(-vec.x, -vec.y, -vec.z);
	}


	inline vec3 operator * (float val, const vec3& vec){
		return vec3(vec.x * val, vec.y * val, vec.z * val);
	}


	inline std::ostream& operator <<(std::ostream& os, vec3 vec){
		os << '(' << vec.x << ',' << vec.y << ',' << vec.z << ")\n";
    	return os << std::endl;
  	}
}