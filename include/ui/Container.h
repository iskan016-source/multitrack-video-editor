#ifndef UI_CONTAINER_H_
#define UI_CONTAINER_H_

#include "graphics/Glyph.h"
#include "graphics/Color.h"
#include "graphics/ColorRect.h"
#include <vector>

namespace csci3081 {

/**
 * @brief Container class for managing layout and grouping of UI elements
 *
 * Container provides a way to group multiple Glyph objects and manage
 * their positioning relative to a container bounds. Useful for creating
 * buttons with backgrounds, panels, and organized layouts.
 */
class Container : public Glyph {
public:
  /**
   * @brief Create a container with a solid color background
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param backgroundColor Background color
   */
  Container(float x, float y, float w, float h, const Color& backgroundColor);

  /**
   * @brief Create a container with an image background
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param backgroundImage Background image
   */
  Container(float x, float y, float w, float h, const Image& backgroundImage);

  virtual ~Container();

  /**
   * @brief Add a child glyph to the container
   * @param child Pointer to glyph to add (Container takes ownership)
   */
  void addChild(Glyph* child);

  /**
   * @brief Remove all children from the container
   */
  void clearChildren();

  /**
   * @brief Draw the container and all its children
   */
  virtual void draw() const override;

  /**
   * @brief Get the container bounds
   */
  float getX() const { return x; }
  float getY() const { return y; }
  float getWidth() const { return w; }
  float getHeight() const { return h; }

protected:
  std::vector<Glyph*> children;
  ColorRect* colorRect;
  bool ownsColorRect;
};

} // namespace csci3081

#endif
