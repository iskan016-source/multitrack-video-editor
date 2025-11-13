#include "assets/CompositeAssetFactory.h"

namespace csci3081 {

CompositeAssetFactory::~CompositeAssetFactory() {
  for (int i = 0; i < factories.size(); i++) {
    delete factories[i];
  }
}

void CompositeAssetFactory::add(IAssetFactory *factory) {
  factories.push_back(factory);
}

IAsset *CompositeAssetFactory::create(const std::string &value) const {
  for (int i = 0; i < factories.size(); i++) {
    IAsset *asset = factories[i]->create(value);
    if (asset != nullptr) {
      return asset;
    }
  }
  return nullptr;
}

} // namespace csci3081
