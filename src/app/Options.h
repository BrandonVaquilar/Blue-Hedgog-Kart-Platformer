#pragma once
#include <string>

struct Options {
    int width = 1280, height = 720;
    int level = 0, frames = 0;
    unsigned int seed = 42;
    bool demo = false, hidden = false, help = false;
    std::string screenshot, state;
    std::string inputScript, frameDirectory;
    int captureEvery = 3;
};
Options parseOptions(int argc, char** argv);
void printHelp();
