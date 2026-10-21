#include "InputScript.h"
#include <fstream>
#include <stdexcept>

std::vector<int> parseInputScript(std::istream& stream, int expectedFrames) {
    std::vector<int> directions;
    int direction;
    while (stream >> direction) {
        if (direction < -1 || direction > 1)
            throw std::invalid_argument("Input directions must be -1, 0 or 1");
        directions.push_back(direction);
    }
    if (!stream.eof() || static_cast<int>(directions.size()) != expectedFrames)
        throw std::invalid_argument("Input script must contain exactly one direction per frame");
    return directions;
}

std::vector<int> loadInputScript(const std::string& path, int expectedFrames) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Cannot read input script: " + path);
    return parseInputScript(file, expectedFrames);
}
