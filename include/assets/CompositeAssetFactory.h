#ifndef COMPOSITE_ASSET_FACTORY_H_
#define COMPOSITE_ASSET_FACTORY_H_

#include "IAssetFactory.h"
#include <vector>

namespace csci3081 {

class CompositeAssetFactory : public IAssetFactory {
public:
  ~CompositeAssetFactory();
  void add(IAssetFactory *factory);
  IAsset *create(const std::string &value) const;

private:
  std::vector<IAssetFactory *> factories;
};

} // namespace csci3081

#endif
