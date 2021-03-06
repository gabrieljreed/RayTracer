#pragma once

#include "Vector.h"
#include "Ray.h"

using namespace std;

class Renderable {
public:
	virtual float calculateIntersectionDistance(const Ray& ray) {
		return FLT_MAX;
	}

	virtual Vector calculateSurfaceNormal(const Vector& intersectionPoint) {
		return Vector(0, 0, 0);
	}

	virtual Vector calculateColor(Vector surfaceNormal, Vector lightDirection, Vector ambientIntensity, Vector lightColor, Vector view, bool isInShadow) {
		return Vector(0, 0, 0);
	}

	virtual float getRefl() {
		return Refl;
	}


	// Shading 
	Vector Od, Os;
	float Ka, Kd, Ks, kGls, Refl;
};

