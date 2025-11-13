#ifndef UI_BUTTON_PANEL_H_
#define UI_BUTTON_PANEL_H_

#include "ui/Container.h"
#include "ui/Button.h"
#include "ui/TextButton.h"
#include "ui/IconButton.h"
#include <vector>
#include <string>
#include <functional>

namespace csci3081 {

/**
 * @brief Panel for organizing action buttons
 *
 * ButtonPanel provides a Container for organizing multiple buttons
 * in a vertical or horizontal layout. Useful for creating action menus
 * and toolbars.
 */
class ButtonPanel {
public:
  /**
   * @brief Layout direction for buttons
   */
  enum class LayoutDirection {
    VERTICAL,
    HORIZONTAL
  };

  /**
   * @brief Create a button panel
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param backgroundColor Background color
   * @param direction Layout direction
   */
  ButtonPanel(float x, float y, float w, float h,
              const Color& backgroundColor = Color(50, 50, 50, 255),
              LayoutDirection direction = LayoutDirection::VERTICAL);

  ~ButtonPanel();

  /**
   * @brief Add a text button to the panel
   * @param label Button text
   * @param callback Click callback
   * @param iconPath Optional icon path
   * @return Pointer to created button
   */
  Button* addTextButton(const std::string& label,
                        std::function<void()> callback,
                        const std::string& iconPath = "");

  /**
   * @brief Add a custom button to the panel
   * @param button Button to add (panel takes ownership)
   */
  void addButton(Button* button);

  /**
   * @brief Remove all buttons from the panel
   */
  void clearButtons();

  /**
   * @brief Draw the panel and all buttons
   */
  void draw() const;

  /**
   * @brief Get button at position
   * @param x X coordinate (normalized)
   * @param y Y coordinate (normalized)
   * @return Pointer to button, or nullptr if none found
   */
  Button* getButtonAt(float x, float y) const;

  /**
   * @brief Get all buttons in the panel
   * @return Vector of button pointers
   */
  const std::vector<Button*>& getButtons() const { return buttons; }

  /**
   * @brief Set button spacing
   * @param spacing Spacing between buttons (normalized)
   */
  void setButtonSpacing(float spacing) { this->buttonSpacing = spacing; }

  /**
   * @brief Set button size
   * @param width Button width (normalized), -1 to fill width
   * @param height Button height (normalized)
   */
  void setButtonSize(float width, float height) {
    this->buttonWidth = width;
    this->buttonHeight = height;
  }

  /**
   * @brief Update button layout after adding/removing buttons
   */
  void updateLayout();

private:
  Container* container;
  std::vector<Button*> buttons;

  float x, y, w, h;
  LayoutDirection direction;
  float buttonSpacing;
  float buttonWidth;
  float buttonHeight;
  float padding;

  /**
   * @brief Calculate button position based on index and layout
   * @param index Button index
   * @param outX Output X position
   * @param outY Output Y position
   * @param outW Output width
   * @param outH Output height
   */
  void calculateButtonPosition(size_t index, float& outX, float& outY, float& outW, float& outH) const;
};

} // namespace csci3081

#endif // UI_BUTTON_PANEL_H_
