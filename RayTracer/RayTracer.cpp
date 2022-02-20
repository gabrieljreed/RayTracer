#include <iostream>
#include <fstream>

#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"

using namespace std;

int main() {
    ofstream renderFile("renderFile.ppm");

    Vector cameraOrigin = Vector(0, 0, 0);

    Sphere s1 = Sphere(Vector(0, 0, -1), 0.4, Vector(255, 0, 255), Vector(1, 1, 1), 0.7, 0.2, 0.1, 16.0);

    // Lighting 
    Vector directionToLight = Vector(1, 1, -0.5);
    Vector lightColor = Vector(255, 255, 255);
    Vector ambientLighting = Vector(0.1, 0.1, 0.1) * 10;

    // Colors 
    Vector backgroundColor = Vector(0.2, 0.2, 0.2) * 255;
    Vector colorWhite = Vector(255, 255, 255);
    Vector colorRed = Vector(255, 0, 0);

    float imagePlaneX = 2; // FIXME: I need a better way to define the viewplane and file size 
    float imagePlaneY = 2;
    float imagePlaneZ = 4;

    // Output file dimensions 
    int dimensionX = 500;
    int dimensionY = 500;

    renderFile << "P3" << endl << dimensionX << " " << dimensionY << endl << "255" << endl;
    for (int j = dimensionY - 1; j >= 0; j--) {
        for (int i = 0; i < dimensionX; i++) {
            float x = (float(i) / float(dimensionX) * imagePlaneX) - (imagePlaneX / 2);
            float y = (float(j) / float(dimensionY) * imagePlaneY) - (imagePlaneY / 2);

            Vector direction = Vector(x, y, -1); 
            direction /= direction.getLength();

            Ray ray = Ray(cameraOrigin, direction);

            // See if ray hits sphere 
            // 1 - Determine whether the ray's origin is outside the sphere
            Vector OC = s1.center - ray.origin;
            bool insideSphere;
            if (OC.getLength() < s1.radius) {
                insideSphere = true;
            }
            else {
                insideSphere = false;
            }

            // 2 - Find tCA: closest approach of ray to sphere center 
            float tCA = OC.dot(ray.direction);

            // 3 - If tCA is less than 0 and the ray origin is outside the sphere, the ray does not intersect 
            if (tCA < 0 && !insideSphere) {
                renderFile << backgroundColor.toString() << endl;
            }

            // 4 - Find tHC2: distance from closest approach to sphere surface 
            float tCH2 = s1.radius * s1.radius - OC.getLength() * OC.getLength() + tCA * tCA;

            // 5 - If tCH2 < 0, the ray does not intersect the sphere 
            if (tCH2 < 0) {
                renderFile << backgroundColor.toString() << endl;
            }
            else {
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
                Vector surfaceNormal = Vector((intersectionPoint.x - s1.center.x) / s1.radius,
                    (intersectionPoint.y - s1.center.y) / s1.radius,
                    (intersectionPoint.z - s1.center.z) / s1.radius);

                // Write color to renderFile 
                renderFile << s1.calculateColor(surfaceNormal, directionToLight, ambientLighting, lightColor, Vector(0, 0, -1)).toString() << endl; // FIXME: Is ray.direction the right "view" vector? 
            }
        }
    }

    renderFile.close();
}

