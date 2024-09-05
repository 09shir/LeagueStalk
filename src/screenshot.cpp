#include "../include/screenshot.h"
#include <ImageIO/ImageIO.h>
#include <iostream>

CGImageRef TrimImage(CGImageRef originalImage, float x, float y, float width, float height) {
    CGRect rect = CGRectMake(x, y, width, height);
    CGImageRef trimmedImage = CGImageCreateWithImageInRect(originalImage, rect);
    return trimmedImage;
}

void SaveScreenshot(){
    CGImageRef screenShot_preTrim = CGWindowListCreateImage( CGRectInfinite, kCGWindowListOptionOnScreenOnly, kCGNullWindowID, kCGWindowImageDefault);

    std::filesystem::path resourcesDir = std::filesystem::current_path() / "resources";

    const char* fp = "../resources/";
    const char* fp_suffix = ".jpg";
    int counter = 1;

    for (std::pair<float, float> p : playerIdTopLeftCoordinates){

        std::string fp_suffix_with_filename = std::to_string(counter) + fp_suffix;

        std::filesystem::path filePath = resourcesDir / fp_suffix_with_filename;
        std::string fullPath_str = filePath.string();

        const char* fullPath = fullPath_str.c_str();

        std::cout << fullPath << std::endl;

        counter++;

        CGImageRef screenShot = TrimImage(screenShot_preTrim, p.first, p.second, playerIdWidth, playerIdHeight);
        CFStringRef file = CFStringCreateWithCString(NULL, fullPath, kCFStringEncodingUTF8);
        CFStringRef type = CFSTR("public.jpeg");
        CFURLRef urlRef = CFURLCreateWithFileSystemPath( kCFAllocatorDefault, file, kCFURLPOSIXPathStyle, false );
        CGImageDestinationRef image_destination = CGImageDestinationCreateWithURL( urlRef, type, 1, NULL );
        CGImageDestinationAddImage( image_destination, screenShot, NULL );
        // CGImageDestinationFinalize( image_destination );
        Boolean success = CGImageDestinationFinalize(image_destination);
        if (!success) {
            std::cerr << "Failed to write image to disk." << std::endl;
        }
    }

}