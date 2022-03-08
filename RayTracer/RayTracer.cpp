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

Vector traceRay(const Ray& ray, const vector<Renderable*>& objects, Vector directionToLight, Vector ambientLighting, Vector lightColor, Vector backgroundColor, int reflectionBounces);

int main() {
    ofstream renderFile("renderFile.ppm");

    const float pi = 3.14159265358979323846;

    // Render objects
    vector<Renderable*> objects;

    // SCENE 1
    //Sphere s1 = Sphere(Vector(0, 0.3, -1.0), 0.25, Vector(191, 191, 191), Vector(1, 1, 1), 0.8, 0.1, 0.3, 10.0, 1.0);
    Sphere s1 = Sphere(Vector(0.0, 0.3, -1.0), 0.25, Vector(191, 191, 191), Vector(1, 1, 1), 0.0, 0.1, 0.1, 10.0, 0.9);
    //Sphere s1 = Sphere(Vector(0.0, 0.3, -1.0), 0.25, Vector(191, 191, 191), Vector(1, 1, 1), 0.0, 0.1, 0.1, 10.0, 0.0);

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

    // SCENE 2
    Sphere s2 = Sphere(Vector(0.5, 0.0, -0.15), 0.05, Vector(255, 255, 255), Vector(1, 1, 1), 0.8, 0.1, 0.3, 4.0, 0.0);
    Sphere s3 = Sphere(Vector(0.3, 0.0, -0.1), 0.08, Vector(255, 0, 0), Vector(0.5, 1.0, 0.5), 0.8, 0.8, 0.1, 32.0, 0.0);
    Sphere s4 = Sphere(Vector(-0.6, 0.0, 0.0), 0.3, Vector(0.0, 255, 0.0), Vector(0.5, 1.0, 0.5), 0.7, 0.6, 0.1, 64.0, 0.0);
    Sphere s5 = Sphere(Vector(0.1, -0.55, 0.25), 0.3, Vector(191, 191, 191), Vector(1, 1, 1), 0.0, 0.1, 0.1, 10.0, 0.9);

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

    // SCENE 3
    verts.clear();
    verts.push_back(Vector(0.0, -1.7, -0.5));
    verts.push_back(Vector(1.0, 0.4, -1.0));
    verts.push_back(Vector(0.0, -1.7, -1.5));
    Polygon t5 = Polygon(verts, 0.9, 1.0, 0.1, Vector(0.0, 0.0, 255.0), Vector(1.0, 1.0, 1.0), 4.0, 0.0);

    verts.clear();
    verts.push_back(Vector(-0.4, 0.1, 0.1));
    verts.push_back(Vector(-0.3, 0.1, -0.3));
    verts.push_back(Vector(-0.4, -0.5, 0.2));
    Polygon t6 = Polygon(verts, 0.9, 0.5, 0.1, Vector(255, 0, 255), Vector(1, 1, 1), 4, 0);

    Sphere s6 = Sphere(Vector(1, 1, 1), 0.5, Vector(255, 0, 0), Vector(0.5, 1.0, 0.5), 0.8, 0.8, 0.1, 32.0, 0.0);

    Sphere s7 = Sphere(Vector(-0.1, 0.1, 0.1), 0.15, Vector(191, 0, 0), Vector(1, 1, 1), 0.0, 0.1, 0.1, 10.0, 0.9);
    Sphere s8 = Sphere(Vector(0.2, -0.1, 0.3), 0.15, Vector(0, 191, 0), Vector(1, 1, 1), 0.5, 0.1, 0.1, 10.0, 0.9);

    Sphere s9 = Sphere(Vector(-0.05, -0.1, 0.24), 0.07, Vector(150, 150, 150), Vector(0.5, 1.0, 0.5), 0.8, 0.8, 0.1, 32.0, 0.0);

    // Lighting 
    Vector directionToLight = Vector(0, 1, 0);
    Vector lightColor = Vector(1, 1, 1);
    Vector ambientLighting = Vector(0.1, 0.1, 0.1);

    // Colors 
    Vector backgroundColor = Vector(0.2, 0.2, 0.2) * 255;
    Vector colorWhite = Vector(255, 255, 255);
    Vector colorRed = Vector(255, 0, 0);

    // Camera setup 
    Vector cameraOrigin = Vector(0, 0, 1);
    float FOV = 60.0;
    FOV *= 180 / pi;
    float imagePlaneX = abs(tan(FOV));
    float imagePlaneY = abs(tan(FOV));
    float imagePlaneZ = 4;

    // Define output file resolution
    int dimensionX = 400;
    int dimensionY = dimensionX;

    bool scene1 = false;
    bool scene2 = false;
    bool testScene = true;

    if (scene1) {
        objects.push_back(&s1);
        objects.push_back(&t1);
        objects.push_back(&t2);

        directionToLight = Vector(0, 1, 0);
    }
    else if (scene2) {
        // Scene 2 objects 
        objects.push_back(&s2);
        objects.push_back(&s3);
        objects.push_back(&s4);
        objects.push_back(&s5);
        objects.push_back(&t3);
        objects.push_back(&t4);

        directionToLight = Vector(1, 0, 0);
    }
    else if (testScene) {
        objects.push_back(&t5);
        objects.push_back(&t6);
        objects.push_back(&s6);
        objects.push_back(&s7);
        objects.push_back(&s8);
        objects.push_back(&s9);
        directionToLight = Vector(-1, 0, 0);
    }

    renderFile << "P3" << endl << dimensionX << " " << dimensionY << endl << "255" << endl;
    for (int j = dimensionY - 1; j >= 0; j--) {
        for (int i = 0; i < dimensionX; i++) {
            float x = (float(i) / float(dimensionX) * imagePlaneX) - (imagePlaneX / 2);
            float y = (float(j) / float(dimensionY) * imagePlaneY) - (imagePlaneY / 2);

            Vector direction = Vector(x, y, -1); 
            direction.unitVector();

            Ray ray = Ray(cameraOrigin, direction, 1); // Ray from camera through each pixel 

            Vector color = traceRay(ray, objects, directionToLight, ambientLighting, lightColor, backgroundColor, 3);

            // Write color to renderFile 
            renderFile << color.toString() << endl;
        }
    }

    renderFile.close();
}

