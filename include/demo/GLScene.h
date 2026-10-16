#ifndef GLSCENE_H
#define GLSCENE_H

#include <common.h>


class GLScene
{
    public:
        GLScene();
        virtual ~GLScene();
        GLint initGL();
        GLint drawScene();
        GLvoid resizeScene(GLsizei, GLsizei);
        GLvoid switchLevel(int);

        void handleHorizontalInput(int direction);
        struct State { int level; int lives; float playerX; float playerY; int collisions; };
        State state() const;

        int collisions = 0;

        float screenWidth;                     // get values from main window
        float screenHeight;                    // get Values from main window
        int flag;                              // current level we are on
        float radius;
        int lives;
        float ground;
        int none;

    protected:

    private:
};

#endif // GLSCENE_H
