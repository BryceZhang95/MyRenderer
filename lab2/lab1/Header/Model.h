#ifndef Model_h
#define Model_h
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <math.h>
#include <algorithm>
#include <ctime>
#include <time.h>
#include "Math/Vector3.h"
#include "Math/Matrix.h"
#include "Math/Transformation.h"
#include "Math/Graphic.h"
#include "Parameter.h"
#include "Illumination.h"
using namespace std;

//struct Faces{
//    int id;
//    int sides_number;
//    int* indices;
//    Vector3* normal;
//    Faces()
//    {
//        sides_number = 0;
//        indices = NULL;
//        normal = NULL;
//    }
//};
//
//struct Edge
//{
//public:
//    float slopeRecip;
//    float maxY;
//    float minX;
//    float currentZ;
//    float incZ;
//
//    bool operator < ( const Edge &e )
//    {
//        if ( minX == e.minX )
//        {
//            return slopeRecip < e.slopeRecip;
//        }
//        else
//        {
//            return minX < e.minX;
//        }
//    }
//};
//
//struct act_cmp{
//    bool operator ()(Edge& a, Edge& b) {
//        return a.minX < b.minX;
//    }
//};

inline int to_pixel(float &value) {
    // [-1, 1] is transformed to [0, PIXEL]
    return floor((value+1)*PIXELY/2);
}
vector<vector<Edge> > edgeTable;
vector<Edge> activeEdgeTable;
float framebuffer[PIXELX][PIXELY][3];
float zbuffer[PIXELX][PIXELY];
class Model{
public:
    int vertices_number;
    int faces_number;
    int front_faces_number;
    Point* points;
    Point* current_points;
    //Vector3* normal;
    Faces* faces;
    Faces* front_faces;
    Matrix model_matrix;
    

public:
    Model()
    {
        vertices_number = 0;
        faces_number = 0;
        front_faces_number = 0;
        points = NULL;
        //normal = NULL;
        faces = NULL;
        front_faces = NULL;
        model_matrix = Matrix();
        
    }
    
    bool loadModel(const char *Path)
    {
        if ( (Path==NULL) ) {
            cout<<"fail1"<<endl;
            return false;
        }
        
        FILE *file = fopen(Path, "r" );            // open file
        if ( (file==NULL) ) {
            cout<<"fail2"<<endl;
            return false;
        }
        
        fscanf( file, "%d%d", &vertices_number , &faces_number );    // num of vertices and faces
        points = new Point[vertices_number];
        current_points = new Point[vertices_number];
        faces = new Faces[faces_number];
        for ( int i = 0; i < vertices_number; i++ ) {
            fscanf( file, "%f%f%f", &points[ i ].vert.x_position, &points[ i ].vert.y_position, &points[ i ].vert.z_position );
        }
        front_faces = new Faces[faces_number];
        
        // read indices
        for ( int i = 0; i < faces_number; i++ ) {
            Faces *face = &faces[ i ];
            //face[i].id = i;
            fscanf( file, "%i", &face->sides_number );
            faces[ i ].indices = new int[ face->sides_number ];
            for( int k = 0; k < face->sides_number; k++ ) {
                fscanf( file, "%i", &face->indices[ k ] );
            }
            
        }
       
        
        // close file
        fclose( file );
        
        return true;
    }
    
