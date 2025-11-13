#include "ui/ButtonPanel.h"

namespace csci3081 {

ButtonPanel::ButtonPanel(float x, float y, float w, float h,
                         const Color& backgroundColor,
                         LayoutDirection direction)
  : x(x), y(y), w(w), h(h), direction(direction),
    buttonSpacing(0.01f), buttonWidth(-1.0f), buttonHeight(0.06f), padding(0.01f) {

  container = new Container(x, y, w, h, backgroundColor);
}

ButtonPanel::~ButtonPanel() {
  clearButtons();
  delete container;
}

Button* ButtonPanel::addTextButton(const std::string& label,
                                   std::function<void()> callback,
                                   const std::string& iconPath) {
  // Calculate button position
  float btnX, btnY, btnW, btnH;
  calculateButtonPosition(buttons.size(), btnX, btnY, btnW, btnH);

  TextButton* button = new TextButton(
    btnX, btnY, btnW, btnH,
    label,
    Color(255, 255, 255, 255),
    32,
    "Roboto-Regular.ttf",
    callback
  );

  buttons.push_back(button);
  return button;
}

void ButtonPanel::addButton(Button* button) {
  if (button) {
    buttons.push_back(button);
    updateLayout();
  }
}

void ButtonPanel::clearButtons() {
  for (Button* button : buttons) {
    delete button;
  }
  buttons.clear();
}

void ButtonPanel::draw() const {
  if (container) {
    container->draw();
  }

  for (const Button* button : buttons) {
    if (button) {
      button->draw();
    }
  }
}

Button* ButtonPanel::getButtonAt(float x, float y) const {
  for (Button* button : buttons) {
    if (button && button->contains(x, y)) {
      return button;
    }
  }
  return nullptr;
}

void ButtonPanel::updateLayout() {
  // Recalculate positions for all buttons
  for (size_t i = 0; i < buttons.size(); ++i) {
    float btnX, btnY, btnW, btnH;
    calculateButtonPosition(i, btnX, btnY, btnW, btnH);

    // Note: This would require Button to have setPosition/setSize methods
    // For now, buttons are positioned at creation time
  }
}

void ButtonPanel::calculateButtonPosition(size_t index, float& outX, float& outY, float& outW, float& outH) const {
  if (direction == LayoutDirection::VERTICAL) {
    // Vertical layout
    outX = x + padding;
    outW = (buttonWidth < 0) ? (w - 2 * padding) : buttonWidth;
    outH = buttonHeight;
    outY = y + padding + index * (buttonHeight + buttonSpacing);
  } else {
    // Horizontal layout
    outY = y + padding;
    outH = buttonHeight;
    outW = (buttonWidth < 0) ? ((w - 2 * padding - (buttons.size() - 1) * buttonSpacing) / buttons.size()) : buttonWidth;
    outX = x + padding + index * (outW + buttonSpacing);
  }
}

} // namespace csci3081
