#include "assets/Caption.h"
#include <iostream>
#include <fstream>

namespace csci3081 {

Caption::Caption(const std::string& text, const std::string& fontPath,
                 unsigned int fontSize, const Color& textColor, double duration)
    : captionText(text),
      textColor(textColor),
      captionDuration(duration),
      textRenderer(nullptr),
      renderedImage(nullptr) {

    // Extract just the font filename from the path
    std::string fontFamily = fontPath;
    size_t lastSlash = fontPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        fontFamily = fontPath.substr(lastSlash + 1);
    }

    // Create Text object with the current project's API
    textRenderer = new Text(text, textColor, fontSize, fontFamily);
    regenerateImage();
}

Caption::~Caption() {
    delete textRenderer;
    delete renderedImage;
}

const Image& Caption::getFrame(double time) {
    // Caption is static, so we ignore time parameter
    // Could add effects based on time here if desired (fade in/out, etc.)
    return *renderedImage;
}

const Image& Caption::getThumbnail() {
    return *renderedImage;
}

bool Caption::isVideo() const {
    return false;
}

AssetType Caption::getAssetType() const {
    return AssetType::TEXT;
}

void Caption::setText(const std::string& text) {
    captionText = text;
    if (textRenderer) {
        textRenderer->setText(text);
    }
    regenerateImage();
}

void Caption::setTextColor(const Color& color) {
    textColor = color;
    if (textRenderer) {
        textRenderer->setColor(color);
    }
    regenerateImage();
}

void Caption::regenerateImage() {
    if (!textRenderer) {
        // Create a minimal fallback image
        if (renderedImage) {
            delete renderedImage;
        }
        renderedImage = new Image(10, 10);
        return;
    }

    // Generate the text image using the current project's API
    Image* newImage = textRenderer->renderToImage();

    // Store the new image
    if (renderedImage) {
        delete renderedImage;
    }
    renderedImage = newImage;
}

}
