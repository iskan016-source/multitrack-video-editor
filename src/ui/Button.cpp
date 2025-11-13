#include "ui/Button.h"
#include "graphics/ShaderProgram.h"
#include <glad/glad.h>

namespace csci3081 {

Button::Button(float x, float y, float w, float h, const Image &image,
               std::function<void()> callback, Glyph *overlayIcon)
    : Glyph(x, y, w, h, image,
            new ShaderProgram("src/graphics/shaders/quad.vsh",
                              "src/graphics/shaders/button.fsh")),
      onClickCallback(callback), overlay(overlayIcon) {}

Button::~Button() {
  if (overlay) {
    delete overlay;
  }
}

void Button::update(const Image &image) {
  Glyph::update(image);
}

bool Button::contains(float xpos, float ypos) const {
  return xpos >= x && xpos <= x + w && ypos >= y && ypos <= y + h;
}

void Button::onClick() {
  onClickCallback();
}

void Button::draw() const {
  const ShaderProgram &shaderProgram = getShaderProgram();
  shaderProgram.use();
  int buttonHighlightLoc =
      glGetUniformLocation(shaderProgram.getId(), "highlight");
  glUniform1i(buttonHighlightLoc, highlighted && !clicked);

  int buttonSelectedLoc =
      glGetUniformLocation(shaderProgram.getId(), "selected");
  glUniform1i(buttonSelectedLoc, selected);

  int buttonBorderLoc =
      glGetUniformLocation(shaderProgram.getId(), "border");
  glUniform1i(buttonBorderLoc, border);
  

  Glyph::draw();

  // Draw overlay icon if present
  if (overlay) {
    overlay->draw();
  }
}

} // namespace csci3081
