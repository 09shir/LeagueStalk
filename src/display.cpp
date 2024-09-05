#include "../include/display.h"
#include <iostream>

CGSize GetResolution(CGDirectDisplayID id){

    int width = CGDisplayPixelsWide(id);
    int height = CGDisplayPixelsHigh(id);

    CGSize screenResolution = {static_cast<double>(width), static_cast<double>(height)};
    
    return screenResolution;
}

std::vector<CGSize> GetDisplaysResolutions(){

    UInt32 displayCount;
    CGGetOnlineDisplayList(0, nullptr, &displayCount);

    std::vector<CGDirectDisplayID> displaysList(displayCount);
    CGGetOnlineDisplayList(displayCount, displaysList.data(), &displayCount);

    std::vector<CGSize> displaysResolution;

    for (CGDirectDisplayID i : displaysList){
        displaysResolution.push_back(GetResolution(i));
    }

    // test
    for (int i = 0; i < displaysResolution.size(); i++){
        std::cout << "height of display " << i << " is " << displaysResolution[i].height << std::endl;
        std::cout << "width of display " << i << " is " << displaysResolution[i].width << std::endl;
    }

    return displaysResolution;
}