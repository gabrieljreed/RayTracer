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

	float E = 0.00001;

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

	float calculateIntersectionDistance(const Ray& ray) {


		/*
		// Calculate plane that ray lies on 
		Vector planeNormal = calculateSurfaceNormal(Vector(0, 0, 0));

		// Find distanceToPlane
		float distanceToPlane = abs(planeNormal.x + planeNormal.y + planeNormal.z); 
		
		// Ray-Plane intersection 
		float Vd = planeNormal.dot(ray.direction);

		if (Vd >= 0) {
			return FLT_MAX; // Ray is pointing away (>0) or parallel to plane (=0)
		}

		float Vo = -(planeNormal.dot(ray.origin) + distanceToPlane); // This is getting set to 0? 

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

		cout << intersectionPoint << endl;
		
		if (pointInsidePolygon(intersectionPoint)) {
			return planeIntersectionDistance;
		}
		else {
			return FLT_MAX;
		}
		*/








		// Calculate normal 
		Vector planeNormal = calculateSurfaceNormal(ray.direction);

		// Check if ray and plane are parallel 
		float rayDirection = planeNormal.dot(ray.direction);
		if (abs(rayDirection) < E) {
			return FLT_MAX;
		}

		// Distance (?)
		float d = -planeNormal.dot(vertices[0]); // FIXME: I'm not sure if verts[0] is the correct one 

		float t = -(planeNormal.dot(ray.origin) + d) / rayDirection;

		if (t < 0) return FLT_MAX; // Triangle is behind ray 

		Vector intersectionPoint = ray.origin + t * ray.direction;

		Vector C;

		// Edge 0
		Vector edge0 = vertices[1] - vertices[0];
		Vector V0 = intersectionPoint - vertices[0];
		C = edge0.cross(edge0, V0);
		if (planeNormal.dot(C) > 0) return FLT_MAX; // intersectionPoint is on the right side 

		// Edge 1 
		Vector edge1 = vertices[2] - vertices[1];
		Vector V1 = intersectionPoint - vertices[1];
		C = edge1.cross(edge1, V1);
		if (planeNormal.dot(C) > 0) return FLT_MAX; // intersectionPoint is on the right side 

		// Edge 2
		Vector edge2 = vertices[0] - vertices[2];
		Vector V2 = intersectionPoint - vertices[2];
		C = edge2.cross(edge2, V2);
		if (planeNormal.dot(C) > 0) return FLT_MAX; // intersectionPoint is on the right side 

		return t;
	}

	bool pointInsidePolygon(Vector intersectionPoint) {
		// Ray-Polygon intersection 

		Vector planeNormal = calculateSurfaceNormal(intersectionPoint);

		// 1: Determine dominant coordinate of normal 
		int dominantIndex = 0; // x:0, y:1, z:2
		if (abs(planeNormal.y) > abs(planeNormal.x)) {
			dominantIndex = 1;
		}
		if (abs(planeNormal.z) > abs(planeNormal.x) || abs(planeNormal.z) > abs(planeNormal.y)) {
			dominantIndex = 2;
		}

		// 2: For vertex in polygon: project onto axis of dominant coordinate 
		// 3: Project intersection point onto axis of dominant coordinate 
		switch (dominantIndex) {
		case 0:
			for (unsigned int i = 0; i < vertices.size(); i++) {
				vertices[i].x = vertices[i].y;
				vertices[i].y = vertices[i].z;
			}

			intersectionPoint.x = intersectionPoint.y;
			intersectionPoint.y = intersectionPoint.z;

			break;
		case 1:
			for (unsigned int i = 0; i < vertices.size(); i++) {
				vertices[i].y = vertices[i].z;
			}

			intersectionPoint.y = intersectionPoint.z;

			break;
		}

		// 4: Translate everything so the intersection point is in the center 

		for (unsigned int i = 0; i < vertices.size(); i++) {
			vertices[i] -= intersectionPoint;
		}

		//intersectionPoint = Vector(0, 0, 0); // FIXME: Do I maybe want to make this a new variable? 

		// 5: Set numCrossings = 0
		int numCrossings = 0;

		// 6: Set signHolder to the sign of the first vertex 
		int signHolder = 0;

		if (vertices[0].y < 0) {
			signHolder = -1;
		}
		else {
			signHolder = 1;
		}
		int nextSignHolder = 0;

		// 7: Loop time 

		for (unsigned int i = 0; i < vertices.size(); i++) {
			int k;
			if (i == vertices.size() - 1) {
				k = 0;
			}
			else {
				k = i + 1;
			}
			if (vertices[k].y < 0) {
				nextSignHolder = -1;
			}
			else {
				nextSignHolder = 1;
			}

			if (signHolder != nextSignHolder) { // If they have the same sign 
				if (vertices[i].x > 0 && vertices[k].x > 0) {
					numCrossings++;
				}
				else if (vertices[i].x > 0 || vertices[k].x > 0) {
					float xCross = vertices[i].x - vertices[i].y * (vertices[k].x - vertices[i].x) / (vertices[k].y - vertices[i].y);
					if (xCross > 0) {
						numCrossings++;
					}
				}
			}

			signHolder = nextSignHolder;
		}

		// 8: If numCrossings is odd, the point is inside the polygon 
		if (numCrossings % 2 == 0) {
			return false;
		}
		else {
			return true;
		}
	}

	Vector calculateSurfaceNormal(const Vector& intersectionPoint) {
		if (vertices.size() > 3) {
			return Vector(0, 0, 0);
		}

		Vector v1 = vertices[0] - vertices[1];
		Vector v2 = vertices[2] - vertices[1];

		return v1.cross(v1, v2);
	}

	Vector calculateColor(Vector surfaceNormal, Vector lightDirection, Vector ambientIntensity, Vector lightColor, Vector view) {
		return Vector(255.0, 255.0, 255.0);
	}

	float calculateTriangleIntersection(const Ray& ray) {
		// Calculate normal 
		Vector planeNormal = calculateSurfaceNormal(ray.direction);

		// Check if ray and plane are parallel 
		float rayDirection = planeNormal.dot(ray.direction);
		if (abs(rayDirection) < E) {
			return FLT_MAX;
		}

		// Distance (?)
		float d = -planeNormal.dot(vertices[0]); // FIXME: I'm not sure if verts[0] is the correct one 

		float t = -(planeNormal.dot(ray.origin) + d) / rayDirection;

		if (t < 0) return FLT_MAX; // Triangle is behind ray 

		Vector intersectionPoint = ray.origin + t * ray.direction;

		Vector C;

		// Edge 0
		Vector edge0 = vertices[1] - vertices[0];
		Vector V0 = intersectionPoint - vertices[0];
		C = edge0.cross(edge0, V0);
		if (planeNormal.dot(C) < 0) return FLT_MAX; // intersectionPoint is on the right side 

		// Edge 1 
		Vector edge1 = vertices[2] - vertices[1];
		Vector V1 = intersectionPoint - vertices[1];
		C = edge1.cross(edge1, V1);
		if (planeNormal.dot(C) < 0) return FLT_MAX; // intersectionPoint is on the right side 

		// Edge 2
		Vector edge2 = vertices[0] - vertices[2];
		Vector V2 = intersectionPoint - vertices[2];
		C = edge2.cross(edge2, V2);
		if (planeNormal.dot(C) < 0) return FLT_MAX; // intersectionPoint is on the right side 

		return t;
	}
};

