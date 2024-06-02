#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED


#include<windows.h>
#include<iostream>
#include<gl/gl.h>
#include<GL/glut.h>
#include<time.h>
#include<math.h>

#include<SOIL.h>

using namespace std;

typedef struct{   // shader positions in 3D
    float x;
    float y;
    float z;
    float w;
}vec4;


typedef struct{  //  positions in 3D graphics
    float x;
    float y;
    float z;
}vec3;

typedef struct{  //  positions in 3D graphics
    float x;
    float y;
}vec2;


#endif // COMMON_H_INCLUDED
