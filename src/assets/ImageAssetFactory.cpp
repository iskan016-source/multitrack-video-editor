#include "assets/ImageAssetFactory.h"
#include <algorithm>

namespace csci3081 {

IAsset *ImageAssetFactory::create(const std::string &value) const {
  // Check if the string ends with an image extension
  std::string lower = value;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  if (lower.find(".png") != std::string::npos ||
      lower.find(".jpg") != std::string::npos ||
      lower.find(".jpeg") != std::string::npos ||
      lower.find(".gif") != std::string::npos ||
      lower.find(".bmp") != std::string::npos ||
      lower.find(".ppm") != std::string::npos) {
    return new ImageAsset(value);
  }

  return nullptr;
}

} // namespace csci3081
