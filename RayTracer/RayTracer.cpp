#include <iostream>
#include <fstream>

#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"

using namespace std;

int main() {
    ofstream renderFile("renderFile.ppm");

    Vector cameraOrigin = Vector(0, 0, 5);

    Sphere s1 = Sphere(Vector(0, 0, 0), 0.4);

    // Colors 
    Vector colorWhite = Vector(255, 255, 255);
    Vector colorRed = Vector(255, 0, 0);

    int imagePlaneX = 4;
    int imagePlaneY = 2;
    int imagePlaneZ = 4;

    // Output file dimensions 
    int dimensionX = 50;
    int dimensionY = 50;

    renderFile << "P3" << endl << dimensionX << " " << dimensionY << endl << "255" << endl;
    for (int j = dimensionY - 1; j >= 0; j--) {
        for (int i = 0; i < dimensionX; i++) {
            /*float r = float(i) / float(dimensionX);
            float g = float(j) / float(dimensionY);
            float b = 0.2;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);*/

            float x = ((i / dimensionX) * imagePlaneX) - (imagePlaneX/2);
            float y = ((j / dimensionY) * imagePlaneY) - (imagePlaneY/2);

            Vector direction = Vector(x, y, 0); 
            direction /= direction.getLength();

            Ray ray = Ray(cameraOrigin, direction);

            if (s1.isOnSphere(ray)) {
                // Output red 
                renderFile << colorRed.toString() << endl;
            }
            else {
                // Output white 
                renderFile << colorWhite.toString() << endl;
            }

             //renderFile << ir << " " << ig << " " << ib << endl;
            //renderFile << colorWhite.toString() << endl;
        }
    }

    renderFile.close();
}

