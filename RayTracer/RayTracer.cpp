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

int main() {
    ofstream renderFile("renderFile.ppm");

    const float pi = 3.14159265358979323846;

    // Render objects
    Sphere s1 = Sphere(Vector(0, 0, 0), 0.15, Vector(255, 255, 255), Vector(1, 1, 1), 0.8, 0.1, 0.3, 4.0, 1.0);

    vector<Vector> verts;
    verts.push_back(Vector(0.0, -0.7, -0.5));
    verts.push_back(Vector(1.0, 0.4, -1.0));
    verts.push_back(Vector(0.0, -0.7, -1.5));
    Polygon t1 = Polygon(verts, 0.9, 1.0, 0.1, Vector(0.0, 0.0, 1.0), Vector(1.0, 1.0, 1.0), 4.0, 0.0);

    vector<Renderable*> objects;
    objects.push_back(&s1);
    //objects.push_back(t1);

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
    int dimensionX = 200;
    int dimensionY = 200;

    renderFile << "P3" << endl << dimensionX << " " << dimensionY << endl << "255" << endl;
    for (int j = dimensionY - 1; j >= 0; j--) {
        for (int i = 0; i < dimensionX; i++) {
            float x = (float(i) / float(dimensionX) * imagePlaneX) - (imagePlaneX / 2);
            float y = (float(j) / float(dimensionY) * imagePlaneY) - (imagePlaneY / 2);

            Vector direction = Vector(x, y, -1); 
            direction.unitVector();

            Ray ray = Ray(cameraOrigin, direction); // Ray from camera through each pixel 

            // Find closest object along ray 
            Renderable* closestObject = NULL; 
            float objDist = FLT_MAX;
            for (unsigned int k = 0; k < objects.size(); k++) {
                //float temp = computeIntersectionDistance(objects[k], ray);
                float temp = objects[k]->calculateIntersectionDistance(ray);
                if (temp < objDist) {
                    closestObject = objects[k];
                    objDist = temp;
                }
            }

            if (objDist == FLT_MAX) {
                // Nothing was hit 
                renderFile << backgroundColor.toString() << endl;
            }
            else if (closestObject != NULL) {
                // 7 - Calculate intersection point 
                Vector intersectionPoint = Vector(ray.origin.x + ray.direction.x * objDist,
                    ray.origin.y + ray.direction.y * objDist,
                    ray.origin.z + ray.direction.z * objDist);

                // 8 - Calculate surface normal 
                Vector surfaceNormal = closestObject->calculateSurfaceNormal(intersectionPoint);

                // Write color to renderFile 
                renderFile << closestObject->calculateColor(surfaceNormal, directionToLight, ambientLighting, lightColor, ray.direction).toString() << endl;
            }
        }
    }

    renderFile.close();
}
