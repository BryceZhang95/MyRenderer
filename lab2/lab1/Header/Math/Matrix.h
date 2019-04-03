#ifndef Matrix_h
#define Matrix_h
#include "Vector3.h"
class Matrix{

public:
    float matrix[4][4];
    
    Matrix()
    {
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                if (i == j) {
                    matrix[i][j] = 1.0;
                } else {
                    matrix[i][j] = 0.0;
                }
            }
        }
    }
    Matrix operator+(Matrix ref)
    {
        Matrix result;
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                result.matrix[i][j] = matrix[i][j] + ref.matrix[i][j];
            }
        }
        return result;
    }
    
    Matrix operator+=(Matrix ref)
    {
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                 matrix[i][j] += ref.matrix[i][j];
            }
        }
        return *this;
    }
    
    Matrix operator-(Matrix ref)
    {
        Matrix result;
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                result.matrix[i][j] = matrix[i][j] - ref.matrix[i][j];
            }
        }
        return result;
    }
    
    Matrix operator-=(Matrix ref)
    {
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                matrix[i][j] -= ref.matrix[i][j];
            }
        }
        return *this;
    }
    
    Matrix operator*(Matrix ref)
    {
        
        Matrix result;
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                result.matrix[i][j] = 0;
            }
        }
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                for (int k=0; k<4; k++) {
                    result.matrix[i][j] += (matrix[i][k] * ref.matrix[k][j]);
                }
            }
        }
        return result;
    }
    
    
    void setMatrix(float ref[4][4])
    {
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                matrix[i][j] = ref[i][j];
            }
        }
    }
    
};

#endif /* Matrix_h */
