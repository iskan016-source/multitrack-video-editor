#ifndef GRAPHICS_COLOR_RECT_H_
#define GRAPHICS_COLOR_RECT_H_

#include "graphics/Color.h"
#include "Image.h"

namespace csci3081 {

/**
 * @brief ColorRect generates a solid color rectangular image
 *
 * This is a utility class that creates an Image filled with a single color.
 * Useful for creating colored backgrounds, placeholders, or UI elements.
 */
class ColorRect {
public:
  /**
   * @brief Create a solid color rectangle image
   * @param width Width in pixels
   * @param height Height in pixels
   * @param color The fill color
   */
  ColorRect(int width, int height, const Color& color);

  /**
   * @brief Get the generated image
   * @return Reference to the color-filled image
   */
  const Image& getImage() const { return image; }

private:
  Image image;
};

} // namespace csci3081

#endif
