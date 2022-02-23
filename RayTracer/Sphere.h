#pragma once

#include <math.h>

#include "Vector.h"
#include "Ray.h"

using namespace std;

class Sphere {
public:
	Vector center;
	float radius;
	
	// Shading 
	Vector Od, Os;
	float Ka, Kd, Ks, kGls;

	Sphere() {
		center = Vector();
		radius = 0;
	}

	Sphere(const Vector& center, const float& radius) {
		this->center = center;
		this->radius = radius; 
	}

	Sphere(const Vector& center, const float& radius, const Vector& color, const Vector& specularColor, const float& Kd, const float& Ks, const float& Ka, const float& kGls) {
		this->center = center;
		this->radius = radius;
		this->Od = color;
		this->Os = specularColor;
		this->Kd = Kd;
		this->Ks = Ks;
		this->Ka = Ka;
		this->kGls = kGls;
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
			//return Vector(255, 255, 255);
			return false;
		}

		// 4 - Find tHC2: distance from closest approach to sphere surface 
		float tCH2 = radius * radius - OC.getLength() * OC.getLength() + tCA * tCA;

		// 5 - If tCH2 < 0, the ray does not intersect the sphere 
		if (tCH2 < 0) {
			//return Vector(255, 255, 255);
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

	Vector calculateColor(Vector surfaceNormal, Vector lightDirection, Vector ambientIntensity, Vector lightColor, Vector view) {
		lightDirection.unitVector();
		surfaceNormal.unitVector();
		Vector diffuse = Kd * lightColor * Od * max((float)0, surfaceNormal.dot(lightDirection));

		Vector ambient = ambientIntensity * Ka * Od;

		Vector R = 2 * surfaceNormal * surfaceNormal.dot(lightDirection) - lightDirection;
		Vector specular = Ks * lightColor * Os * pow(max((float)0, view.dot(R)), kGls) *255;

		Vector result = diffuse + ambient + specular;
		//result = specular;
		// result.clamp(255); // In theory, you shouldn't have to clamp 

		return result;
	}
};
