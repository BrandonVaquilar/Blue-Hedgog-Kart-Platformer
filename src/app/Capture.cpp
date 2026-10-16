#include "Capture.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace {
std::ofstream output(const std::string& path) {
    const auto parent = std::filesystem::path(path).parent_path();
    if (!parent.empty()) std::filesystem::create_directories(parent);
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot write output: " + path);
    return file;
}
}

void captureFrame(const std::string& path, int width, int height, bool frontBuffer) {
    std::vector<unsigned char> pixels(static_cast<std::size_t>(width)*height*3);
    glReadBuffer(frontBuffer ? GL_FRONT : GL_BACK);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    if (glGetError() != GL_NO_ERROR) throw std::runtime_error("OpenGL framebuffer capture failed");
    auto file = output(path);
    file << "P6\n" << width << ' ' << height << "\n255\n";
    // OpenGL rows start at the bottom; PPM rows start at the top.
    for (int row = height-1; row >= 0; --row)
        file.write(reinterpret_cast<const char*>(pixels.data()+static_cast<std::size_t>(row)*width*3), width*3);
    if (!file) throw std::runtime_error("Could not finish framebuffer capture");
}

void saveState(const std::string& path, const GLScene::State& state, int frames) {
    auto file = output(path);
    file << "{\n  \"level\": " << state.level << ",\n  \"lives\": " << state.lives
         << ",\n  \"player_x\": " << state.playerX << ",\n  \"player_y\": " << state.playerY
         << ",\n  \"collisions\": " << state.collisions << ",\n  \"frames\": " << frames << "\n}\n";
    if (!file) throw std::runtime_error("Could not finish state output");
}
