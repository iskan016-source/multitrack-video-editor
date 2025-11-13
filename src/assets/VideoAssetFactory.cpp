#include "assets/VideoAssetFactory.h"
#include <algorithm>

namespace csci3081 {

IAsset *VideoAssetFactory::create(const std::string &value) const {
  // Check if the string ends with a video extension
  std::string lower = value;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  if (lower.find(".mp4") != std::string::npos) {
    return new VideoAsset(value);
  }

  return nullptr;
}

} // namespace csci3081
