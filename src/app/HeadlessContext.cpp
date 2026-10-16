#include "HeadlessContext.h"
#include <EGL/eglext.h>
#include <stdexcept>
#include <string>

namespace {
void require(bool success, const char* operation) {
    if (!success) throw std::runtime_error(std::string(operation)+" failed, EGL error "+std::to_string(eglGetError()));
}
}
HeadlessContext::HeadlessContext(int width, int height) {
    try {
        display = eglGetPlatformDisplay(EGL_PLATFORM_SURFACELESS_MESA, EGL_DEFAULT_DISPLAY, nullptr);
        require(display != EGL_NO_DISPLAY, "Surfaceless display");
        require(eglInitialize(display, nullptr, nullptr), "EGL initialization");
        require(eglBindAPI(EGL_OPENGL_API), "OpenGL API selection");
        const EGLint attributes[] = {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE
        };
        EGLConfig config{};
        EGLint count = 0;
        require(eglChooseConfig(display, attributes, &config, 1, &count) && count > 0, "Framebuffer configuration");
        const EGLint size[] = {EGL_WIDTH, width, EGL_HEIGHT, height, EGL_NONE};
        surface = eglCreatePbufferSurface(display, config, size);
        require(surface != EGL_NO_SURFACE, "Offscreen framebuffer");
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, nullptr);
        require(context != EGL_NO_CONTEXT, "OpenGL context");
        require(eglMakeCurrent(display, surface, surface, context), "Current context");
    } catch (...) { close(); throw; }
}
HeadlessContext::~HeadlessContext() { close(); }
void HeadlessContext::close() {
    if (display == EGL_NO_DISPLAY) return;
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
    if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
    eglTerminate(display);
    display = EGL_NO_DISPLAY;
}
