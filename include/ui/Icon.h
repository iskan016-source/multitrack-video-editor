#ifndef UI_ICON_H
#define UI_ICON_H

#include "graphics/Glyph.h"
#include "Image.h"
#include <string>

namespace csci3081 {

/**
 * @brief Icon class that extends Glyph to render icon images
 *
 * Icon wraps a Glyph and applies texture to a square region.
 * It can optionally invert colors for better visibility.
 */
class Icon : public Glyph {
public:
  /**
   * @brief Create an icon from a file path
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param iconPath Path to the icon image file
   * @param invert Whether to invert RGB colors (default true)
   */
  Icon(float x, float y, float w, float h, const std::string& iconPath, bool invert = true);

  /**
   * @brief Create an icon from an existing Image
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param image The image to use for the icon
   * @param invert Whether to invert RGB colors (default true)
   */
  Icon(float x, float y, float w, float h, const Image& image, bool invert = true);

  virtual ~Icon();

private:
  Image* iconImage;
  bool ownsImage;

  /**
   * @brief Inverts the RGB colors of an image while preserving alpha
   * @param img The image to invert
   * @return A new inverted image
   */
  static Image* invertColors(const Image& img);
};

} // namespace csci3081

#endif
