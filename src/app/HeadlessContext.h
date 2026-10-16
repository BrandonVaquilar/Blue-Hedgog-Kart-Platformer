#pragma once
#include <EGL/egl.h>

class HeadlessContext {
public:
    HeadlessContext(int width, int height);
    ~HeadlessContext();
    HeadlessContext(const HeadlessContext&) = delete;
    HeadlessContext& operator=(const HeadlessContext&) = delete;
private:
    void close();
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
};
