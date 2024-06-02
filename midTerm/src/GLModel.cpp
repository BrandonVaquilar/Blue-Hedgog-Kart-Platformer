#include "GLModel.h"

GLModel::GLModel()
{
    //ctor

        RotateX= 0.0;  // init rotations
        RotateY= 0.0;
        RotateZ= 0.0;


        xPos = 0.0;    // init translation
        yPos = 0.0;
        zPos = 0.0;
}

GLModel::~GLModel()
{
    //dtor
}
void GLModel::drawModel()
{
    glColor3f(1.0,0.5,1.0);     // set model color

    glTranslatef(xPos,yPos,zPos); // set model location

    glRotatef(RotateX, 1,0,0);   // rotation controlers
    glRotatef(RotateY, 0,1,0);
    glRotatef(RotateZ, 0,0,1);

    glutSolidCube(0.2);     // load and render object n

}
