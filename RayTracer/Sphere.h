#pragma once

#include "Vector.h"

using namespace std;

class Sphere {
public:
	Vector origin;
	float radius;

	Sphere() {
		origin = Vector();
		radius = 0;
	}

	Sphere(const Vector& origin, const float& radius) {
		this->origin = origin;
		this->radius = radius; 
	}
};
