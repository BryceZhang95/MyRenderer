//
//  Graphic.h
//  lab1
//
//  Created by 张芹菜 on 2019/3/22.
//  Copyright © 2019 张芹菜. All rights reserved.
//

#ifndef Graphic_h
#define Graphic_h
#include "Vector3.h"
struct Faces{
    int id;
    int sides_number;
    int* indices;
    Vector3 normal;
    Faces()
    {
        sides_number = 0;
        indices = NULL;
        //normal = NULL;
        normal = Vector3();
    }
    ~Faces() {
        if ( indices ) {
            delete[] indices;
            indices = NULL;
        }
        sides_number = 0;
    }
};

struct Point{
    Vector3 vert;
    Vector3 normal;
    double intensity;
};

struct Edge
{
public:
    float slopeRecip;
    float maxY;
    float minX;
    float currentZ;
    float currentI;
    float incZ;
    float incI;
    Vector3 currentN;
    Vector3 incN;
    
    bool operator < ( const Edge &e )
    {
        if ( minX == e.minX )
        {
            return slopeRecip < e.slopeRecip;
        }
        else
        {
            return minX < e.minX;
        }
    }
};

struct act_cmp{
    bool operator ()(Edge& a, Edge& b) {
        return a.minX < b.minX;
    }
};


#endif /* Graphic_h */
