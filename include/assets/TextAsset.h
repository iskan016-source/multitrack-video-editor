#ifndef TEXT_ASSET_H_
#define TEXT_ASSET_H_

#include "IAsset.h"
#include "graphics/Text.h"

namespace csci3081 {

class TextAsset : public IAsset {
public:
  TextAsset(const std::string& text,
            const Color& color = Color(255, 255, 255, 255),
            int fontSize = 48,
            const std::string& fontFamily = "Roboto-Regular.ttf");
  ~TextAsset();
  double getDuration() const;
  const Image &getFrame(double time = 0.0);
  const Image &getThumbnail();
  bool isVideo() const;
  AssetType getAssetType() const;

private:
  Text *text;
  Image *renderedImage;
};

}

#endif
