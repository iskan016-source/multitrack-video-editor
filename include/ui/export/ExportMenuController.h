#ifndef EXPORT_MENU_CONTROLLER_H_
#define EXPORT_MENU_CONTROLLER_H_

#include "ui/export/ExportMenuModel.h"
#include "ui/export/ExportMenuView.h"
#include "export/ExportFacade.h"
#include "commands/ICommand.h"
#include <string>

namespace csci3081 {

// Forward declaration
class ExportMenuView;

/**
 * @brief Controller for the export menu (MVC Pattern)
 *
 * ExportMenuController handles user interactions with the export menu
 * and coordinates between the model and view. It creates and executes
 * commands to perform export operations.
 *
 * Design Pattern: Model-View-Controller (MVC)
 * - ExportMenuModel is the Model (data and business logic)
 * - ExportMenuView is the View (UI rendering)
 * - ExportMenuController is the Controller (user interaction handling)
 *
 * The controller also uses the Command pattern to execute export operations.
 */
class ExportMenuController {
public:
  /**
   * @brief Create an export menu controller
   * @param model The model to control
   * @param view The view to update
   * @param facade The export facade for operations
   */
  ExportMenuController(ExportMenuModel* model, ExportMenuView* view, ExportFacade* facade);

  ~ExportMenuController();

  /**
   * @brief Handle format button click
   * @param format The format to select
   */
  void onFormatSelected(ExportFormat format);

  /**
   * @brief Handle export button click
   */
  void onExportClicked();

  /**
   * @brief Handle export video button click (exports composited timeline)
   */
  void onExportVideoClicked();

  /**
   * @brief Set the filename for export
   * @param filename The filename to use
   */
  void setFilename(const std::string& filename);

  /**
   * @brief Handle mouse move event
   * @param x X coordinate (normalized)
   * @param y Y coordinate (normalized)
   */
  void onMouseMove(float x, float y);

  /**
   * @brief Handle mouse click event
   * @param button Mouse button
   * @param action Press/release
   * @param mods Modifier keys
   */
  void onMouseClick(int button, int action, int mods);

private:
  ExportMenuModel* model;
  ExportMenuView* view;
  ExportFacade* facade;

  /**
   * @brief Execute a command
   * @param command The command to execute
   * @return true if successful, false otherwise
   */
  bool executeCommand(ICommand* command);
};

} // namespace csci3081

#endif // EXPORT_MENU_CONTROLLER_H_
