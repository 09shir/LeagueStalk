#pragma once
#include <CoreGraphics/CoreGraphics.h>
#include <vector>

CGSize GetResolution(CGDirectDisplayID id);
std::vector<CGSize> GetDisplaysResolutions();
