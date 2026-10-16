#pragma once
#include <common.h>
#include <string>
class GLTexture {
public:
    GLuint tex = 0;
    GLint width = 0, height = 0;
    void loadTexture(const char* filename);
    void bindTexture();
private:
    std::string loadedPath;
};
