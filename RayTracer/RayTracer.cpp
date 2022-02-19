#include <iostream>
#include <fstream>
#include "Vector.h"

using namespace std;

int main() {
    ofstream renderFile("renderFile.ppm");

    int nx = 200;
    int ny = 100;

    renderFile << "P3" << endl << nx << " " << ny << endl << "255" << endl;
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            renderFile << ir << " " << ig << " " << ib << endl;
        }
    }

    Vector v1 = Vector(1, 1, 1);
    Vector v2 = Vector(0, 1, 0);
    float s = 2;

    v1 = v1 * s;
    cout << v1 << endl;

    renderFile.close();
}

