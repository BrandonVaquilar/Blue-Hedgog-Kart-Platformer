#pragma once
#include <istream>
#include <string>
#include <vector>

// Capture inputs use the same -1/0/1 directions as real keyboard input.
std::vector<int> parseInputScript(std::istream& stream, int expectedFrames);
std::vector<int> loadInputScript(const std::string& path, int expectedFrames);
