#ifndef EXPORT_MENU_MODEL_H_
#define EXPORT_MENU_MODEL_H_

#include "export/ExportFacade.h"
#include "assets/IAsset.h"
#include <string>
#include <vector>

namespace csci3081 {

/**
 * @brief Model for the export menu (MVC Pattern)
 *
 * ExportMenuModel holds the state and data for the export menu.
 * It manages the current asset to export, export settings, and
 * available export options.
 *
 * Design Pattern: Model-View-Controller (MVC)
 * - ExportMenuModel is the Model (data and business logic)
 * - ExportMenuView is the View (UI rendering)
 * - ExportMenuController is the Controller (user interaction handling)
 */
class ExportMenuModel {
public:
  ExportMenuModel();
  ~ExportMenuModel();

  /**
   * @brief Set the asset to be exported
   * @param asset The asset to export
   */
  void setAsset(IAsset* asset) { this->asset = asset; }

  /**
   * @brief Get the current asset
   * @return Pointer to current asset, or nullptr if none set
   */
  IAsset* getAsset() const { return asset; }

  /**
   * @brief Set the timeline to be exported (for video export)
   * @param timeline The timeline to export
   */
  void setTimeline(class Timeline* timeline) { this->timeline = timeline; }

  /**
   * @brief Get the timeline
   * @return Pointer to timeline, or nullptr if none set
   */
  class Timeline* getTimeline() const { return timeline; }

  /**
   * @brief Set export settings
   * @param settings The new export settings
   */
  void setSettings(const ExportSettings& settings) { this->settings = settings; }

  /**
   * @brief Get current export settings
   * @return Reference to current settings
   */
  const ExportSettings& getSettings() const { return settings; }
  ExportSettings& getSettings() { return settings; }

  /**
   * @brief Set the output filename
   * @param filename The output filename
   */
  void setFilename(const std::string& filename) { this->filename = filename; }

  /**
   * @brief Get the output filename
   * @return The output filename
   */
  const std::string& getFilename() const { return filename; }

  /**
   * @brief Get available export formats for current asset
   * @return Vector of available export formats
   */
  std::vector<ExportFormat> getAvailableFormats() const;

  /**
   * @brief Check if current configuration is valid for export
   * @return true if can export, false otherwise
   */
  bool canExport() const;

  /**
   * @brief Get validation error message if canExport() is false
   * @return Error message, or empty string if valid
   */
  std::string getValidationError() const;

private:
  IAsset* asset;
  class Timeline* timeline;
  ExportSettings settings;
  std::string filename;
};

} // namespace csci3081

#endif // EXPORT_MENU_MODEL_H_
