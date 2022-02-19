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
		// Original slow method 
		//// (x-xc)^2 + (y-yc)^2 + (z-zx)^2 = r^2
		/*float A = 1;
		float B = 2 * (ray.direction.x*ray.origin.x - ray.direction.x*center.x + 
					   ray.direction.y*ray.origin.y - ray.direction.y*center.y + 
					   ray.direction.z*ray.origin.z - ray.direction.z*center.z);

		float C = ray.origin.x*ray.origin.x - 2*ray.origin.x*center.x + center.x*center.x +  
				  ray.origin.y*ray.origin.y - 2*ray.origin.y*center.y + center.y*center.y + 
				  ray.origin.z*ray.origin.z - 2*ray.origin.z*center.z + center.z*center.z 
			- (radius * radius);

		if (B*B - 4*C >= 0) {
			return true;
		}

		return false;*/

		// Internet method 
		/*Vector OC = ray.origin - center;
		Vector d = ray.direction;
		float a = d.dot(ray.direction);
		float b = 2.0 * OC.dot(ray.direction);
		float c = OC.dot(OC) - (radius * radius);
		float discriminant = b * b - 4 * a * c;

		return (discriminant > 0);
		*/

		// Fast method 
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
