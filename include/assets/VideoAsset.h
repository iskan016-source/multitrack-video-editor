#ifndef VIDEO_ASSET_H_
#define VIDEO_ASSET_H_

#include "IAsset.h"
#include "Video.h"

namespace csci3081 {

class VideoAsset : public IAsset {
public:
  VideoAsset(const std::string &filename);
  ~VideoAsset();
  double getDuration() const;
  const Image &getFrame(double time = 0.0);
  const Image &getThumbnail();
  bool isVideo() const;
  AssetType getAssetType() const;

private:
  Video *video;
  Image *thumbnail;
};

} // namespace csci3081

#endif
