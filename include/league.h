#pragma once
#include <iostream>
#include <future>
#include <thread>
#include "riot_api.h"
#include "ocr.h"
#include "screenshot.h"
#include "display.h"

std::vector<std::vector<UserDetails>> getPlayerInfo();