#pragma once
#include <GLModel.h>
#include <GLParallax.h>
#include <GLPlayer.h>
class GLInputs {
public:
    int direction = 0;
    void keyPress(GLModel*, GLPlayer*);
    void keyBackground(GLParallax*, float speed);
};
