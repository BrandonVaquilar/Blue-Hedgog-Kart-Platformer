#include "Options.h"
#include <iostream>
#include <stdexcept>

Options parseOptions(int argc, char** argv) {
    Options options;
    auto number = [](const std::string& value, int minimum, int maximum) {
        std::size_t end = 0;
        int result = std::stoi(value, &end);
        if (end != value.size() || result < minimum || result > maximum)
            throw std::invalid_argument("Numeric argument is outside its allowed range");
        return result;
    };
    for (int i = 1; i < argc; ++i) {
        const std::string flag = argv[i];
        auto value = [&]() -> std::string {
            if (++i >= argc) throw std::invalid_argument("Missing value after " + flag);
            return argv[i];
        };
        if (flag == "--help") options.help = true;
        else if (flag == "--demo") options.demo = true;
        else if (flag == "--hidden") options.hidden = true;
        else if (flag == "--level") options.level = number(value(), 0, 2);
        else if (flag == "--frames") options.frames = number(value(), 1, 10000);
        else if (flag == "--width") options.width = number(value(), 320, 1920);
        else if (flag == "--height") options.height = number(value(), 180, 1080);
        else if (flag == "--seed") options.seed = number(value(), 0, 2147483647);
        else if (flag == "--screenshot") options.screenshot = value();
        else if (flag == "--state") options.state = value();
        else if (flag == "--input-script") options.inputScript = value();
        else if (flag == "--frame-directory") options.frameDirectory = value();
        else if (flag == "--capture-every") options.captureEvery = number(value(), 1, 60);
        else throw std::invalid_argument("Unknown option: " + flag);
    }
    if ((!options.screenshot.empty() || !options.state.empty() || !options.inputScript.empty() || !options.frameDirectory.empty()) && options.frames == 0)
        throw std::invalid_argument("Captures require --frames so the run has a defined endpoint");
    return options;
}

void printHelp() {
    std::cout << "Blue Hedgehog Game Engine - Platformer Demo\n"
              << "Controls: Left/Right or A/D to move; Escape to quit.\n"
              << "  --level 0|1|2     Starting scene\n"
              << "  --frames N        Run N fixed 60 Hz frames, then exit (1-10000)\n"
              << "  --demo            Simulate holding Right for a reproducible preview\n"
              << "  --hidden          Render without showing the window\n"
              << "  --width N         Width, 320-1920 (default 1280)\n"
              << "  --height N        Height, 180-1080 (default 720)\n"
              << "  --seed N          Random seed (default 42)\n"
              << "  --screenshot FILE Write the final framebuffer as a PPM image\n"
              << "  --state FILE      Write final gameplay state as JSON\n"
              << "Capture runner only:\n"
              << "  --input-script FILE One direction (-1, 0, 1) per fixed 60 Hz frame\n"
              << "  --frame-directory DIR Write numbered PPM frames and gameplay JSON\n"
              << "  --capture-every N  Export every Nth frame (default 3, for 20 fps)\n";
}
