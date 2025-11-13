#ifndef IMAGE_ASSET_FACTORY_H_
#define IMAGE_ASSET_FACTORY_H_

#include "../lib/stb_image.h"
#include "AssetFactory.h"
#include "ImageAsset.h"

namespace csci3081 {
class ImageAssetFactory : AssetFactory {
public:
  ImageAsset create(std::string);

private:
};
} // namespace csci3081

#endif