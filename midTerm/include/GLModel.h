#ifndef GLMODEL_H
#define GLMODEL_H

#include<common.h>


class GLModel
{
    public:
        GLModel();
        virtual ~GLModel();

        GLfloat RotateX;  // for rotation around X
        GLfloat RotateY;  // for rotation around Y
        GLfloat RotateZ;  // for rotation around Z

    //    GLfloat zoom;
        GLfloat xPos;     // model x possition
        GLfloat yPos;     // model y possition
        GLfloat zPos;     // model z possition

        void drawModel(); // model drawing

    protected:

    private:
};

#endif // GLMODEL_H
