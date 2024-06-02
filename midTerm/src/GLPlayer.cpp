#include "GLPlayer.h"

GLPlayer::GLPlayer()
{
    int lives;
    //ctor
    vert[0].x =-0.5; vert[0].y = -0.5; vert[0].z =-1.0;
    vert[1].x = 0.5; vert[1].y = -0.5; vert[1].z =-1.0;
    vert[2].x = 0.5; vert[2].y =  0.5; vert[2].z =-1.0;
    vert[3].x =-0.5; vert[3].y =  0.5; vert[3].z =-1.0;
}

GLPlayer::~GLPlayer()
{
    //dtor
}
void GLPlayer::initPlayer(int x, int y, char* fileName)
{
    plPosition.x =-10.2;         // initialize positions
    plPosition.y =-0.171;
    plPosition.z = -1.0;

    plScale.x = 2.2;           // initialize scale
    plScale.y = 2.2;
    plScale.z = -1.0;


    framesX = x;               // record number of frames
    framesY = y;

    texture->loadTexture(fileName); //loading my background

    xMin =0;
    yMax =2.0/(float)framesY;
    xMax =1.0/(float)framesX;
    yMin =yMax-1.0/(float)framesY;

    actionTrigger=0;
    myTime->startTime = clock();
}

void GLPlayer::drawPlayer()
{

    glTranslatef(plPosition.x,plPosition.y,plPosition.z);
    glScalef(plScale.x,plScale.y,plScale.z);

    glColor3f(1.0,1.0,1.0);    //white rectangle
    texture->bindTexture();    //binding my background


    glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMax);
      glVertex3f(vert[0].x,vert[0].y,vert[0].z);

      glTexCoord2f(xMax,yMax);
      glVertex3f(vert[1].x,vert[1].y,vert[1].z);

      glTexCoord2f(xMax,yMin);
      glVertex3f(vert[2].x,vert[2].y,vert[2].z);

      glTexCoord2f(xMin,yMin);
      glVertex3f(vert[3].x,vert[3].y,vert[3].z);

    glEnd();
}

void GLPlayer::actions(int lives)
{


   switch(actionTrigger)
   {
   case STAND:

        xMax = 7.0/(float)framesX;
        xMin =6.0/(float)framesX;


    if(clock() - myTime->startTime>420)
    {
       if(yMax <= (1/2.0) ) {
       yMin += 1.0/(float)framesY;
       yMax += 1.0/(float)framesY;
       }
       else
       {
        yMin = 0.0;
        yMax = 1.0/(float)framesY;
       }
     myTime->startTime =clock();
    }
      // xMin =0.0;
      // xMax = 1.0/(float)framesX;


       break;

   case WALKLEFT:
      yMin = 0.0;
      yMax = 1.0/(float)framesY;

    if(clock() - myTime->startTime>60)
   {
       if(xMax == 7.0/(float)framesX) {
        xMax = 6.0/(float)framesX;
        xMin = 7.0/(float)framesX;
       }
       else if(xMin > 4.0/(float)framesX) {
       xMax -= 1.0/(float)framesX;
       xMin -= 1.0/(float)framesX;
       }
       else
       {
        xMin = 4.0/(float)framesX;
        xMax = 3.0/(float)framesX;
       }
     myTime->startTime =clock();
   }
       plPosition.x -= 0.03;
       break;

   case WALKRIGHT:
        yMin = 0.0;
        yMax = 1.0/(float)framesY;

    if(clock() - myTime->startTime>60)
   {
       if(xMax > 4.0/(float)framesX) {
       xMin -= 1.0/(float)framesX;
       xMax -= 1.0/(float)framesX;
       }
       else
       {
        xMin = 3.0/(float)framesX;
        xMax = 4.0/(float)framesX;
       }
     myTime->startTime =clock();
   }
    plPosition.x += 0.03;
       break;

   case RUN: break;
   case JUMP: break;
   case ATTACK: break;
   case HEALTH:
       switch (lives)
       {
       case 5:
      xMin = 2.04/(float)framesX;
      xMax = 2.96/(float)framesX;
      yMin = 4.04/(float)framesY;
      yMax = 4.96/(float)framesY;
        break;
       case 4:
      xMin = 2.04/(float)framesX;
      xMax = 2.96/(float)framesX;
      yMin = 3.04/(float)framesY;
      yMax = 3.96/(float)framesY;
        break;
       case 3:
      xMin = 2.04/(float)framesX;
      xMax = 2.96/(float)framesX;
      yMin = 2.04/(float)framesY;
      yMax = 2.96/(float)framesY;
        break;
       case 2:
      xMin = 2.04/(float)framesX;
      xMax = 2.96/(float)framesX;
      yMin = 1.04/(float)framesY;
      yMax = 1.96/(float)framesY;
        break;
       case 1:
      xMin = 3.04/(float)framesX;
      xMax = 3.96/(float)framesX;
      yMin = 0.04/(float)framesY;
      yMax = 0.96/(float)framesY;
        break;
       }

     //  cout << "working" << endl;
    break;
   default:

       break;
   }
}
