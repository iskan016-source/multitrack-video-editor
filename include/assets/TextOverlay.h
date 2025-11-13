#ifndef ASSETS_TEXT_OVERLAY_H_
#define ASSETS_TEXT_OVERLAY_H_

#include "assets/IAsset.h"
#include "graphics/Text.h"

namespace csci3081 {

/**
 * @brief TextOverlay asset for displaying text in the video editor
 *
 * TextOverlay is an asset that renders text using the Text graphics primitive.
 * It can be used for captions, titles, labels, and other text overlays.
 */
class TextOverlay : public IAsset {
public:
  /**
   * @brief Create a text overlay asset
   * @param text The text content to display
   * @param color Text color (default white)
   * @param fontSize Font size in pixels (default 48)
   * @param fontFamily Font filename in assets/fonts/ (default "Roboto-Regular.ttf")
   */
  TextOverlay(const std::string& text,
              const Color& color = Color(255, 255, 255, 255),
              int fontSize = 48,
              const std::string& fontFamily = "Roboto-Regular.ttf");

  virtual ~TextOverlay();

  double getDuration() const override;
  const Image &getFrame(double time = 0.0) override;
  const Image &getThumbnail() override;
  bool isVideo() const override;
  AssetType getAssetType() const override;

private:
  Text *text;
  Image *renderedImage;
};

} // namespace csci3081

#endif
