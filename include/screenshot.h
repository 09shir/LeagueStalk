#pragma once
#include <CoreGraphics/CoreGraphics.h>
#include <filesystem>

const std::pair<float, float> playerIdTopLeftCoordinates[10] = {
    {245, 490}, {541, 490}, {835, 490}, {1130, 490}, {1430, 490},
    {245, 1013}, {541, 1013}, {835, 1013}, {1130, 1013}, {1430, 1013}
};
const float playerIdWidth = 238;
const float playerIdHeight = 20;

CGImageRef TrimImage(CGImageRef originalImage, float x, float y, float width, float height);
void SaveScreenshot();
