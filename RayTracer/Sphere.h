#pragma once

#include <math.h>
#include <cmath>

#include "Renderable.h"
#include "Vector.h"
#include "Ray.h"

using namespace std;

class Sphere : public Renderable {
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

	Sphere(const Vector& center, const float& radius, const Vector& color, const Vector& specularColor, const float& Kd, const float& Ks, const float& Ka, const float& kGls, const float& Refl) {
		this->center = center;
		this->radius = radius;
		this->Od = color;
		this->Os = specularColor;
		this->Kd = Kd;
		this->Ks = Ks;
		this->Ka = Ka;
		this->kGls = kGls;
		Renderable::Refl = Refl; 
	}

	float getRefl() {
		return Refl;
	}

	float calculateIntersectionDistance(const Ray& ray) {
		Vector OC = center - ray.origin;
		// 1 - Determine if the ray origin is inside the sphere 
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
			return FLT_MAX;
		}

		// 4 - Find tHC2: distance from closest approach to sphere surface 
		float tCH2 = radius * radius - OC.getLength() * OC.getLength() + tCA * tCA;

		// 5 - If tCH2 < 0, the ray does not intersect the sphere 
		if (tCH2 < 0) {
			return FLT_MAX;
		}
		// 6 - Find t: intersection distance 
		float t;

		if (insideSphere) {
			t = tCA + sqrt(tCH2);
		}
		else {
			t = tCA - sqrt(tCH2);
		}

		return t;
	}

	Vector calculateSurfaceNormal(const Vector& intersectionPoint) {
		return (intersectionPoint - center) / radius;
	}

	Vector calculateColor(Vector surfaceNormal, Vector lightDirection, Vector ambientIntensity, Vector lightColor, Vector view, bool isInShadow) {
		lightDirection.unitVector();
		surfaceNormal.unitVector();

		Vector result = Vector(0, 0, 0);

		if (isInShadow) {  // The ray hit something else on the way to the light 
			result = ambientIntensity * Ka * Od;
		}
		else {  // The ray didn't hit anything and is not in shadow 
			Vector diffuse = Kd * lightColor * Od * max((float)0, surfaceNormal.dot(lightDirection));

			Vector ambient = ambientIntensity * Ka * Od;

			lightDirection *= -1;
			Vector R = 2 * surfaceNormal * surfaceNormal.dot(lightDirection) - lightDirection;


			Vector specular = Ks * lightColor * Os * pow(max((float)0, view.dot(R)), kGls) * 255;

			result = diffuse + ambient + specular;

			if (result.x > 255) result.x = 255;
			if (result.y > 255) result.y = 255;
			if (result.z > 255) result.z = 255;
		}

		return result;
	}
};
