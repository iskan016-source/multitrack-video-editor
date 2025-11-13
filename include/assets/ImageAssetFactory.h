#ifndef IMAGE_ASSET_FACTORY_H_
#define IMAGE_ASSET_FACTORY_H_

#include "IAssetFactory.h"
#include "ImageAsset.h"

namespace csci3081 {

class ImageAssetFactory : public IAssetFactory {
public:
  IAsset *create(const std::string &value) const;
};

} // namespace csci3081

#endif
