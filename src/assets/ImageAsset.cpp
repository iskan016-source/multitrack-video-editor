#include "assets/ImageAsset.h"

namespace csci3081 {

ImageAsset::ImageAsset(const std::string &filename) {
  image = new Image(filename);
}

ImageAsset::~ImageAsset() { delete image; }

double ImageAsset::getDuration() const { return 5.0; }  // Default 5 seconds for images

const Image &ImageAsset::getFrame(double time) { return *image; }

const Image &ImageAsset::getThumbnail() { return *image; }

bool ImageAsset::isVideo() const { return false; }

AssetType ImageAsset::getAssetType() const { return AssetType::IMAGE; }

} // namespace csci3081
