//
//  Transformation.h
//  lab1
//
//  Created by 张芹菜 on 2019/2/4.
//  Copyright © 2019 张芹菜. All rights reserved.
//

#ifndef Transformation_h
#define Transformation_h

#include "Vector3.h"
#include "Matrix.h"

#include <math.h>
using namespace std;
class Transformation{
public:
    Matrix static translate(Matrix model, Vector3 vec3)
    {
        Matrix temp;
        temp.matrix[0][3] = vec3.x_position;
        temp.matrix[1][3] = vec3.y_position;
        temp.matrix[2][3] = vec3.z_position;
        model = model*temp;
        return model;
    }
    
    Matrix static scale(Matrix model, Vector3 vec3)
    {
        Matrix temp;
        temp.matrix[0][0] = vec3.x_position;
        temp.matrix[1][1] = vec3.y_position;
        temp.matrix[2][2] = vec3.z_position;
        model = model*temp;
        return model;
    }
    
    Matrix static rotate(Matrix model, float angle, Vector3 vec3)
    {
        Matrix temp;
        temp.matrix[0][0] = cos(angle) + vec3.x_position * vec3.x_position * (1 - cos(angle));
        temp.matrix[0][1] = vec3.x_position * vec3.y_position * (1 - cos(angle)) - vec3.z_position * sin(angle);
        temp.matrix[0][2] = vec3.x_position * vec3.z_position * (1 - cos(angle)) + vec3.y_position * sin(angle);
        temp.matrix[1][0] = vec3.y_position * vec3.x_position * (1 - cos(angle)) + vec3.z_position * sin(angle);
        temp.matrix[1][1] = cos(angle) + vec3.y_position * vec3.y_position * (1 - cos(angle));
        temp.matrix[1][2] = vec3.y_position * vec3.z_position * (1 - cos(angle)) - vec3.x_position * sin(angle);
        temp.matrix[2][0] = vec3.z_position * vec3.x_position * (1 - cos(angle)) - vec3.y_position * sin(angle);
        temp.matrix[2][1] = vec3.z_position * vec3.y_position * (1 - cos(angle)) + vec3.x_position * sin(angle);
        temp.matrix[2][2] = cos(angle) + vec3.z_position * vec3.z_position * (1 - cos(angle));
        model = model*temp;
        return model;
    }
    
};

#endif /* Transformation_h */
