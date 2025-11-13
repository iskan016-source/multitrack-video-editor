#ifndef CAPTION_ASSET_FACTORY_H_
#define CAPTION_ASSET_FACTORY_H_

#include "IAssetFactory.h"
#include "Caption.h"

namespace csci3081 {

class CaptionAssetFactory : public IAssetFactory {
public:
  IAsset *create(const std::string &value) const;
};

} // namespace csci3081

#endif
