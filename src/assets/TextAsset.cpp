#include "assets/TextAsset.h"

namespace csci3081 {

TextAsset::TextAsset(const std::string& textStr, const Color& color, int fontSize, const std::string& fontFamily) {
  text = new Text(textStr, color, fontSize, fontFamily);
  renderedImage = text->renderToImage();
}

TextAsset::~TextAsset() {
  delete text;
  delete renderedImage;
}

double TextAsset::getDuration() const {
  return 0.0; // Text is static like images
}

const Image &TextAsset::getFrame(double time) {
  return *renderedImage;
}

const Image &TextAsset::getThumbnail() {
  return *renderedImage;
}

bool TextAsset::isVideo() const {
  return false;
}

AssetType TextAsset::getAssetType() const {
  return AssetType::TEXT;
}

}
