#ifndef GLENMS_H
#define GLENMS_H

#include<common.h>
#include<GLTexture.h>
#include<GLTimer.h>
#include <cstdlib>
#include <cmath>

class GLEnms
{
    public:
        GLEnms();
        virtual ~GLEnms();

        GLTexture *texture = new GLTexture();
        void initEnemy(int,int,char* fileName,int); // number of x, y frames

        void drawEnemy(int);
        void PlaceEnemy(vec3);
        void actions(int);

        bool isEnemyLive =true;

        vec3 pos;
        vec2 eScale;
        vec3 eRotate;

        int framesX;
        int framesY;
        int flag;

        int action;
        float speed;
        float xMax,xMin,yMax,yMin;
        GLTimer   *myTime = new GLTimer();

    protected:

    private:
};

#endif // GLENMS_H
