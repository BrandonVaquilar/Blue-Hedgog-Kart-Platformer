#include "GLTexture.h"
void GLTexture::loadTexture(const char* filename) {
    // Respawning a hazard reuses its texture instead of allocating another one.
    if (tex && loadedPath == filename) return;
    unsigned char* image = SOIL_load_image(filename, &width, &height, nullptr, SOIL_LOAD_RGBA);
    if (!image) throw std::runtime_error(std::string("Cannot load texture: ")+filename+" ("+SOIL_last_result()+")");
    if (!tex) glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    loadedPath = filename;
}
void GLTexture::bindTexture() { glBindTexture(GL_TEXTURE_2D, tex); }
