#define _USE_MATH_DEFINES
#include<cmath>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<iostream>
using namespace std;
using namespace Eigen;

int main()
{
    Vector3f point(2.0, 1.0, 1.0);
    float rad = 45.0 * M_PI / 180.0 ;
    Vector2f translateV(1, 2);
    Matrix3f rotationMatrix;
    rotationMatrix<<cos(rad), -sin(rad), 0,
                    sin(rad), cos(rad), 0,
                    0, 0, 1;
    Matrix3f translateMatrix;
    translateMatrix <<1, 0, translateV.x(),
                        0, 1, translateV.y(),
                        0, 0, 1;
    point = translateMatrix * rotationMatrix * point;
    cout << "Transformed Point:\n";
    cout << point.head(2) << endl;
    return 0;
}