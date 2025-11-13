#ifndef TEXT_ASSET_FACTORY_H_
#define TEXT_ASSET_FACTORY_H_

#include "IAssetFactory.h"
#include "TextAsset.h"

namespace csci3081 {

class TextAssetFactory : public IAssetFactory {
public:
  IAsset* create(const std::string& value) const;
};

}

#endif