    void backFaceRemove(Matrix t)
    {
        //glLineWidth(2);
        glBegin( GL_LINES );
        glColor3f( 0.94, 0.5, 0.5 );
        Vector3 a, b ,c;
        front_faces_number = 0;
        for ( int i = 0; i < faces_number; i++ ) {
//            for ( int k = 0; k < faces[ i ].sides_number; k++ ) {
//                int p0=faces[i].indices[k] - 1;
//                int p1=faces[i].indices[(k+1)%faces[i].sides_number] - 1;
//                int p2=faces[i].indices[(k+2)%faces[i].sides_number] - 1;
//                a=vertices[p0]*(t);
//                b=vertices[p1]*(t);
//                c=vertices[p2]*(t);
//                //faces[i].normal = (b-a).cross(c-b);
//                Vector3 normal = (b-a).cross(c-b);
//                normal = normal.normalize();
//                //printf( ">%d %f %f %f\n", p0, vertices[p0].x_position, vertices[p0].y_position, vertices[p0].z_position );
//                //printf( "<%f %f %f\n", a.x_position, a.y_position, a.z_position );
//                //bace face remove
//                if (normal.z_position<0) {
//                    //front_faces[front_faces_number] = faces[i];
//                    glVertex2fv( a.toArray() );
//                    glVertex2fv( b.toArray() );
//                    //front_faces_number++;
//                }
//
//             }
            faces[i].id = i;
            int p0=faces[i].indices[0] - 1;
            int p1=faces[i].indices[(1)%faces[i].sides_number] - 1;
            int p2=faces[i].indices[(2)%faces[i].sides_number] - 1;
            a=points[p0].vert*(t);
            b=points[p1].vert*(t);
            c=points[p2].vert*(t);
//            faces[i].normal = (b-a).cross(c-b);
            Vector3 normal = (b-a).cross(c-b);
            normal = normal.normalize();
            
            // remove back face by deciding the normal of z position
            if (normal.z_position<=0) {
                front_faces[front_faces_number] = faces[i];
                front_faces_number++;
            }
        }
        //glVertex2f( 0, 0 );
        //glVertex2f( 1, 0 );
        glEnd();
  }

