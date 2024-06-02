#ifndef GLCHECKCOLLISION_H
#define GLCHECKCOLLISION_H

#include<common.h>

class GLCheckCollision
{
    public:
        GLCheckCollision();
        virtual ~GLCheckCollision();


          bool isLinearCollision(vec2, vec2); // check x,x1 or y.y1 components
          bool isRadialCollision(vec2,vec2,float,float,float); // element1 , element2 radius1, radius2
          bool isHitPlane(vec2,vec2);// check object vs floor/wall

          bool isCubicCollision(vec3, vec3); // check x,y.z components of two models
          bool isSphereCollision(vec3,vec3,float,float,float); // element1 , element2 radius1, radius2
          bool isHitPlane3D(vec3,vec3);// check object vs floor/wall

    protected:

    private:
};

#endif // GLCHECKCOLLISION_H
