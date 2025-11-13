#include "assets/VideoAsset.h"

namespace csci3081 {

VideoAsset::VideoAsset(const std::string &filename) {
  video = new Video(filename);
  // Create thumbnail from the first frame
  thumbnail = new Image(video->getFrame());
}

VideoAsset::~VideoAsset() {
  delete video;
  delete thumbnail;
}

double VideoAsset::getDuration() const { return video->getDuration(); }

const Image &VideoAsset::getFrame(double time) {
  video->nextFrame(time);
  return video->getFrame();
}

const Image &VideoAsset::getThumbnail() {
  return *thumbnail;
}

bool VideoAsset::isVideo() const { return true; }

AssetType VideoAsset::getAssetType() const { return AssetType::VIDEO; }

} // namespace csci3081
