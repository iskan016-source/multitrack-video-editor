#ifndef VIDEO_ASSET_FACTORY_H_
#define VIDEO_ASSET_FACTORY_H_

#include "IAssetFactory.h"
#include "VideoAsset.h"

namespace csci3081 {

class VideoAssetFactory : public IAssetFactory {
public:
  IAsset *create(const std::string &value) const;
};

} // namespace csci3081

#endif
