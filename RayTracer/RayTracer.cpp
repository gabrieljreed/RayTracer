#include <iostream>
#include <fstream>
#include <vector>

#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"

using namespace std;

float computeIntersectionDistance(const Sphere& sphere, const Ray& ray);

int main() {
    ofstream renderFile("renderFile.ppm");

    // Render objects
    Sphere s1 = Sphere(Vector(0, 0, -1), 0.4, Vector(255, 0, 255), Vector(1, 1, 1), 0.7, 0.2, 0.1, 16.0);
    Sphere s2 = Sphere(Vector(1, 1, -2), 0.8, Vector(0, 255, 0), Vector(1, 1, 1), 0.7, 0.2, 0.1, 16.0);
    vector<Sphere> spheres;
    spheres.push_back(s1);
    spheres.push_back(s2);

    // Lighting 
    Vector directionToLight = Vector(0, 1, 0);
    Vector lightColor = Vector(255, 255, 255);
    Vector ambientLighting = Vector(0.1, 0.1, 0.1) * 10;

    // Colors 
    Vector backgroundColor = Vector(0.2, 0.2, 0.2) * 255;
    Vector colorWhite = Vector(255, 255, 255);
    Vector colorRed = Vector(255, 0, 0);

    // Camera setup 
    Vector cameraOrigin = Vector(0, 0, 1);
    float imagePlaneX = 4; // FIXME: I need a better way to define the viewplane and file size 
    float imagePlaneY = 4;
    float imagePlaneZ = 4;

    // Output file dimensions 
    int dimensionX = 200;
    int dimensionY = 200;

    renderFile << "P3" << endl << dimensionX << " " << dimensionY << endl << "255" << endl;
    for (int j = dimensionY - 1; j >= 0; j--) {
        for (int i = 0; i < dimensionX; i++) {
            float x = (float(i) / float(dimensionX) * imagePlaneX) - (imagePlaneX / 2);
            float y = (float(j) / float(dimensionY) * imagePlaneY) - (imagePlaneY / 2);

            Vector direction = Vector(x, y, -1); 
            direction /= direction.getLength();

            Ray ray = Ray(cameraOrigin, direction);

            // Find closest sphere along ray 
            Sphere closestSphere; // Optimize this with pointers 
            float sphereDist = FLT_MAX;
            for (unsigned int k = 0; k < spheres.size(); k++) {
                float temp = computeIntersectionDistance(spheres[k], ray);
                if (temp < sphereDist) {
                    closestSphere = spheres[k];
                    sphereDist = temp;
                }
            }

            if (sphereDist == FLT_MAX) {
                // No spheres were hit 
                renderFile << backgroundColor.toString() << endl;
            }
            else {
                // 7 - Calculate intersection point 
                Vector intersectionPoint = Vector(ray.origin.x + ray.direction.x * sphereDist,
                    ray.origin.y + ray.direction.y * sphereDist,
                    ray.origin.z + ray.direction.z * sphereDist);

                // 8 - Calculate surface normal 
                Vector surfaceNormal = Vector((intersectionPoint.x - closestSphere.center.x) / closestSphere.radius,
                    (intersectionPoint.y - closestSphere.center.y) / closestSphere.radius,
                    (intersectionPoint.z - closestSphere.center.z) / closestSphere.radius);

                // Write color to renderFile 
                renderFile << closestSphere.calculateColor(surfaceNormal, directionToLight, ambientLighting, lightColor, Vector(0, 0, -1)).toString() << endl; // FIXME: What is the right "view" vector? 
            }
        }
    }

    renderFile.close();
}

float computeIntersectionDistance(const Sphere& sphere, const Ray& ray) {
    Vector OC = sphere.center - ray.origin;
    // 1 - Determine if the ray origin is inside the sphere 
    bool insideSphere;
    if (OC.getLength() < sphere.radius) {
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
    float tCH2 = sphere.radius * sphere.radius - OC.getLength() * OC.getLength() + tCA * tCA;

    // 5 - If tCH2 < 0, the ray does not intersect the sphere 
    if (tCH2 < 0) {
        return FLT_MAX;
    }
    // 6 - Find t: intersection distance 
    float t;

    if (insideSphere) {
        t = tCA - sqrt(tCH2);
    }
    else {
        t = tCA + sqrt(tCH2);
    }
    return t;
}

