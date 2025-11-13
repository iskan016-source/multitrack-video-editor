#include "ui/IconButton.h"

namespace csci3081 {

IconButton::IconButton(float x, float y, float w, float h,
                       const Image& image,
                       std::function<void()> func,
                       const std::string& badgeIconPath,
                       float badgeSize)
    : Button(x, y, w, h, image, func, nullptr), badgeIcon(nullptr) {

  // Create badge icon in bottom-right corner if path provided
  if (!badgeIconPath.empty()) {
    float badgeX = x + w - badgeSize - 0.005f; // 0.005 padding from edge
    float badgeY = y + h - badgeSize - 0.005f;
    badgeIcon = new Icon(badgeX, badgeY, badgeSize, badgeSize, badgeIconPath, true);
    overlay = badgeIcon;
  }
}

IconButton::~IconButton() {
  // Badge icon is owned by overlay, which Button destructor handles
  badgeIcon = nullptr;
}

} // namespace csci3081
