#include "assets/TextOverlay.h"

namespace csci3081 {

TextOverlay::TextOverlay(const std::string& textStr, const Color& color, int fontSize, const std::string& fontFamily) {
  text = new Text(textStr, color, fontSize, fontFamily);
  renderedImage = text->renderToImage();
}

TextOverlay::~TextOverlay() {
  delete text;
  delete renderedImage;
}

double TextOverlay::getDuration() const {
  return 5.0; // Default 5 seconds for text overlays (same as images)
}

const Image &TextOverlay::getFrame(double time) {
  return *renderedImage;
}

const Image &TextOverlay::getThumbnail() {
  return *renderedImage;
}

bool TextOverlay::isVideo() const {
  return false;
}

AssetType TextOverlay::getAssetType() const {
  return AssetType::TEXT;
}

}
