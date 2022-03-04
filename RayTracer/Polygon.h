#pragma once

#include <vector>

#include "Renderable.h"
#include "Vector.h"
#include "Ray.h"

using namespace std;

class Polygon : public Renderable {
public:
	vector<Vector> vertices; 

	// Shading 
	Vector Od, Os;
	float Ka, Kd, Ks, kGls, Refl;

	Polygon() {}
	Polygon(vector<Vector> vertices, float Kd, float Ks, float Ka, Vector Od, Vector Os, float kGls, float Refl) {
		this->vertices = vertices;
		this->Kd = Kd;
		this->Ks = Ks;
		this->Ka = Ka;
		this->Od = Od;
		this->Os = Os;
		this->kGls = kGls;
		this->Refl = Refl;
	}

	virtual float calculateIntersectionDistance(const Ray& ray) {
		// Calculate plane that ray lies on 
		// Poly must have at least 3 verts 
		if (vertices.size() > 3) { 
			return FLT_MAX;
		}

		Vector v1 = vertices[0] - vertices[1];
		Vector v2 = vertices[2] - vertices[1];

		Vector planeNormal = v1.cross(v2, v1);

		// Find distanceToPlane
		float distanceToPlane = -(planeNormal.x + planeNormal.y + planeNormal.z);
		
		// Ray-Plane intersection 
		float Vd = planeNormal.dot(ray.direction);

		if (Vd >= 0) {
			return FLT_MAX; // Ray is pointing away (>0) or parallel to plane (=0)
		}

		float Vo = -(planeNormal.dot(ray.origin) + distanceToPlane);

		float planeIntersectionDistance = Vo / Vd;

		if (planeIntersectionDistance < 0) {
			return FLT_MAX; // Point is behind ray 
		}

		if (Vd > 0) {
			planeNormal *= -1; // Reverse plane normal 
		}

		// Calculate intersection point 
		Vector intersectionPoint = Vector(ray.origin.x + ray.direction.x * planeIntersectionDistance,
			ray.origin.y + ray.direction.y * planeIntersectionDistance,
			ray.origin.z + ray.direction.z * planeIntersectionDistance);


		// Ray-Polygon intersection 

		// 1: Determine dominant coordinate of normal 
		float dominantIndex = 0; // x:0, y:1, z:2
		if (planeNormal.y > planeNormal.x) {
			dominantIndex++;
		}
		if (planeNormal.z > planeNormal.x || planeNormal.z > planeNormal.y) {
			dominantIndex++;
		}

		// 2: For vertex in polygon: project onto axis of dominant coordinate 

		// 3: Project intersection point onto axis of dominant coordinate 

		return FLT_MAX;
	}

	virtual Vector calculateSurfaceNormal(const Vector& intersectionPoint) {
		return Vector(0, 0, 0);
	}

	Vector calculateColor(Vector surfaceNormal, Vector lightDirection, Vector ambientIntensity, Vector lightColor, Vector view) {
		return Vector(0.0, 0.0, 0.0);
	}
};

