#include "app/Options.h"
#include "app/Capture.h"
#include "app/HeadlessContext.h"
#include "app/InputScript.h"
#include <GLTimer.h>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>

int main(int argc, char** argv) try {
    const auto options = parseOptions(argc, argv);
    if (options.help) { printHelp(); return 0; }
    if (options.frames == 0) throw std::invalid_argument("The capture runner requires --frames");
    const auto inputs = options.inputScript.empty() ? std::vector<int>{}
        : loadInputScript(options.inputScript, options.frames);
    HeadlessContext context(options.width, options.height);
    GLTimer::useFixedTime(0);
    std::srand(options.seed);
    GLScene scene;
    scene.initGL();
    scene.switchLevel(options.level);
    scene.resizeScene(options.width, options.height);
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << '\n';
    for (int frame = 0; frame < options.frames; ++frame) {
        GLTimer::useFixedTime(frame*(1000.0/60.0));
        scene.handleHorizontalInput(inputs.empty() ? (options.demo ? 1 : 0) : inputs[frame]);
        scene.drawScene();
        if (glGetError() != GL_NO_ERROR) throw std::runtime_error("OpenGL rendering failed");
        if (!options.frameDirectory.empty() && frame % options.captureEvery == 0) {
            std::ostringstream name;
            name << std::setw(5) << std::setfill('0') << frame/options.captureEvery;
            const auto path = std::filesystem::path(options.frameDirectory)/name.str();
            captureFrame(path.string()+".ppm", options.width, options.height, true);
            saveState(path.string()+".json", scene.state(), frame+1);
        }
    }
    if (!options.screenshot.empty()) captureFrame(options.screenshot, options.width, options.height, true);
    if (!options.state.empty()) saveState(options.state, scene.state(), options.frames);
    const auto state = scene.state();
    std::cout << "Rendered " << options.frames << " frames; scene " << state.level
              << ", lives " << state.lives << ", collisions " << state.collisions << '\n';
    return 0;
} catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
}
