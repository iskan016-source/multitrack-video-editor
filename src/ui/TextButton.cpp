#include "ui/TextButton.h"

namespace csci3081 {

TextButton::TextButton(float x, float y, float w, float h,
                       const std::string& text,
                       const Color& color,
                       int fontSize,
                       const std::string& fontFamily,
                       std::function<void()> func)
    : Button(x, y, w, h, Image(10, 10), func, nullptr) {
  textRenderer = new Text(text, color, fontSize, fontFamily);
  textImage = textRenderer->renderToImage();
  update(*textImage);
}

TextButton::~TextButton() {
  delete textRenderer;
  delete textImage;
}

} // namespace csci3081
