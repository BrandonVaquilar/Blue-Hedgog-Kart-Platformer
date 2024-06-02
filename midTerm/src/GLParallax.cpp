#include "GLParallax.h"

GLParallax::GLParallax()
{
    //ctor
    xMax=1.0;
    xMin=0.0;
    yMax =0.0;
    yMin = 1.0;
    bool isLeft = false;

}

GLParallax::~GLParallax()
{
    //dtor
}

void GLParallax::parallaxInit(char* fileName)
{

    background->loadTexture(fileName); //loading my background
    myTime->startTime = clock();
}

void GLParallax::parallaxScroll(bool Auto, string dir, float speed)
{
    if(Auto)
    {
        if(clock() - myTime->startTime>15)
        {

        if(dir=="up")
        {
           yMin -=speed;
           yMax -=speed;
        }
        if(dir=="down")
        {
           yMin +=speed;
           yMax +=speed;
        }
        if(dir=="left")
        {
           xMin -=speed;
           xMax -=speed;
        }
        if(dir=="right")
        {
           xMin +=speed;
           xMax +=speed;
        }
        if(dir=="level1")
        {
           yMin -=speed;
           yMax -=speed;
           xMin +=speed;
           xMax +=speed;
        }
        if(dir=="level2")
        {

            if (isLeft) {
            if (xMax <= -0.75) {
                    isLeft = false;
            }
            xMin -=speed;
            xMax -=speed;
            }

            else {
            if (xMax >= .07) {
                    isLeft = true;
            }
            xMin +=speed;
            xMax +=speed;
            }
        }


        myTime->startTime =clock();
        }
    }
}

void GLParallax::parallaxDraw(float width, float height)
{
    glColor3f(1.0,1.0,1.0); //white rectangle
    background->bindTexture();    //binding my background
    glBegin(GL_POLYGON);
        glTexCoord2f(xMin,yMin);
        glVertex3f(-width/height,-1,-8.0);

        glTexCoord2f(xMax,yMin);
        glVertex3f( width/height,-1,-8.0);

        glTexCoord2f(xMax,yMax);
        glVertex3f( width/height,1,-8.0);

        glTexCoord2f(xMin,yMax);
        glVertex3f(-width/height,1,-8.0);
    glEnd();

}
