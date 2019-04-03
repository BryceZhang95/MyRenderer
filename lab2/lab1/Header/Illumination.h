//
//  Illumination.h
//  lab1
//
//  Created by 张芹菜 on 2019/3/23.
//  Copyright © 2019 张芹菜. All rights reserved.
//

#ifndef Illumination_h
#define Illumination_h
#include <cmath>
#include "Math/Vector3.h"

double ambient_term(double ka, double I_ambient)
{
    return ka*I_ambient;
}

double diffuse_term(double kd, double I_light, Vector3 normal, Vector3 light, int power)
{
//    double gradient = pow((normal.dot(light)),power);
//
//    return gradient > 0 ? gradient * kd * I_light : 0;
    light = light.normalize();
    //printf(">%f",pow((normal.dot(light)),power));
    double gradient = pow((normal.dot(light)),power);
    return gradient > 0 ? gradient * kd * I_light : 0;
}

double specular_term(double ks, double I_light, Vector3 normal,  Vector3 light,  Vector3 view, const int n)
{
    Vector3 h = light + view;
    //printf(">%f %f %f\n",h.x_position,h.y_position,h.z_position);
    //printf("<%f %f %f\n",normal.x_position,normal.y_position,normal.z_position);
    h = h.normalize();
    
    return ks * I_light * pow((normal.dot(h)),n);
}

double Phong_illumination(double ka, double kd, double ks, double I_ambient, double I_light,int power, const int n, Vector3 normal, Vector3 light, Vector3 view)
{
    return ambient_term(ka, I_ambient) + diffuse_term(kd, I_light, normal, light, power) + specular_term(ks, I_light, normal, light, view, n);
}

#endif /* Illumination_h */
