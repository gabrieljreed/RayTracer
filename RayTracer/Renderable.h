#pragma once

#include "Vector.h"
#include "Ray.h"

using namespace std;

class Renderable {

	// Renderable(); // it's pure virtual, so you'd never want a constructor anyways? 

	virtual Vector calculateColor(Vector surfaceNormal, Vector lightDirection, Vector ambientIntensity, Vector lightColor, Vector view) = 0;

protected:
	// Shading 
	Vector Od, Os;
	float Ka, Kd, Ks, kGls, Refl;
};

