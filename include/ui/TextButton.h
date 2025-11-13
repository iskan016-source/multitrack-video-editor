#ifndef UI_TEXT_BUTTON_H
#define UI_TEXT_BUTTON_H

#include "ui/Button.h"
#include "graphics/Text.h"
#include "graphics/Color.h"
#include <string>

namespace csci3081 {

/**
 * @brief Button that displays text
 *
 * TextButton extends Button to render text as its content.
 */
class TextButton : public Button {
public:
  /**
   * @brief Create a text button
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param text The text to display
   * @param color Text color
   * @param fontSize Font size in pixels
   * @param fontFamily Font filename (in assets/fonts/)
   * @param func Callback function when clicked
   */
  TextButton(float x, float y, float w, float h,
             const std::string& text,
             const Color& color = Color(255, 255, 255, 255),
             int fontSize = 48,
             const std::string& fontFamily = "Roboto-Regular.ttf",
             std::function<void()> func = []() {});

  virtual ~TextButton();

private:
  Text* textRenderer;
  Image* textImage;
};

} // namespace csci3081

#endif
