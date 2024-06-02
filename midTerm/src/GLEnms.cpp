#include "GLEnms.h"

GLEnms::GLEnms()
{
    //ctor

    GLTexture *texture = new GLTexture();

    pos.x =0;    // Default position settings
    pos.y =2.0;
    pos.z =-1.0;

    eScale.x =1.0; // default scaling
    eScale.y =1.0;

    eRotate.x =0;  // default rotations
    eRotate.y =0;
    eRotate.z =0;

    framesX =7;   // number of columns in the sprite sheet
    framesY =2;   // number of rows in the spritesheet

    xMax =1.0/framesX;  // default image
    xMin=0;
    yMax=1.0/framesY;
    yMin=0;

    speed =0.5;  // moving speed
    action=1;     // set actions
    myTime->startTime = clock();
}

GLEnms::~GLEnms()
{
    //dtor

}
void GLEnms::initEnemy(int x, int y, char* fileName, int flag)
{
    switch(flag)
    {
    case 0:
        pos.x = (std::abs(std::fmod(rand(), 19.6))) + (-9.3);         // initialize positions
        break;
    case 1:
        pos.x = (std::abs(std::fmod(rand(), 19.6)));         // initialize positions
        break;
    case 2:
        pos.x = (std::abs(std::fmod(rand(), 19.6))) + (-9.3);         // initialize positions
        break;
    }
    //pos.x = 0.0;
    pos.y = (std::abs(std::fmod(rand(), 15.0))) + (5.0);
   //pos.y = -2.0;
    pos.z = -1.0;

    eScale.x = 2.20;           // initialize scale
    eScale.y = 2.20;

    framesX = x;               // record number of frames
    framesY = y;

    texture->loadTexture(fileName); //loading my background
/*
    xMin =0;
    yMax =2.0/(float)framesY;
    yMax =(float)framesY;
    xMax =1.0/(float)framesX;
    yMin =yMax-1.0/(float)framesY;
*/
    xMin =0;
    yMax =2.0/(float)framesY;
    yMax =(float)framesY;
    xMax =1.0/(float)framesX;
    yMin =yMax-1.0/(float)framesY;


    myTime->startTime = clock();
}
void GLEnms::drawEnemy(int flag)
{
    glPushMatrix();
   // glScalef(10,10,-1.0);
    glDisable(GL_LIGHTING);
    glTranslatef(pos.x,pos.y,pos.z);
    glRotatef(eRotate.x,1,0,0);
    glRotatef(eRotate.y,0,1,0);
    glRotatef(eRotate.z,0,0,1);
   switch(flag)
   {
case 0:
    glScalef(1.0,1.0,-1.0);
    break;
   }
    //glScalef(0.2,0.2,1.0);
    glColor3f(1.0,1.0,1.0);    //white rectangle
    texture->bindTexture();    //binding my background


     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMax);
      glVertex3f(-1,-1,0);

      glTexCoord2f(xMax,yMax);
      glVertex3f(1,-1,0);

      glTexCoord2f(xMax,yMin);
      glVertex3f(1,1,0);

      glTexCoord2f(xMin,yMin);
      glVertex3f(-1,1,0);



   glEnd();
   glPopMatrix();

}

void GLEnms::PlaceEnemy(vec3 p)
{
    pos.x =p.x;
    pos.y =p.y;
    pos.z =p.z;
}

void GLEnms::actions(int action)
{
    switch(action)
    {
    case 0: // level 0
        yMin= 0.0;
        yMax=1.0;
        xMin = 0.0;
        xMax = 1.0;

        if(clock() - myTime->startTime>60){
        xMin +=1.0/framesX;
        xMax +=1.0/framesX;

        pos.y -=speed;

        myTime->startTime =clock();
        }
        break;

    case 1:
        yMin= 0.0;
        yMax=1.0;
        xMin = 0.0;
        xMax = 1.0;

        if(clock() - myTime->startTime>60){
        xMin +=1.0/framesX;
        xMax +=1.0/framesX;

        pos.y -=speed;
        pos.x -=speed;
        eRotate.z += speed * 50.0;
        myTime->startTime =clock();
        }
        break;


    case 2:
        yMin= 0.0;
        yMax=1.0;
        xMin = 0.0;
        xMax = 1.0;

        if(clock() - myTime->startTime>60){
        xMin +=1.0/framesX;
        xMax +=1.0/framesX;

        pos.y -=speed;
        pos.x += (std::abs(std::fmod(rand(), 0.3))) + (-0.15);
        myTime->startTime =clock();
        }
        break;

    }

}
