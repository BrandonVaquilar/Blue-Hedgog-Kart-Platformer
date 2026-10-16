#pragma once
#include <GLScene.h>
#include <string>
void captureFrame(const std::string& path, int width, int height, bool frontBuffer = false);
void saveState(const std::string& path, const GLScene::State& state, int frames);
