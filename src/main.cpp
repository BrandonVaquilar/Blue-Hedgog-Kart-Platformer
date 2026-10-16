#include "app/Options.h"
#include "app/Capture.h"
#include <GLTimer.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <thread>

int main(int argc, char** argv) {
    GLFWwindow* window = nullptr;
    try {
        const auto options = parseOptions(argc, argv);
        if (options.help) { printHelp(); return 0; }
        // CMake copies assets beside the executable for launches from other directories.
        if (!std::filesystem::exists("assets/images/sonic.png")) {
            const auto directory = std::filesystem::absolute(argv[0]).parent_path();
            if (!std::filesystem::exists(directory/"assets/images/sonic.png"))
                throw std::runtime_error("Cannot find assets; run from the repository root or build directory");
            std::filesystem::current_path(directory);
        }
        glfwSetErrorCallback([](int code, const char* message) {
            std::cerr << "GLFW " << code << ": " << message << '\n';
        });
        if (!glfwInit()) throw std::runtime_error("Could not initialize GLFW; a graphical display is required");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_VISIBLE, options.hidden ? GLFW_FALSE : GLFW_TRUE);
        window = glfwCreateWindow(options.width, options.height, "Game Engine Demo | Blue Hedgehog Kart", nullptr, nullptr);
        if (!window) throw std::runtime_error("Could not create the OpenGL window");
        glfwMakeContextCurrent(window);
        glfwSwapInterval(options.frames ? 0 : 1);
        std::srand(options.seed);
        if (options.frames) GLTimer::useFixedTime(0);
        GLScene scene;
        scene.initGL();
        scene.switchLevel(options.level);
        const auto* renderer = glGetString(GL_RENDERER);
        std::cout << "OpenGL renderer: " << (renderer ? reinterpret_cast<const char*>(renderer) : "unknown") << '\n';
        int rendered = 0;
        auto nextFrame = std::chrono::steady_clock::now();
        while (!glfwWindowShouldClose(window) && (!options.frames || rendered < options.frames)) {
            glfwPollEvents();
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
            if (options.frames) GLTimer::useFixedTime(rendered*(1000.0/60.0));
            const bool left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
            const bool right = options.demo || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
            scene.handleHorizontalInput(static_cast<int>(right)-static_cast<int>(left));
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            if (width > 0 && height > 0) {
                scene.resizeScene(width, height);
                scene.drawScene();
                if (glGetError() != GL_NO_ERROR) throw std::runtime_error("OpenGL rendering failed");
                if (options.frames && rendered+1 == options.frames && !options.screenshot.empty())
                    captureFrame(options.screenshot, width, height);
                glfwSwapBuffers(window);
            }
            ++rendered;
            if (!options.frames) {
                nextFrame += std::chrono::microseconds(16667);
                std::this_thread::sleep_until(nextFrame);
                // Do not catch up with a burst after a long pause or window drag.
                if (nextFrame < std::chrono::steady_clock::now()-std::chrono::milliseconds(100))
                    nextFrame = std::chrono::steady_clock::now();
            }
        }
        if (!options.state.empty()) saveState(options.state, scene.state(), rendered);
        const auto state = scene.state();
        std::cout << "Rendered " << rendered << " frames; scene " << state.level
                  << ", lives " << state.lives << ", collisions " << state.collisions << '\n';
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
}
