#ifndef Vector3_h
#define Vector3_h

#include <math.h>
#include "Matrix.h"
class Vector3{
public:
    float x_position;
    float y_position;
    float z_position;
public:
    Vector3()
    {
        x_position = 0.0f;
        y_position = 0.0f;
        z_position = 0.0f;
    }
    
    Vector3(float x, float y, float z)
    {
        x_position = x;
        y_position = y;
        z_position = z;
    }
    
    Vector3 operator+(Vector3 ref)
    {
        return Vector3(x_position+ref.x_position,y_position+ref.y_position,z_position+ref.z_position);
    }
    
    Vector3 operator-(Vector3 ref)
    {
        return Vector3(x_position-ref.x_position,y_position-ref.y_position,z_position-ref.z_position);
    }
    
    Vector3 operator*(float scalar)
    {
        return Vector3(x_position*scalar,y_position*scalar,z_position*scalar);
    }
    
    Vector3 operator/(float scalar)
    {
        float temp = 1/scalar;
        return Vector3(x_position*temp,y_position*temp,z_position*temp);
    }
    
    Vector3 operator+=(Vector3 ref)
    {
        x_position += ref.x_position;
        y_position += ref.y_position;
        z_position += ref.z_position;
        return *this;
    }
    
    Vector3 operator*(Matrix ref)
    {
        float vec4[4] = { x_position, y_position, z_position, 1};
        float result[4] = {0,0,0,0};
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                result[i] += vec4[j] * ref.matrix[i][j];
            }
        }
        
        return Vector3( result[0]/result[3], result[1]/result[3], result[2]/result[3]);
    }
    const float operator[]( int index ) {
        assert( index >= 0 && index < 3 );
        return (&x_position)[ index ];
    }
    const float* toArray() const
    {
        return &x_position;
    }
    float dot(Vector3 ref)
    {
        return x_position*ref.x_position+y_position*ref.y_position+z_position*ref.z_position;
    }
    
    Vector3 cross(Vector3 ref)
    {
        return Vector3(
                       y_position*ref.z_position-ref.y_position*z_position,
                       z_position*ref.x_position-ref.z_position*x_position,
                       x_position*ref.y_position-ref.x_position*y_position);
    }
    
    Vector3 normalize()
    {
        float magnitude = sqrtf(x_position*x_position + y_position*y_position +z_position*z_position );
        return Vector3(x_position/magnitude, y_position/magnitude, z_position/magnitude);
    }

};

#endif /* Vector3_h */
