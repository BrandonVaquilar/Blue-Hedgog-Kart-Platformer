#pragma once
#include <common.h>
class GLCheckCollision {
public:
    bool isRadialCollision(vec2 first, vec2 second, float firstRadius, float secondRadius, float threshold);
};
