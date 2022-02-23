#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"

using namespace std;

float computeIntersectionDistance(const Sphere& sphere, const Ray& ray);

int main() {
    ofstream renderFile("renderFile.ppm");

    // Render objects
    Sphere s1 = Sphere(Vector(0.45, 0, -0.15), 0.15, Vector(255, 255, 255), Vector(1, 1, 1), 0.8, 0.1, 0.3, 4.0);
    Sphere s2 = Sphere(Vector(0, 0, -0.1), 0.2, Vector(255, 0, 0), Vector(1, 1, 1), 0.6, 0.3, 0.1, 32.0);
    Sphere s3 = Sphere(Vector(-0.6, 0, 0), 0.3, Vector(0, 255, 0), Vector(0.5, 1.0, 0.5), 0.7, 0.2, 0.1, 64.0);
    Sphere s4 = Sphere(Vector(0.0, -10000.5, 0.0), 10000.0, Vector(0, 0, 255), Vector(1, 1, 1), 0.9, 0.0, 0.1, 16.0);
    vector<Sphere> spheres;
    spheres.push_back(s1);
    spheres.push_back(s2);
    spheres.push_back(s3);
    spheres.push_back(s4);

    // Lighting 
    Vector directionToLight = Vector(1, 1, -1);
    Vector lightColor = Vector(1, 1, 1);
    Vector ambientLighting = Vector(0.1, 0.1, 0.1);

    // Colors 
    Vector backgroundColor = Vector(0.2, 0.2, 0.2) * 255;
    Vector colorWhite = Vector(255, 255, 255);
    Vector colorRed = Vector(255, 0, 0);

    // Camera setup 
    Vector cameraOrigin = Vector(0, 0, 1);
    float FOV = 90.0;
    float imagePlaneX = 1; // FIXME: I need a better way to define the viewplane and file size - properly implement the FOV stuff (look at Martin's messages) 
    float imagePlaneY = 1;
    float imagePlaneZ = 4;

    // Output file dimensions 
    int dimensionX = 400;
    int dimensionY = 400;

    renderFile << "P3" << endl << dimensionX << " " << dimensionY << endl << "255" << endl;
    for (int j = dimensionY - 1; j >= 0; j--) {
        for (int i = 0; i < dimensionX; i++) {
            float x = (float(i) / float(dimensionX) * imagePlaneX) - (imagePlaneX / 2);
            float y = (float(j) / float(dimensionY) * imagePlaneY) - (imagePlaneY / 2);

            Vector direction = Vector(x, y, -1); 
            direction.unitVector();

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
                renderFile << closestSphere.calculateColor(surfaceNormal, directionToLight, ambientLighting, lightColor, ray.direction).toString() << endl; // FIXME: Should I reverse ray.direction? ( *= -1)
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

