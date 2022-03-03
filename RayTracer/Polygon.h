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

	Vector calculateColor(Vector surfaceNormal, Vector lightDirection, Vector ambientIntensity, Vector lightColor, Vector view) {
		return Vector(0.0, 0.0, 0.0);
	}
};

