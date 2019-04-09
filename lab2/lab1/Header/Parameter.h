//
//  Parameter.h
//  lab1
//
//  Created by 张芹菜 on 2019/2/24.
//  Copyright © 2019 张芹菜. All rights reserved.
//

#ifndef Parameter_h
#define Parameter_h

#define PIXELX 800
#define PIXELY 800
#define PIXELZ 800
#define N 999/Users/aaa/Desktop/Computer Graphics 2/lab2/lab1/Header/Illumination.h
#define COLOR_NUM 10    //number of color
Vector3 globalColor[COLOR_NUM]={Vector3(1.0, 0.0, 1.0), Vector3(1.0, 1.0, 0.0)};
Vector3 globalcolor(1.0,0.0,1.0);
Vector3 camera_position{0.0, 6.0, 25.0};
Vector3 light_position{0,10,10};
float Zbuffer[PIXELX][PIXELY];
Vector3 Ibuffer[PIXELX][PIXELY];
float xValue[PIXELX];
float yValue[PIXELY];
float zValue[PIXELZ];
float xIncreasement;
float yIncreasement;
float zIncreasement;
#endif /* Parameter_h */
