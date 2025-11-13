#ifndef IMAGE_ASSET_H_
#define IMAGE_ASSET_H_

#include "IAsset.h"
#include "Image.h"

namespace csci3081 {

class ImageAsset : public IAsset {
public:
  ImageAsset(const std::string &filename);
  ~ImageAsset();
  double getDuration() const;
  const Image &getFrame(double time = 0.0);
  const Image &getThumbnail();
  bool isVideo() const;
  AssetType getAssetType() const;

private:
  Image *image;
};

} // namespace csci3081

#endif
