#include "GLCheckCollision.h"

GLCheckCollision::GLCheckCollision()
{
    //ctor
}

GLCheckCollision::~GLCheckCollision()
{
    //dtor
}

bool GLCheckCollision::isLinearCollision(vec2, vec2)
{

}

bool GLCheckCollision::isRadialCollision(vec2 p1, vec2 p2, float r1, float r2, float thresHold)
// position1, position2, radius1, radius2, Threshold
{
    return(sqrt( pow( (p1.x - p2.x),2)+ pow( (p1.y - p2.y),2)) < (r1+r2) );
}

bool GLCheckCollision::isHitPlane(vec2, vec2)
{

}

bool GLCheckCollision::isCubicCollision(vec3, vec3)
{

}

bool GLCheckCollision::isSphereCollision(vec3 p1, vec3 p2, float r1, float r2, float thresHold)
{
 return(abs(sqrt( pow((p1.x - p2.x),2)+ pow( (p1.y - p2.y),2)+pow((p1.z - p2.z),2))-(r1+r2)) > thresHold);
}

bool GLCheckCollision::isHitPlane3D(vec3, vec3)
{

}
