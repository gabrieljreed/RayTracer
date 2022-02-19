#pragma once

#include "Vector.h"

using namespace std;

class Ray {
public: 
	Vector origin, direction;
	float length;
	
	Ray() {}
	Ray(const Vector& origin, const Vector& direction) {
		this->origin = origin;
		this->direction = direction;
	}
	Ray(const Vector& origin, const Vector& direction, const float& length) {
		this->origin = origin;
		this->direction = direction;
		this->length = length;
	}

	Vector getPoint() {
		return origin + direction * length;
	}

	Vector getPoint(const float length) {
		return origin + direction * length;
	}

	

};