    void scanConversion(Matrix t,int colorID)
    {
        for (int i=0; i<vertices_number; i++) {
            // here we get the screen coordinate of each point
            current_points[i].vert = points[i].vert * t;
            //printf( "<%f %f %f\n", current_vertices[i].x_position, current_vertices[i].y_position, current_vertices[i].z_position );
        }
        //int flag = 1;
        // for each face we create an edge table
        for (int i=0; i<front_faces_number; i++)
        {
            //printf(">%f %f %f\n",front_faces[i].normal.x_position,front_faces[i].normal.y_position,front_faces[i].normal.z_position);
            int length = front_faces[i].sides_number;
            float z;
            float dZ = 0;
            //printf("> %d\n",front_faces[i].id);
            edgeTable.clear ( );
            //float random[3];
//            srand(time(NULL));//设置随机数种子，使每次产生的随机序列不同
//            for (int i = 0; i < 3; i++)
//            {
//                random[i] = rand() % (N + 1) / (float)(N + 1);
//            }
            for (int j=0; j<PIXELY; j++)
            {
                // add rows equal to height of image to active edge table
                vector<Edge> row;
                edgeTable.push_back ( row );
            }
            
            for (int k=0; k<length; k++)
            {
                Edge e;
                int p_current = front_faces[i].indices[k]-1;
                int p_next = front_faces[i].indices[(k+1)%length]-1;
                // ignore the situations that the edge is horizontal
                if (to_pixel(current_points[p_current].vert.y_position)==to_pixel(current_points[p_next].vert.y_position)) {
                    continue;
                }
                // initialize the parameter of edge table
                // maxY is the y_position of the upper point
                // minX is the x_position of the lower point
                // slopeRecip is the change of x on y
                // incZ is the change of z on y(pixel)
                e.maxY = max(to_pixel(current_points[p_current].vert.y_position), to_pixel(current_points[p_next].vert.y_position));
                e.slopeRecip = ( current_points[p_current].vert.x_position - current_points[p_next].vert.x_position ) / (float)( current_points[p_current].vert.y_position - current_points[p_next].vert.y_position );
                e.incZ = ( current_points[p_current].vert.z_position - current_points[p_next].vert.z_position ) / (float)( to_pixel(current_points[p_current].vert.y_position) - to_pixel(current_points[p_next].vert.y_position) );
                // judge if the current point is the upper point or the lower point
                if (to_pixel(current_points[p_current].vert.y_position) == e.maxY) {
                    e.minX = to_pixel(current_points[p_next].vert.x_position);
                    e.currentZ = current_points[p_next].vert.z_position;
                    edgeTable[to_pixel(current_points[p_next].vert.y_position)].push_back(e);
                }
                else{
                    e.minX = to_pixel(current_points[p_current].vert.x_position);
                    e.currentZ = current_points[p_current].vert.z_position;
                    edgeTable[to_pixel(current_points[p_current].vert.y_position)].push_back(e);
                }

            }
//            for (int i=0; i<edgeTable.size(); i++) {
//                printf("<%f %f %f\n",edgeTable[i][0].maxY,edgeTable[i][0].minX,edgeTable[i][0].slopeRecip);
//            }
            activeEdgeTable.clear();
            for (int y=0; y<PIXELY; y++)
            {
                // initialize the active edge table
                for (int k=0; k<edgeTable[y].size(); k++)
                {
                    activeEdgeTable.push_back(edgeTable[y][k]);
                }

                // delete edges from the active edge table
                for (int k=0; k<activeEdgeTable.size(); k++)
                {
                    if (activeEdgeTable[k].maxY<=y) {
                        activeEdgeTable.erase(activeEdgeTable.begin()+k);
                        k--;
                    }
                }

                // sort according to x value
                sort(activeEdgeTable.begin(),activeEdgeTable.end(),act_cmp());

                //computeNormal();
                double gray = diffuse_term(1, 200, front_faces[i].normal, light_position, 3);
                if(gray < 0) gray = 0;
                
                if(gray > 255) gray = 255;
                
                double inten = (double)gray / 100.0;
                //printf(">%f\n",inten);
                //printf(">%f %f %f\n",front_faces[i].normal.x_position,front_faces[i].normal.y_position,front_faces[i].normal.z_position);
                //printf(">%f\n",gray);
                // draw scanline in pair
                for (int k=0; k<activeEdgeTable.size(); k+=2) {
                    dZ = (activeEdgeTable[k+1].currentZ - activeEdgeTable[k].currentZ) / (int)ceil(activeEdgeTable[k+1].minX - (int)ceil(activeEdgeTable[k].minX));
                    //printf(">%f\n",dZ);
                    z = activeEdgeTable[k].currentZ;
                    //printf(">%f\n",z);
                    for (int x=(int)ceil(activeEdgeTable[k].minX); x<activeEdgeTable[k+1].minX; x++) {

                        //printf(">%f%f%f\n",random[0],random[1],random[2]);
                        switch (colorID) {
                            case 1:
                            {
                                    auto r = 1*inten;
                                    auto g = 0.4*inten;
                                    auto b = 0*inten;
                                    setFrameBuffer(y,x,z,r,g,b);
                                    z+=dZ;
                            }
                                    break;
                                
                                
                            case 3:
                                if ((front_faces[i].id)%2==0) {
                                    //printf("> %d\n",front_faces[i].id);
                                    auto r = 1*inten;
                                    auto g = 1*inten;
                                    auto b = 1*inten;
                                    setFrameBuffer(y,x,z,r,g,b);
                                    z+=dZ;
                                    
                                }
                                
                                else{
                                    
                                    //printf("> %d\n",front_faces[i].id);
                                    auto r = 0*inten;
                                    auto g = 0.4*inten;
                                    auto b = 0*inten;
                                    setFrameBuffer(y,x,z,r,g,b);
                                    z+=dZ;
                                }
                                break;
                                
                        }
                       
                        
                        
                        //printf( ">%d %d\n",x,y );
                    }
                    
                }
                // update edges in active edge table
                for (int k=0; k<activeEdgeTable.size(); k++) {
                    activeEdgeTable[k].minX += activeEdgeTable[k].slopeRecip;
                    activeEdgeTable[k].currentZ += activeEdgeTable[k].incZ;
                }
                
            }
            //flag++;
        }

    }
    void drawGouraud(Matrix t,int colorID)
    {
        for (int i=0; i<vertices_number; i++) {
            // here we get the screen coordinate of each point
            current_points[i].vert = points[i].vert * t;
            //printf( "<%f %f %f\n", current_vertices[i].x_position, current_vertices[i].y_position, current_vertices[i].z_position );
        }
        // for each face we create an edge table
        for (int i=0; i<front_faces_number; i++)
        {
            //printf(">%f %f %f\n",front_faces[i].normal.x_position,front_faces[i].normal.y_position,front_faces[i].normal.z_position);
            int length = front_faces[i].sides_number;
            float z;
            float dZ = 0;
            float intensity;
            float dI = 0;
            //printf("> %d\n",front_faces[i].id);
            edgeTable.clear ( );

            for (int j=0; j<PIXELY; j++)
            {
                // add rows equal to height of image to active edge table
                vector<Edge> row;
                edgeTable.push_back ( row );
            }
            for (int k=0; k<length; k++)
            {
                Edge e;
                int p_current = front_faces[i].indices[k]-1;
                int p_next = front_faces[i].indices[(k+1)%length]-1;
                //double gray_current = diffuse_term(1, 200, points[p_current].normal, light_position, 3);
//                double gray_current = Phong_illumination(0.5, 0.8, 0.4, 10, 200, 3, 5, points[p_current].normal, light_position, camera_position);
                double gray_current = specular_term(0.2, 100, points[p_current].normal, light_position, camera_position, 5);
                //printf(">%f\n",gray_current);
                if(gray_current < 0) gray_current = 0;
                if(gray_current > 25) gray_current = 25;
                printf(">%f\n",gray_current);
                double inten_current = (double)gray_current/25 ;
                
                //double gray_next = diffuse_term(1, 200, points[p_next].normal, light_position, 3);
//                double gray_next = Phong_illumination(0.5, 0.8, 0.4, 10, 200, 3, 5, points[p_next].normal, light_position, camera_position);
                double gray_next = specular_term(0.2, 100, points[p_next].normal, light_position, camera_position, 5);
                if(gray_next < 0) gray_next = 0;
                if(gray_next > 25) gray_next = 25;
                double inten_next = (double)gray_next/25 ;
                current_points[p_current].intensity = inten_current;
                current_points[p_next].intensity = inten_next;
                // ignore the situations that the edge is horizontal
                if (to_pixel(current_points[p_current].vert.y_position)==to_pixel(current_points[p_next].vert.y_position)) {
                    continue;
                }
                // initialize the parameter of edge table
                // maxY is the y_position of the upper point
                // minX is the x_position of the lower point
                // slopeRecip is the change of x on y
                // incZ is the change of z on y(pixel)
                e.maxY = max(to_pixel(current_points[p_current].vert.y_position), to_pixel(current_points[p_next].vert.y_position));
                e.slopeRecip = ( current_points[p_current].vert.x_position - current_points[p_next].vert.x_position ) / (float)( current_points[p_current].vert.y_position - current_points[p_next].vert.y_position );
                e.incZ = ( current_points[p_current].vert.z_position - current_points[p_next].vert.z_position ) / (float)( to_pixel(current_points[p_current].vert.y_position) - to_pixel(current_points[p_next].vert.y_position) );
                e.incI = (current_points[p_current].intensity - current_points[p_next].intensity) / (float)( to_pixel(current_points[p_current].vert.y_position) - to_pixel(current_points[p_next].vert.y_position) );
                // judge if the current point is the upper point or the lower point
                if (to_pixel(current_points[p_current].vert.y_position) == e.maxY) {
                    e.minX = to_pixel(current_points[p_next].vert.x_position);
                    e.currentZ = current_points[p_next].vert.z_position;
                    e.currentI = current_points[p_next].intensity;
                    edgeTable[to_pixel(current_points[p_next].vert.y_position)].push_back(e);
                }
                else{
                    e.minX = to_pixel(current_points[p_current].vert.x_position);
                    e.currentZ = current_points[p_current].vert.z_position;
                    e.currentI = current_points[p_current].intensity;
                    edgeTable[to_pixel(current_points[p_current].vert.y_position)].push_back(e);
                }
                
            }
            //            for (int i=0; i<edgeTable.size(); i++) {
            //                printf("<%f %f %f\n",edgeTable[i][0].maxY,edgeTable[i][0].minX,edgeTable[i][0].slopeRecip);
            //            }
            activeEdgeTable.clear();
            for (int y=0; y<PIXELY; y++)
            {
                // initialize the active edge table
                for (int k=0; k<edgeTable[y].size(); k++)
                {
                    activeEdgeTable.push_back(edgeTable[y][k]);
                }
                
                // delete edges from the active edge table
                for (int k=0; k<activeEdgeTable.size(); k++)
                {
                    if (activeEdgeTable[k].maxY<=y) {
                        activeEdgeTable.erase(activeEdgeTable.begin()+k);
                        k--;
                    }
                }
                
                // sort according to x value
                sort(activeEdgeTable.begin(),activeEdgeTable.end(),act_cmp());
                
                // draw scanline in pair
                for (int k=0; k<activeEdgeTable.size(); k+=2) {
                    dZ = (activeEdgeTable[k+1].currentZ - activeEdgeTable[k].currentZ) / (int)ceil(activeEdgeTable[k+1].minX - (int)ceil(activeEdgeTable[k].minX));
                    //printf(">%f\n",dZ);
                    z = activeEdgeTable[k].currentZ;
                    //printf(">%f\n",z);
                    dI = (activeEdgeTable[k+1].currentI - activeEdgeTable[k].currentI) / (int)ceil(activeEdgeTable[k+1].minX - (int)ceil(activeEdgeTable[k].minX));
                    intensity = activeEdgeTable[k].currentI;
                    for (int x=(int)ceil(activeEdgeTable[k].minX); x<activeEdgeTable[k+1].minX; x++) {
                        
                        //printf(">%f%f%f\n",random[0],random[1],random[2]);
                        switch (colorID) {
                            case 1:
                            {
                                auto r = 1*intensity;
                                auto g = 0.4*intensity;
                                auto b = 0*intensity;
                                setFrameBuffer(y,x,z,r,g,b);
                                z+=dZ;
                                intensity+=dI;
                            }
                                break;
                                
                                
                            case 3:
                                if ((front_faces[i].id)%2==0) {
                                    //printf("> %d\n",front_faces[i].id);
                                    auto r = 1;
                                    auto g = 1;
                                    auto b = 1;
                                    setFrameBuffer(y,x,z,r,g,b);
                                    z+=dZ;
                                    
                                }
                                
                                else{
                                    
                                    //printf("> %d\n",front_faces[i].id);
                                    auto r = 0;
                                    auto g = 0.4;
                                    auto b = 0;
                                    setFrameBuffer(y,x,z,r,g,b);
                                    z+=dZ;
                                }
                                break;
                                
                        }
                        
                        
                        
                        //printf( ">%d %d\n",x,y );
                    }
                    
                }
                // update edges in active edge table
                for (int k=0; k<activeEdgeTable.size(); k++) {
                    activeEdgeTable[k].minX += activeEdgeTable[k].slopeRecip;
                    activeEdgeTable[k].currentZ += activeEdgeTable[k].incZ;
                    activeEdgeTable[k].currentI += activeEdgeTable[k].incI;
                }
                
            }
            //flag++;
        }
        
    }
    void drawPhong(Matrix t,int colorID)
    {
        for (int i=0; i<vertices_number; i++) {
            // here we get the screen coordinate of each point
            current_points[i].vert = points[i].vert * t;
            //printf( "<%f %f %f\n", current_vertices[i].x_position, current_vertices[i].y_position, current_vertices[i].z_position );
        }
        //int flag = 1;
        // for each face we create an edge table
        for (int i=0; i<front_faces_number; i++)
        {
            //printf(">%f %f %f\n",front_faces[i].normal.x_position,front_faces[i].normal.y_position,front_faces[i].normal.z_position);
            int length = front_faces[i].sides_number;
            float z;
            float dZ = 0;
            Vector3 n;
            n.x_position = n.y_position = n.z_position = 0;
            Vector3 dN;
            dN.x_position = dN.y_position = dN.z_position= 0;
            //printf("> %d\n",front_faces[i].id);
            edgeTable.clear ( );
            //float random[3];
            //            srand(time(NULL));//设置随机数种子，使每次产生的随机序列不同
            //            for (int i = 0; i < 3; i++)
            //            {
            //                random[i] = rand() % (N + 1) / (float)(N + 1);
            //            }
            for (int j=0; j<PIXELY; j++)
            {
                // add rows equal to height of image to active edge table
                vector<Edge> row;
                edgeTable.push_back ( row );
            }
            
            for (int k=0; k<length; k++)
            {
                Edge e;
                int p_current = front_faces[i].indices[k]-1;
                int p_next = front_faces[i].indices[(k+1)%length]-1;
                // ignore the situations that the edge is horizontal
                if (to_pixel(current_points[p_current].vert.y_position)==to_pixel(current_points[p_next].vert.y_position)) {
                    continue;
                }
                // initialize the parameter of edge table
                // maxY is the y_position of the upper point
                // minX is the x_position of the lower point
                // slopeRecip is the change of x on y
                // incZ is the change of z on y(pixel)
                e.maxY = max(to_pixel(current_points[p_current].vert.y_position), to_pixel(current_points[p_next].vert.y_position));
                e.slopeRecip = ( current_points[p_current].vert.x_position - current_points[p_next].vert.x_position ) / (float)( current_points[p_current].vert.y_position - current_points[p_next].vert.y_position );
                e.incZ = ( current_points[p_current].vert.z_position - current_points[p_next].vert.z_position ) / (float)( to_pixel(current_points[p_current].vert.y_position) - to_pixel(current_points[p_next].vert.y_position) );
                e.incN.x_position =(points[p_current].normal.x_position - points[p_next].normal.x_position) / (float)( to_pixel(current_points[p_current].vert.y_position) - to_pixel(current_points[p_next].vert.y_position) );
                e.incN.y_position =(points[p_current].normal.y_position - points[p_next].normal.y_position) / (float)( to_pixel(current_points[p_current].vert.y_position) - to_pixel(current_points[p_next].vert.y_position) );
                e.incN.z_position =(points[p_current].normal.z_position - points[p_next].normal.z_position) / (float)( to_pixel(current_points[p_current].vert.y_position) - to_pixel(current_points[p_next].vert.y_position) );
                // judge if the current point is the upper point or the lower point
                if (to_pixel(current_points[p_current].vert.y_position) == e.maxY) {
                    e.minX = to_pixel(current_points[p_next].vert.x_position);
                    e.currentZ = current_points[p_next].vert.z_position;
                    e.currentN = points[p_next].normal;
                    edgeTable[to_pixel(current_points[p_next].vert.y_position)].push_back(e);
                }
                else{
                    e.minX = to_pixel(current_points[p_current].vert.x_position);
                    e.currentZ = current_points[p_current].vert.z_position;
                    e.currentN = points[p_current].normal;
                    edgeTable[to_pixel(current_points[p_current].vert.y_position)].push_back(e);
                }
                
            }
            //            for (int i=0; i<edgeTable.size(); i++) {
            //                printf("<%f %f %f\n",edgeTable[i][0].maxY,edgeTable[i][0].minX,edgeTable[i][0].slopeRecip);
            //            }
            activeEdgeTable.clear();
            for (int y=0; y<PIXELY; y++)
            {
                // initialize the active edge table
                for (int k=0; k<edgeTable[y].size(); k++)
                {
                    activeEdgeTable.push_back(edgeTable[y][k]);
                }
                
                // delete edges from the active edge table
                for (int k=0; k<activeEdgeTable.size(); k++)
                {
                    if (activeEdgeTable[k].maxY<=y) {
                        activeEdgeTable.erase(activeEdgeTable.begin()+k);
                        k--;
                    }
                }
                
                // sort according to x value
                sort(activeEdgeTable.begin(),activeEdgeTable.end(),act_cmp());


                // draw scanline in pair
                for (int k=0; k<activeEdgeTable.size(); k+=2) {
                    dZ = (activeEdgeTable[k+1].currentZ - activeEdgeTable[k].currentZ) / (int)ceil(activeEdgeTable[k+1].minX - (int)ceil(activeEdgeTable[k].minX));
                    //printf(">%f\n",dZ);
                    z = activeEdgeTable[k].currentZ;
                    dN.x_position = (activeEdgeTable[k+1].currentN.x_position - activeEdgeTable[k].currentN.x_position) / (int)ceil(activeEdgeTable[k+1].minX - (int)ceil(activeEdgeTable[k].minX));
                    dN.y_position = (activeEdgeTable[k+1].currentN.y_position - activeEdgeTable[k].currentN.y_position) / (int)ceil(activeEdgeTable[k+1].minX - (int)ceil(activeEdgeTable[k].minX));
                    dN.z_position = (activeEdgeTable[k+1].currentN.z_position - activeEdgeTable[k].currentN.z_position) / (int)ceil(activeEdgeTable[k+1].minX - (int)ceil(activeEdgeTable[k].minX));
                    
                    n = activeEdgeTable[k].currentN;
                    //printf(">%f\n",z);
                    for (int x=(int)ceil(activeEdgeTable[k].minX); x<activeEdgeTable[k+1].minX; x++) {
                        
                        //printf(">%f%f%f\n",random[0],random[1],random[2]);
                        switch (colorID) {
                            case 1:
                            {
                                double gray = diffuse_term(0.8, 200, n, light_position, 10);
                                if(gray < 0) gray = 0;
                                
                                if(gray > 255) gray = 255;
                                
                                double inten = (double)gray / 100.0;
                                auto r = 1*inten;
                                auto g = 0.4*inten;
                                auto b = 0*inten;
                                setFrameBuffer(y,x,z,r,g,b);
                                z+=dZ;
                                n+=dN;
                            }
                                break;
                                
                                
                            case 3:
                                if ((front_faces[i].id)%2==0) {
                                    //printf("> %d\n",front_faces[i].id);
                                    double inten = diffuse_term(0.8, 200, n, light_position, 10);
                                    auto r = 1*inten;
                                    auto g = 1*inten;
                                    auto b = 1*inten;
                                    setFrameBuffer(y,x,z,r,g,b);
                                    z+=dZ;
                                    n+=dN;
                                    
                                }
                                
                                else{
                                    
                                    //printf("> %d\n",front_faces[i].id);
                                    double inten = diffuse_term(0.8, 200, n, light_position, 3);
                                    auto r = 0*inten;
                                    auto g = 0.4*inten;
                                    auto b = 0*inten;
                                    setFrameBuffer(y,x,z,r,g,b);
                                    z+=dZ;
                                    n+=dN;
                                }
                                break;
                                
                        }
                        
                        
                        
                        //printf( ">%d %d\n",x,y );
                    }
                    
                }
                // update edges in active edge table
                for (int k=0; k<activeEdgeTable.size(); k++) {
                    activeEdgeTable[k].minX += activeEdgeTable[k].slopeRecip;
                    activeEdgeTable[k].currentZ += activeEdgeTable[k].incZ;
                    activeEdgeTable[k].currentN += activeEdgeTable[k].incN;
                }
                
            }
            //flag++;
        }
        
    }
    // Sets pixel x,y to the color RGB
    void setFrameBuffer(int width, int height, float depth, float R, float G, float B)
    {
        if (zbuffer[width][height]<depth) {
           // update zbuffer
            zbuffer[width][height] = depth;
            //printf("> %f\n",zbuffer[width][height]);
//            printf("%f\n",zbuffer[width][height]);
            if (R<=1.0){
                if (R>=0.0)
                    framebuffer[width][height][0]=R;
                else
                    framebuffer[width][height][0]=0.0;
                
            }
            else
                framebuffer[width][height][0]=1.0;
            if (G<=1.0){
                if (G>=0.0)
                    framebuffer[width][height][1]=G;
                else
                    framebuffer[width][height][1]=0.0;
                
            }
            else
                framebuffer[width][height][1]=1.0;
            if (B<=1.0){
                if (B>=0.0)
                    framebuffer[width][height][2]=B;
                else
                    framebuffer[width][height][2]=0.0;
                
            }
            else
                framebuffer[width][height][2]=1.0;
        }
    }
    
    void computeNormal()
    {
        for (int i=0; i<front_faces_number; i++) {
            int p0=front_faces[i].indices[0] - 1;
            int p1=front_faces[i].indices[(1)%faces[i].sides_number] - 1;
            int p2=front_faces[i].indices[(2)%faces[i].sides_number] - 1;
            front_faces[i].normal = (points[p1].vert - points[p0].vert).cross(points[p2].vert - points[p1].vert);
            front_faces[i].normal = front_faces[i].normal.normalize();
            for(int j=0; j<front_faces[i].sides_number; j++) {
                int p=front_faces[i].indices[j]-1;
                points[p].normal=points[p].normal+front_faces[i].normal;
            }
        }
        for (int i=0; i<vertices_number; i++) {
            points[i].normal=points[i].normal.normalize();
        }
    }
       

};

#endif /* Model_h */
