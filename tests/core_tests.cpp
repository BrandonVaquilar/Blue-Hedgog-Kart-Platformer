#include <GLCheckCollision.h>
#include <GLPlayer.h>
#include <GLInputs.h>
#include <GLTimer.h>
#include <GLParallax.h>
#include <cmath>
#include <iostream>
#include <stdexcept>

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
int main() try {
    GLCheckCollision collision;
    check(collision.isRadialCollision({0,0},{0.5f,0},0.4f,0.4f,0), "overlap should collide");
    check(!collision.isRadialCollision({0,0},{1,0},0.5f,0.5f,0), "touching is not overlap");
    check(!collision.isRadialCollision({-5,3},{5,-3},0.5f,0.5f,0), "distant circles should not collide");
    GLPlayer player;
    player.plPosition = {0,0,0}; player.framesX = 7; player.framesY = 2;
    GLInputs input;
    GLTimer::useFixedTime(0);
    input.direction = 1; input.keyPress(nullptr, &player);
    player.actions(-1);
    check(std::abs(player.plPosition.x-0.03f) < 0.0001f, "right input must move player");
    input.direction = -1; input.keyPress(nullptr, &player);
    player.actions(-1);
    check(std::abs(player.plPosition.x) < 0.0001f, "left input must reverse right movement");
    input.direction = 0; input.keyPress(nullptr, &player);
    player.actions(-1);
    check(std::abs(player.plPosition.x) < 0.0001f, "released input must stop player");
    GLParallax layer;
    check(!layer.isLeft, "parallax direction must be initialized");
    GLTimer::useFixedTime(100);
    layer.myTime->startTime = 0;
    layer.parallaxScroll(true,"down",0.008f);
    check(std::abs(layer.yMin-1.008f) < 0.0001f, "automatic layer movement must advance");
    GLTimer::useFixedTime(1234);
    check(GLTimer::now()==1234, "preview clock must be deterministic");
    std::cout << "PASS: collisions, player input, parallax initialization and deterministic timing\n";
} catch (const std::exception& error) {
    std::cerr << error.what() << '\n'; return 1;
}
