//
//  main.cpp
//  lab1
//
//  Created by 张芹菜 on 2019/1/28.
//  Copyright © 2019 张芹菜. All rights reserved.
//

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream>

#include "Header/Math/Vector3.h"
#include "Header/Math/Matrix.h"
#include "Header/Math/Graphic.h"
#include "Header/Illumination.h"
#include "Header/Camera.h"
#include "Header/Model.h"
#include "Header/Math/Transformation.h"
#include "Header/Parameter.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 800
#define WIN_AXIS_X 100
#define WIN_AXIS_Y 100
#define PIXELX 800
#define PIXELY 800
#define PIXELZ 800


using namespace std;

int g_frameIndex;    //frame index
Model model[3];
Camera camera(0.0, 6.0, 25.0);


void initFrameBuffer()
{
    for(int i=0;i<PIXELX;i++) {
        for (int j=0;j<PIXELY;j++) {
            framebuffer[i][j][0] = 0.2;
            framebuffer[i][j][1] = 0.3;
            framebuffer[i][j][2] = 0.3;
            zbuffer[i][j] = INT_MIN;
        }
    }
}

void clearZbuffer()
{
    for(int i=0;i<PIXELX;i++) {
        for (int j=0;j<PIXELY;j++) {
            zbuffer[i][j] = INT_MIN;
        }
    }
}


void init()
{
    camera.setLookAt(0.0, 0.0, 0.0);
    camera.perspective(20.0f, (GLfloat)WIN_WIDTH/(GLfloat)WIN_HEIGHT, 0.01f, 100.0f);
    // load model from file
    model[0].loadModel("/Users/aaa/Desktop/Computer Graphics 2/lab2/lab1/Assets/cow.d.txt");
    model[1].loadModel("/Users/aaa/Desktop/Computer Graphics 2/lab2/lab1/Assets/lbox.d.txt");
    model[2].loadModel("/Users/aaa/Desktop/Computer Graphics 2/lab2/lab1/Assets/soccer.d.txt");
    //model.model_matrix = Transformation::translate(model.model_matrix, {0.0,-6.0,0.0});
    //model.model_matrix = Transformation::scale(model.model_matrix, {0.8,0.8,0.8});
    //model.model_matrix = Transformation::rotate(model.model_matrix, 180.0, {1.0,0.0,0.0});
    //model.model_matrix = Transformation::rotate(model.model_matrix, 180.0, {1.0,0.0,0.0});
    
    //init_buffer();
    initFrameBuffer();
    //model[0].model_matrix = Transformation::rotate(model[0].model_matrix, 7.5, {0.0,1.0,0.0});
    //model[0].model_matrix = Transformation::scale(model[0].model_matrix, {0.6,0.6,0.6});
    model[1].model_matrix = Transformation::scale(model[1].model_matrix, {3.5,3.5,3.5});
    model[1].model_matrix = Transformation::translate(model[1].model_matrix, {0,-1.1,0});
    model[2].model_matrix = Transformation::translate(model[2].model_matrix, {0,5,0});
    //Matrix result_matrix = camera.projection_matrix * camera.view_matrix * model.model_matrix;
//    for (int i=0; i<4; i++) {
//        for (int j=0; j<4; j++) {
//            cout<<result_matrix.matrix[i][j]<<", ";
//        }
//        cout<<endl;
//    }
//    cout<<endl<<endl;
//
//    for (int i=0; i<4; i++) {
//        for (int j=0; j<4; j++) {
//            cout<<camera.projection_matrix.matrix[i][j]<<", ";
//        }
//        cout<<endl;
//    }
//    cout<<endl<<endl;
//
//    for (int i=0; i<4; i++) {
//        for (int j=0; j<4; j++) {
//            cout<<camera.view_matrix.matrix[i][j]<<", ";
//        }
//        cout<<endl;
//    }
//    cout<<endl;
}
void Draw()
{
    // clear buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //glEnable(GL_BLEND);
    //clean_buffer();
    // projection matrix
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity();
//
//    // modelview matrix
//    glMatrixMode( GL_MODELVIEW );
//    glLoadIdentity();
  
    // calculate the result of model, view and projection matrix
    Matrix result_matrix_0 = camera.projection_matrix * camera.view_matrix * model[0].model_matrix;
    Matrix result_matrix_1 = camera.projection_matrix * camera.view_matrix * model[1].model_matrix;
    Matrix result_matrix_2 = camera.projection_matrix * camera.view_matrix * model[2].model_matrix;
    model[0].backFaceRemove(result_matrix_0);
    model[0].computeNormal();
    model[0].drawGouraud(result_matrix_0, 1);
    //model[0].drawPhong(result_matrix_0, 1);
    //model[0].scanConversion(result_matrix_0,1);
    
    model[1].backFaceRemove(result_matrix_1);
    model[1].scanConversion(result_matrix_1,3);
//    model[2].backFaceRemove(result_matrix_2);
   model[2].scanConversion(result_matrix_2,2);
//    for (int i=0; i<model[1].faces_number; i++) {
//        printf("%d\n",model[1].faces[i].id);
//    }
    //DrawImage();
//    for (int i=0; i<800; i++) {
//        for (int j=0; j<800; j++) {
//            printf(">%f %f %f\n",framebuffer[i][j][0],framebuffer[i][j][1],framebuffer[i][j][2]);
//        }
//    }
    // draw pictures
    glDrawPixels(PIXELX,PIXELY,GL_RGB,GL_FLOAT,framebuffer);


    // flush
    glFlush();
    
    // swap buffers
    glutSwapBuffers();

}



void update( void ) {
    // do something before rendering...
    // rotate the cow model
    // translate the soccer model
    model[0].model_matrix = Transformation::rotate(model[0].model_matrix, 0.3, {0.0,1.0,0.0});
    model[1].model_matrix = Transformation::rotate(model[1].model_matrix, 0.3, {0.0,1.0,0.0});
    //model[2].model_matrix = Transformation::rotate(model[2].model_matrix, 0.3,{0,0,1});
//    model[2].model_matrix = Transformation::translate(model[2].model_matrix, {0,-0.15,0});
    
//    model[1].model_matrix = Transformation::translate(model[1].model_matrix, {0,-0.15,0});
    //model[1].model_matrix = Transformation::rotate(model[1].model_matrix, 0.5, {1.0,0.0,0.0});
//     model[1].model_matrix = Transformation::translate(model[1].model_matrix, {0,0.8,0});
    // make sure the frame buffer has been cleared before drawing
    initFrameBuffer();
    Draw();
    
    
}

void timer( int value ) {
    // increase frame index
    g_frameIndex++;
    
    update();

    // render
    glutPostRedisplay();
    
    // reset timer
    glutTimerFunc( 1, timer, 0 );
}
int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH|GLUT_MULTISAMPLE);
    glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );
    glutInitWindowPosition(WIN_AXIS_X, WIN_AXIS_Y);
    glutCreateWindow(argv[0]);
    
    init();
    glutDisplayFunc(Draw);
    
    //glutReshapeFunc( reshape );
    glutTimerFunc( 1, timer, 0 );
    
    // main loop
    glutMainLoop();
    
    //close();

   
    
    
    return 0;
}
