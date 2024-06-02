#include "GLLight.h"

GLLight::GLLight()
{
    //ctor
}

GLLight::~GLLight()
{
    //dtor
}

GLLight::GLLight(GLenum Light)
{
    glEnable(Light);
    glEnable(GL_LIGHTING);
}

GLvoid GLLight::setLight(GLenum Light)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(Light, GL_AMBIENT,  light_ambient);
    glLightfv(Light, GL_DIFFUSE,  light_diffuse);
    glLightfv(Light, GL_SPECULAR, light_specular);
    glLightfv(Light, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}
