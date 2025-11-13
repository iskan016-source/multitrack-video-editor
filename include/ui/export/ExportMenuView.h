#ifndef EXPORT_MENU_VIEW_H_
#define EXPORT_MENU_VIEW_H_

#include "ui/export/ExportMenuModel.h"
#include "ui/Container.h"
#include "ui/TextButton.h"
#include <vector>

namespace csci3081 {

// Forward declaration
class ExportMenuController;

/**
 * @brief View for the export menu (MVC Pattern)
 *
 * ExportMenuView renders the export menu UI and delegates user
 * interactions to the controller.
 *
 * Design Pattern: Model-View-Controller (MVC)
 * - ExportMenuModel is the Model (data and business logic)
 * - ExportMenuView is the View (UI rendering)
 * - ExportMenuController is the Controller (user interaction handling)
 *
 * The view contains buttons for:
 * - Selecting export format (PNG, JPEG, BMP)
 * - Exporting the current asset
 */
class ExportMenuView {
public:
  /**
   * @brief Create an export menu view
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param model The model to display
   */
  ExportMenuView(float x, float y, float w, float h, ExportMenuModel* model);

  ~ExportMenuView();

  /**
   * @brief Set the controller for this view
   * @param controller The controller to use
   */
  void setController(ExportMenuController* controller) {
    this->controller = controller;
  }

  /**
   * @brief Draw the export menu
   */
  void draw() const;

  /**
   * @brief Update the view based on model changes
   */
  void update();

  /**
   * @brief Check if a point is inside any button
   * @param x X coordinate (normalized)
   * @param y Y coordinate (normalized)
   * @return Pointer to button containing point, or nullptr
   */
  Button* getButtonAt(float x, float y) const;

  /**
   * @brief Get all buttons in the view
   * @return Vector of button pointers
   */
  const std::vector<Button*>& getButtons() const { return buttons; }

private:
  ExportMenuModel* model;
  ExportMenuController* controller;
  Container* container;
  std::vector<Button*> buttons;

  float x, y, w, h;

  /**
   * @brief Create the UI elements
   */
  void createUI();

  /**
   * @brief Clear all UI elements
   */
  void clearUI();
};

} // namespace csci3081

#endif // EXPORT_MENU_VIEW_H_
