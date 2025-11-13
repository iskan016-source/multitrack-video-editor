#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "graphics/Glyph.h"
#include <functional>

namespace csci3081 {

/**
 * @brief Base button class that extends Glyph with interactive functionality
 *
 * Button provides click handling, highlighting, and overlay support.
 */
class Button : public Glyph {
public:
  /**
   * @brief Create a button with an image
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param image The button's image
   * @param func Callback function when clicked
   * @param overlay Optional overlay glyph (e.g., badge icon)
   */
  Button(float x, float y, float w, float h, const Image &image,
         std::function<void()> func = []() {}, Glyph *overlay = nullptr);

  virtual ~Button();

  void update(const Image &image);
  virtual void draw() const;
  bool contains(float x, float y) const;
  void setHighlighted(bool highlighted) { this->highlighted = highlighted; }
  void setClicked(bool clicked) { this->clicked = clicked; }
  void setSelected(bool selected) { this->selected = selected; }
  void setBorder(bool border) { this->border = border; }
  bool isHighlighted() const { return highlighted; }
  bool isSelected() const { return selected; }
  virtual void onClick();

protected:
  bool highlighted = false;
  bool clicked = false;
  bool selected = false; 
  bool border = true;
  std::function<void()> onClickCallback;
  Glyph *overlay;
};

} // namespace csci3081

#endif
