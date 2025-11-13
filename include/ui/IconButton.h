#ifndef UI_ICON_BUTTON_H
#define UI_ICON_BUTTON_H

#include "ui/Button.h"
#include "ui/Icon.h"
#include <string>

namespace csci3081 {

/**
 * @brief Button that displays an icon with optional badge overlay
 *
 * IconButton extends Button to display icon images with automatic
 * color inversion and optional badge overlay in the corner.
 */
class IconButton : public Button {
public:
  /**
   * @brief Create an icon button with optional badge
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param image The main button image
   * @param func Callback function when clicked
   * @param badgeIconPath Optional path to badge icon (shown in corner)
   * @param badgeSize Size of badge (normalized, default 0.025)
   */
  IconButton(float x, float y, float w, float h,
             const Image& image,
             std::function<void()> func = []() {},
             const std::string& badgeIconPath = "",
             float badgeSize = 0.025f);

  virtual ~IconButton();

private:
  Icon* badgeIcon;
};

} // namespace csci3081

#endif
