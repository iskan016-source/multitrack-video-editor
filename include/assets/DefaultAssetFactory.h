#ifndef DEFAULT_ASSET_FACTORY_H_
#define DEFAULT_ASSET_FACTORY_H_

#include "IAssetFactory.h"

namespace csci3081 {

class DefaultAssetFactory : public IAssetFactory {
public:
  IAsset *create(const std::string &value) const;
};

} // namespace csci3081

#endif
