#include <iostream>
#include <fstream>

#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"

using namespace std;

int main() {
    ofstream renderFile("renderFile.ppm");

    Vector cameraOrigin = Vector(0, 0, -5);

    Sphere s1 = Sphere(Vector(0, 0, 0), 0.4);

    // Output file dimensions 
    int dimensionX = 5;
    int dimensionY = 5;

    renderFile << "P3" << endl << dimensionX << " " << dimensionY << endl << "255" << endl;
    for (int j = dimensionY - 1; j >= 0; j--) {
        for (int i = 0; i < dimensionX; i++) {
            float r = float(i) / float(dimensionX);
            float g = float(j) / float(dimensionY);
            float b = 0.2;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            renderFile << ir << " " << ig << " " << ib << endl;

            Vector direction = Vector(i, j, 0);
            cout << direction << endl;
        }
    }

    renderFile.close();

    // Vector testing code 
    Vector v1 = Vector(1, 1, 1);
    Vector v2 = Vector(0, 1, 0);
    float s = 2;

    v1 = v1 * s;
    cout << v1 << endl;

    // Ray testing code 
    Vector origin = Vector(0, 0, 0);
    Vector direction = Vector(1, 1, 0);
    Ray r = Ray(origin, direction);

    cout << r.getPoint(5) << endl;


    // Raytracing pseudocode 

    /*
    for pixel in screen: 
        ray(camera, pixel)
        get color
        write color to ppm file 
    
    */
}

