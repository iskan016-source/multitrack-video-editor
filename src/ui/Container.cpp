#include "ui/Container.h"

namespace csci3081 {

Container::Container(float x, float y, float w, float h, const Color& backgroundColor)
    : Glyph(x, y, w, h, ColorRect(100, 100, backgroundColor).getImage()),
      colorRect(new ColorRect(100, 100, backgroundColor)),
      ownsColorRect(true) {
  update(colorRect->getImage());
}

Container::Container(float x, float y, float w, float h, const Image& backgroundImage)
    : Glyph(x, y, w, h, backgroundImage),
      colorRect(nullptr),
      ownsColorRect(false) {
}

Container::~Container() {
  clearChildren();
  if (ownsColorRect && colorRect) {
    delete colorRect;
  }
}

void Container::addChild(Glyph* child) {
  if (child) {
    children.push_back(child);
  }
}

void Container::clearChildren() {
  for (Glyph* child : children) {
    delete child;
  }
  children.clear();
}

void Container::draw() const {
  // Draw container background
  Glyph::draw();

  // Draw all children
  for (const Glyph* child : children) {
    child->draw();
  }
}

} // namespace csci3081
