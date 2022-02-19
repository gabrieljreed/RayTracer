#pragma once

#include <iostream>
#include <math.h>
#include <string>

using namespace std;

class Vector {
public:
	float x, y, z;

	// Constructor 
	Vector() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// Length 
	float getLength() {
		return sqrt(x*x + y*y + z*z);
	}

	// Unit vector 
	void unitVector() {
		float length = getLength();
		this->x /= length;
		this->y /= length;
		this->z /= length;

		return;
	}

	// << Operator 
	friend ostream & operator<<(ostream &out, const Vector &v) {
		out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return out;
	}

	// Add 
	friend Vector operator+(const Vector v1, const Vector v2) {
		return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	// Subtract 
	friend Vector operator-(const Vector v1, const Vector v2) {
		return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	// Multiply 
	friend Vector operator*(const Vector v1, const Vector v2) {
		return Vector(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}

	// Divide 
	friend Vector operator/(const Vector v1, const Vector v2) {
		return Vector(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
	}

	// Scalar multiply 
	friend Vector operator*(const float s, const Vector v) {
		return Vector(v.x * s, v.y * s, v.z * s);
	}

	friend Vector operator*(const Vector v, const float s) {
		return Vector(v.x * s, v.y * s, v.z * s);
	}

	// Scalar divide 
	friend Vector operator/(const float s, const Vector v) {
		return Vector(v.x / s, v.y / s, v.z / s);
	}

	friend Vector operator/(const Vector v, const float s) {
		return Vector(v.x / s, v.y / s, v.z / s);
	}

	// Dot product 
	float dot(Vector v) {
		return (this->x * v.x) +
			   (this->y * v.y) +
			   (this->z * v.z);
	}

	// Cross product 
	Vector cross(const Vector v1, const Vector v2) {
		return Vector((v1.y * v2.z) - (v1.z * v2.y), 
					  -(v1.x * v2.z) - (v1.z * v2.x),
					  (v1.x * v2.y) - (v1.y * v2.x));
	}

	// +=
	Vector& operator+=(const Vector v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		
		return *this;
	}

	// -=
	Vector& operator-=(const Vector v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	// *= 
	inline Vector& operator*=(const Vector v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;

		return *this;
	}

	inline Vector& operator*=(const float s) {
		this->x *= s;
		this->y *= s;
		this->z *= s;

		return *this;
	}

	// /=
	inline Vector& operator/=(const Vector v) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;

		return *this;
	}

	inline Vector& operator/=(const float s) {
		this->x /= s;
		this->y /= s;
		this->z /= s;

		return *this;
	}

	string toString() {
		return to_string((int)x) + " " + to_string((int)y) + " " + to_string((int)z);
	}
};
