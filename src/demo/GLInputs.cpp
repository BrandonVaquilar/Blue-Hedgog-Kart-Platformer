#include "GLInputs.h"
void GLInputs::keyPress(GLModel*, GLPlayer* player) {
    player->actionTrigger = direction < 0 ? GLPlayer::WALKLEFT
        : direction > 0 ? GLPlayer::WALKRIGHT : GLPlayer::STAND;
}
void GLInputs::keyBackground(GLParallax* layer, float speed) {
    layer->xMin += direction * speed;
    layer->xMax += direction * speed;
}
