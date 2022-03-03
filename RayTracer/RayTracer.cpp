#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>

#include "Vector.h"
#include "Ray.h"
#include "Renderable.h"
#include "Sphere.h"
#include "Polygon.h"

using namespace std;

float computeIntersectionDistance(const Renderable& object, const Ray& ray);

int main() {
    ofstream renderFile("renderFile.ppm");

    const float pi = 3.14159265358979323846;

    // Render objects
    Sphere s1 = Sphere(Vector(-0.5, -0.5, -1.0), 0.4, Vector(154, 255, 255), Vector(1, 1, 1), 0.8, 0.1, 0.3, 128.0, 0.9);
    vector<Vector> verts;
    verts.push_back(Vector(0.0, -0.7, -0.5));
    verts.push_back(Vector(1.0, 0.4, -1.0));
    verts.push_back(Vector(0.0, -0.7, -1.5));
    Polygon t1 = Polygon(verts, 0.9, 1.0, 0.1, Vector(0.0, 0.0, 1.0), Vector(1.0, 1.0, 1.0), 4.0, 0.0);
    vector<Renderable> objects;
    objects.push_back(s1);

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
    float imagePlaneX = abs(tan(FOV));
    float imagePlaneY = abs(tan(FOV));
    float imagePlaneZ = 4;

    // Define output file resolution
    int dimensionX = 400;
    int dimensionY = 400;

    renderFile << "P3" << endl << dimensionX << " " << dimensionY << endl << "255" << endl;
    for (int j = dimensionY - 1; j >= 0; j--) {
        for (int i = 0; i < dimensionX; i++) {
            float x = (float(i) / float(dimensionX) * imagePlaneX) - (imagePlaneX / 2);
            float y = (float(j) / float(dimensionY) * imagePlaneY) - (imagePlaneY / 2);

            Vector direction = Vector(x, y, -1); 
            direction.unitVector();

            Ray ray = Ray(cameraOrigin, direction); // Ray from camera through each pixel 

            // Find closest sphere along ray 
            Renderable* closestObject; 
            float objDist = FLT_MAX;
            for (unsigned int k = 0; k < objects.size(); k++) {
                float temp = computeIntersectionDistance(objects[k], ray);
                if (temp < objDist) {
                    closestObject = &objects[k];
                    objDist = temp;
                }
            }

            if (objDist == FLT_MAX) {
                // No spheres were hit 
                renderFile << backgroundColor.toString() << endl;
            }
            else {
                // 7 - Calculate intersection point 
                Vector intersectionPoint = Vector(ray.origin.x + ray.direction.x * objDist,
                    ray.origin.y + ray.direction.y * objDist,
                    ray.origin.z + ray.direction.z * objDist);

                // 8 - Calculate surface normal 
                Vector surfaceNormal = Vector((intersectionPoint.x - closestObject.center.x) / closestObject.radius,
                    (intersectionPoint.y - closestObject.center.y) / closestObject.radius,
                    (intersectionPoint.z - closestObject.center.z) / closestObject.radius);

                // Write color to renderFile 
                renderFile << closestObject.calculateColor(surfaceNormal, directionToLight, ambientLighting, lightColor, ray.direction).toString() << endl;
            }
        }
    }

    renderFile.close();
}

float computeIntersectionDistance(const Renderable& object, const Ray& ray) {
    Vector OC = object.center - ray.origin;
    // 1 - Determine if the ray origin is inside the sphere 
    bool insideSphere;
    if (OC.getLength() < object.radius) {
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
    float tCH2 = object.radius * object.radius - OC.getLength() * OC.getLength() + tCA * tCA;

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

