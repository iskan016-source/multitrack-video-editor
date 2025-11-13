#ifndef IASSET_FACTORY_H_
#define IASSET_FACTORY_H_

#include "IAsset.h"
#include <string>

namespace csci3081 {

class IAssetFactory {
public:
  virtual ~IAssetFactory() {}
  virtual IAsset *create(const std::string &value) const = 0;
};

} // namespace csci3081

#endif