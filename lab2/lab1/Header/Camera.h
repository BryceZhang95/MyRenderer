#ifndef Camera_h
#define Camera_h

#include "Math/Vector3.h"
#include "Math/Matrix.h"
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

class Camera{
private:
    Vector3 position;
    Vector3 up = {0.0 , 1.0, 0.0};
public:
    Matrix view_matrix;
    Matrix projection_matrix;
    
    Camera(float position_x, float position_y, float position_z)
    {
        this->position.x_position = position_x;
        this->position.y_position = position_y;
        this->position.z_position = position_z;
    }
    void setCameraPosition(Vector3 position)
    {
        this->position = position;
    }
    
    void setUpVector(Vector3 up)
    {
        this->up = up;
    }
    void setLookAt(float x, float y,float z)
    {
        Vector3 object_position(x,y,z);
        Vector3 N = position - object_position;
        N = N.normalize();
        Vector3 U = up.cross(N);
        U = U.normalize();
        Vector3 V = N.cross(U);
        V = V.normalize();
        
        view_matrix.matrix[0][0]= U.x_position;
        view_matrix.matrix[0][1]= U.y_position;
        view_matrix.matrix[0][2]= U.z_position;
        view_matrix.matrix[0][3]= -(U.dot(position));
        view_matrix.matrix[1][0]= V.x_position;
        view_matrix.matrix[1][1]= V.y_position;
        view_matrix.matrix[1][2]= V.z_position;
        view_matrix.matrix[1][3]= -(V.dot(position));
        view_matrix.matrix[2][0]= N.x_position;
        view_matrix.matrix[2][1]= N.y_position;
        view_matrix.matrix[2][2]= N.z_position;
        view_matrix.matrix[2][3]= -(N.dot(position));
        
    }
    
    void perspective(float theta, float aspect, float dnear, float dfar)
    {
        
        projection_matrix.matrix[0][0]=2.0/tan(theta/2.0)/aspect;
        projection_matrix.matrix[1][1]=2.0/tan(theta/2.0);
        projection_matrix.matrix[2][2]=(dnear+dfar)/(dnear-dfar);
        projection_matrix.matrix[2][3]=2.0*dnear*dfar/(dfar-dnear);
        projection_matrix.matrix[3][2]=-1.0;
        projection_matrix.matrix[3][3]=0.0;
       
    }
    
    
};

#endif /* Camera_h */