Vector traceRay(const Ray& ray, const vector<Renderable*>& objects, Vector directionToLight, Vector ambientLighting, Vector lightColor, Vector backgroundColor, int reflectionBounces) {
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
        return backgroundColor;
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
        Vector shadowRayOrigin = intersectionPoint + surfaceNormal * 0.001;
        Ray shadowRay = Ray(shadowRayOrigin, directionToLight);
        for (unsigned int k = 0; k < objects.size(); k++) {
            float temp = objects[k]->calculateIntersectionDistance(shadowRay);
            if (temp < FLT_MAX) isInShadow = true;
        }

        // Calculate reflection rays 
        Vector reflectedColor = Vector(0, 0, 0);
        float refl = closestObject->getRefl(); 
        if (refl > 0 && reflectionBounces > 0) {
            reflectionBounces--;
            Vector reflectionRayDirection = -2 * surfaceNormal * surfaceNormal.dot(ray.direction) + ray.direction;
            Vector reflectionRayOrigin = intersectionPoint + surfaceNormal * 0.001;
            Ray reflectionRay = Ray(reflectionRayOrigin, reflectionRayDirection);

            reflectedColor = traceRay(reflectionRay, objects, directionToLight, ambientLighting, lightColor, backgroundColor, reflectionBounces) * refl;
        }

        // Calculate color 
        return closestObject->calculateColor(surfaceNormal, directionToLight, ambientLighting, lightColor, ray.direction, isInShadow) + reflectedColor;
    }
}
