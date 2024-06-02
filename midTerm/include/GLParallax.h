#ifndef GLPARALLAX_H
#define GLPARALLAX_H

#include<common.h>
#include<GLTexture.h>
#include<GLTimer.h>

class GLParallax
{
    public:
        GLParallax();

        GLTexture *background = new GLTexture();
        GLTimer   *myTime = new GLTimer();
        virtual ~GLParallax();

        void parallaxInit(char *);// load background file
        void parallaxScroll(bool,string,float); // auto, direction, speed
        void parallaxDraw(float, float); // width and height

        float xMax,xMin,yMax,yMin,temp;
        bool isLeft;
    protected:

    private:
};

#endif // GLPARALLAX_H
