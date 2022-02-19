#pragma once

#include <math.h>

#include "Vector.h"
#include "Ray.h"

using namespace std;

class Sphere {
public:
	Vector center;
	float radius;

	Sphere() {
		center = Vector();
		radius = 0;
	}

	Sphere(const Vector& center, const float& radius) {
		this->center = center;
		this->radius = radius; 
	}

	bool isOnSphere(const Ray& ray) {
		// 1 - Determine whether the ray's origin is outside the sphere 
		Vector OC = center - ray.origin;
		bool insideSphere;
		if (OC.getLength() < radius) {
			insideSphere = true;
		}
		else {
			insideSphere = false;
		}
		// 2 - Find tCA: closest approach of ray to sphere center 
		float tCA = OC.dot(ray.direction);

		// 3 - If tCA is less than 0 and the ray origin is outside the sphere, the ray does not intersect 
		if (tCA < 0 && !insideSphere) {
			return false;
		}

		// 4 - Find tHC2: distance from closest approach to sphere surface 
		float tCH2 = radius * radius - OC.getLength() * OC.getLength() + tCA * tCA;

		// 5 - If tCH2 < 0, the ray does not intersect the sphere 
		if (tCH2 < 0) {
			return false;
		}

		// 6 - Find t: intersection distance 
		float t;
		if (insideSphere) {
			t = tCA - sqrt(tCH2);
		}
		else {
			t = tCA + sqrt(tCH2);
		}

		// 7 - Calculate intersection point 
		Vector intersectionPoint = Vector(ray.origin.x + ray.direction.x * t,
										  ray.origin.y + ray.direction.y * t,
										  ray.origin.z + ray.direction.z * t);

		// 8 - Calculate surface normal 
		Vector surfaceNormal = Vector((intersectionPoint.x - center.x) / radius, (intersectionPoint.y - center.y) / radius, (intersectionPoint.z - center.z) / radius);

		return true;
	}
};
