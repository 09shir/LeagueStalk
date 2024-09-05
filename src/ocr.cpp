#include "../include/ocr.h"
#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

std::string performOCR(const std::string& imagePath) {
    tesseract::TessBaseAPI ocr;
    
    // Initialize Tesseract to use English (eng) and the LSTM OCR engine.
    if (ocr.Init(nullptr, "eng")) {
        std::cerr << "Could not initialize tesseract.\n";
        return "";
    }
    
    // Open the JPEG image file.
    Pix* image = pixRead(imagePath.c_str());
    if (!image) {
        std::cerr << "Cannot open input file: " << imagePath << std::endl;
        return "";
    }

    // Set the image for Tesseract.
    ocr.SetImage(image);

    // Get OCR result
    std::unique_ptr<char[]> outText(ocr.GetUTF8Text());
    std::string result = outText.get();

    // Clean up
    pixDestroy(&image);
    ocr.End();

    // Remove whitespace
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace),
        result.end());

    return result;
}

std::vector<std::string> extractUsernames(){

    std::vector<std::string> extractedUsernames;
    std::string imagePath = "./resources/";
    std::string imagePathSuffix = ".jpg";

    for (int i = 1; i < 11; i++){
        std::string imageFullPath = imagePath + std::to_string(i) + imagePathSuffix;
        std::string extractedUsername = performOCR(imageFullPath);
        extractedUsernames.push_back(extractedUsername);
    }
    return extractedUsernames;
}