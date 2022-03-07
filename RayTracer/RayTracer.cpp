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

Vector traceRay(const Ray& ray, const vector<Renderable*>& objects);

int main() {
    ofstream renderFile("renderFile.ppm");

    const float pi = 3.14159265358979323846;

    // Render objects
    vector<Renderable*> objects;

    // SCENE 1
    Sphere s1 = Sphere(Vector(0, 0.3, -1.0), 0.25, Vector(255, 255, 255), Vector(1, 1, 1), 0.8, 0.1, 0.3, 4.0, 1.0);

    vector<Vector> verts;
    verts.push_back(Vector(0.0, -0.7, -0.5));
    verts.push_back(Vector(1.0, 0.4, -1.0));
    verts.push_back(Vector(0.0, -0.7, -1.5));
    Polygon t1 = Polygon(verts, 0.9, 1.0, 0.1, Vector(0.0, 0.0, 255.0), Vector(1.0, 1.0, 1.0), 4.0, 0.0);

    verts.clear();
    verts.push_back(Vector(0.0, -0.7, -0.5));
    verts.push_back(Vector(0.0, -0.7, -1.5));
    verts.push_back(Vector(-1.0, 0.4, -1.0));
    Polygon t2 = Polygon(verts, 0.9, 1.0, 0.1, Vector(255, 255, 0), Vector(1, 1, 1), 4.0, 0.0);

    
    /*objects.push_back(&s1);
    objects.push_back(&t1);
    objects.push_back(&t2);*/

    // SCENE 2
    Sphere s2 = Sphere(Vector(0.5, 0.0, -0.15), 0.05, Vector(255, 255, 255), Vector(1, 1, 1), 0.8, 0.1, 0.3, 4.0, 0.0);
    Sphere s3 = Sphere(Vector(0.3, 0.0, -0.1), 0.08, Vector(255, 0, 0), Vector(0.5, 1.0, 0.5), 0.8, 0.8, 0.1, 32.0, 0.0);
    Sphere s4 = Sphere(Vector(-0.6, 0.0, 0.0), 0.3, Vector(0.0, 255, 0.0), Vector(0.5, 1.0, 0.5), 0.7, 0.6, 0.1, 64.0, 0.0);
    Sphere s5 = Sphere(Vector(0.1, -0.66, 0.25), 0.3, Vector(191, 191, 191), Vector(1, 1, 1), 0.0, 0.1, 0.1, 10.0, 0.9);

    verts.clear();
    verts.push_back(Vector(0.3, -0.3, -0.4));
    verts.push_back(Vector(0.0, 0.3, -0.1));
    verts.push_back(Vector(-0.3, -0.3, 0.2));
    Polygon t3 = Polygon(verts, 0.9, 0.9, 0.1, Vector(0, 0, 255), Vector(1, 1, 1), 32.0, 0.0);

    verts.clear();
    verts.push_back(Vector(-0.2, 0.1, 0.1));
    verts.push_back(Vector(-0.2, -0.5, 0.2));
    verts.push_back(Vector(-0.2, 0.1, -0.3));
    Polygon t4 = Polygon(verts, 0.9, 0.5, 0.1, Vector(255, 255, 0), Vector(1, 1, 1), 4, 0);

    //objects.push_back(&s2);
    objects.push_back(&s3);
    objects.push_back(&s4);
    objects.push_back(&s5);
    objects.push_back(&t3);
    objects.push_back(&t4);


    // Lighting 
    Vector directionToLight = Vector(1, 0, 0);
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
    int dimensionX = 300;
    int dimensionY = dimensionX;


    // -----------------------------------------------------Testing-----------------------------------------------------

    vector<Vector> verts2;
    verts2.push_back(Vector(-1, -1, -2));
    verts2.push_back(Vector(0, 1, -2));
    verts2.push_back(Vector(1, -1, -2));
    
    Polygon p1 = Polygon(verts2, 1, 1, 1, Vector(1, 1, 1), Vector(1, 1, 1), 1, 1);

    // objects.push_back(&p1);

    // -----------------------------------------------------Testing-----------------------------------------------------

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

                // Calculate shadow rays 
                bool isInShadow = false;
                Vector shadowRayOrigin = intersectionPoint + directionToLight * 0.001;
                Ray shadowRay = Ray(shadowRayOrigin, directionToLight);
                for (unsigned int k = 0; k < objects.size(); k++) {
                    float temp = objects[k]->calculateIntersectionDistance(shadowRay);
                    if (temp < FLT_MAX) isInShadow = true;
                }

                // Calculate reflection rays 
                if (closestObject->kGls > 0) {
                    Vector reflectionDirection = 2 * surfaceNormal * surfaceNormal.dot(directionToLight) - directionToLight;
                    Vector reflectionOrigin = intersectionPoint + reflectionDirection * 0.001;
                    Ray reflectionRay = Ray(reflectionOrigin, reflectionDirection);

                    Renderable* closestObject = NULL;
                    float objDist = FLT_MAX;
                    for (unsigned int k = 0; k < objects.size(); k++) {
                        float temp = objects[k]->calculateIntersectionDistance(ray);
                        if (temp < objDist) {
                            closestObject = objects[k];
                            objDist = temp;
                        }
                    }

                    if (objDist == FLT_MAX) {
                        // Background color 
                    }
                    else if (closestObject != NULL) {
                        // Calculate that object's color 
                        // 7 - Calculate intersection point 
                        Vector intersectionPoint = Vector(ray.origin.x + ray.direction.x * objDist,
                            ray.origin.y + ray.direction.y * objDist,
                            ray.origin.z + ray.direction.z * objDist);

                        // 8 - Calculate surface normal 
                        Vector surfaceNormal = closestObject->calculateSurfaceNormal(intersectionPoint);

                        // Calculate shadow rays 
                        bool isInShadow = false;
                        Vector shadowRayOrigin = intersectionPoint + directionToLight * 0.001;
                        Ray shadowRay = Ray(shadowRayOrigin, directionToLight);
                        for (unsigned int k = 0; k < objects.size(); k++) {
                            float temp = objects[k]->calculateIntersectionDistance(shadowRay);
                            if (temp < FLT_MAX) isInShadow = true;
                        }
                    }
                }

                // Write color to renderFile 
                renderFile << closestObject->calculateColor(surfaceNormal, directionToLight, ambientLighting, lightColor, ray.direction, isInShadow).toString() << endl;
            }
        }
    }

    renderFile.close();
}

Vector traceRay(const Ray& ray, const vector<Renderable*>& objects) {
    return Vector(0, 0, 0);
}
