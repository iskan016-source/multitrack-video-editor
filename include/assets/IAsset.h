#ifndef IASSET_H_
#define IASSET_H_

#include "Image.h"
#include "filters/IFilter.h"

namespace csci3081 {

enum class AssetType {
  IMAGE,
  VIDEO,
  TEXT,
  DEFAULT
};

class IAsset {
public:
  virtual ~IAsset() {}
  virtual double getDuration() const = 0;
  virtual const Image &getFrame(double time = 0.0) = 0;
  virtual const Image &getThumbnail() = 0;
  virtual bool isVideo() const = 0;
  virtual AssetType getAssetType() const = 0;
};

} // namespace csci3081

#endif