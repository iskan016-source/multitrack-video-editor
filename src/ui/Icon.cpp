#include "ui/Icon.h"

namespace csci3081 {

// Helper to create inverted or normal image from path
static Image createProcessedImage(const std::string& path, bool invert) {
  Image img(path);
  if (!invert) return img;

  // Invert colors
  for (int y = 0; y < img.getHeight(); y++) {
    for (int x = 0; x < img.getWidth(); x++) {
      Color c = img.getPixel(x, y);
      c.setRed(255 - c.red());
      c.setGreen(255 - c.green());
      c.setBlue(255 - c.blue());
      img.setPixel(x, y, c);
    }
  }
  return img;
}

// Helper to create inverted or normal image from existing image
static Image createProcessedImage(const Image& original, bool invert) {
  Image img(original);
  if (!invert) return img;

  // Invert colors
  for (int y = 0; y < img.getHeight(); y++) {
    for (int x = 0; x < img.getWidth(); x++) {
      Color c = img.getPixel(x, y);
      c.setRed(255 - c.red());
      c.setGreen(255 - c.green());
      c.setBlue(255 - c.blue());
      img.setPixel(x, y, c);
    }
  }
  return img;
}

Icon::Icon(float x, float y, float w, float h, const std::string& iconPath, bool invert)
    : Glyph(x, y, w, h, createProcessedImage(iconPath, invert)),
      iconImage(nullptr), ownsImage(false) {
}

Icon::Icon(float x, float y, float w, float h, const Image& image, bool invert)
    : Glyph(x, y, w, h, createProcessedImage(image, invert)),
      iconImage(nullptr), ownsImage(false) {
}

Icon::~Icon() {
  if (ownsImage && iconImage) {
    delete iconImage;
  }
}

Image* Icon::invertColors(const Image& img) {
  Image* inverted = new Image(img);

  // Invert RGB colors while keeping alpha
  for (int y = 0; y < inverted->getHeight(); y++) {
    for (int x = 0; x < inverted->getWidth(); x++) {
      Color c = inverted->getPixel(x, y);
      c.setRed(255 - c.red());
      c.setGreen(255 - c.green());
      c.setBlue(255 - c.blue());
      inverted->setPixel(x, y, c);
    }
  }

  return inverted;
}

} // namespace csci3081
