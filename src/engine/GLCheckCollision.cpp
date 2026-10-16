#include "GLCheckCollision.h"
bool GLCheckCollision::isRadialCollision(vec2 first, vec2 second, float firstRadius, float secondRadius, float) {
    // Preserve the game's strict circular-overlap rule; touching alone is not a hit.
    const float dx = first.x-second.x, dy = first.y-second.y;
    const float radius = firstRadius+secondRadius;
    return dx*dx+dy*dy < radius*radius;
}
