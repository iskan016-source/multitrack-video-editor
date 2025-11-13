#include "graphics/ColorRect.h"

namespace csci3081 {

ColorRect::ColorRect(int width, int height, const Color& color)
    : image(width, height) {
  // Fill the entire image with the specified color
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      image.setPixel(x, y, color);
    }
  }
}

} // namespace csci3081
