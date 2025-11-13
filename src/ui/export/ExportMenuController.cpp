#include "ui/export/ExportMenuController.h"
#include "commands/ExportAssetCommand.h"
#include "timeline/Timeline.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace csci3081 {

ExportMenuController::ExportMenuController(ExportMenuModel* model, ExportMenuView* view, ExportFacade* facade)
  : model(model), view(view), facade(facade) {
}

ExportMenuController::~ExportMenuController() {
}

void ExportMenuController::onFormatSelected(ExportFormat format) {
  if (!model) {
    return;
  }

  // Update model with selected format
  ExportSettings settings = model->getSettings();
  settings.format = format;
  model->setSettings(settings);

  // Update filename extension if needed
  std::string filename = model->getFilename();
  if (!filename.empty()) {
    // Find last dot
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != std::string::npos) {
      // Replace extension
      std::string base = filename.substr(0, dotPos);
      std::string newExt = ExportFacade::getDefaultExtension(format);
      model->setFilename(base + newExt);
    } else {
      // Add extension
      std::string newExt = ExportFacade::getDefaultExtension(format);
      model->setFilename(filename + newExt);
    }
  }

  std::cout << "Format selected: " << static_cast<int>(format) << std::endl;

  // Update view
  if (view) {
    view->update();
  }
}

void ExportMenuController::onExportClicked() {
  if (!model || !facade) {
    std::cerr << "Error: Model or facade is null" << std::endl;
    return;
  }

  // Validate
  if (!model->canExport()) {
    std::cerr << "Cannot export: " << model->getValidationError() << std::endl;
    return;
  }

  // Create and execute export command
  ExportAssetCommand* command = new ExportAssetCommand(
    facade,
    model->getAsset(),
    model->getFilename(),
    model->getSettings()
  );

  bool success = executeCommand(command);

  if (success) {
    std::cout << "Export successful: " << model->getFilename() << std::endl;
  } else {
    std::cerr << "Export failed: " << facade->getLastError() << std::endl;
  }

  delete command;
}

void ExportMenuController::onExportVideoClicked() {
  std::cout << "=== Export Video Button Clicked ===" << std::endl;

  if (!model || !facade) {
    std::cerr << "ERROR: Model or facade is null" << std::endl;
    std::cerr << "  model: " << (model ? "valid" : "NULL") << std::endl;
    std::cerr << "  facade: " << (facade ? "valid" : "NULL") << std::endl;
    return;
  }

  // Get timeline from model
  Timeline* timeline = model->getTimeline();
  std::cout << "Timeline pointer: " << timeline << std::endl;

  if (!timeline) {
    std::cerr << "ERROR: Cannot export video - No timeline set in model" << std::endl;
    return;
  }

  double duration = timeline->getTotalDuration();
  std::cout << "Timeline duration: " << duration << " seconds" << std::endl;
  std::cout << "Timeline tracks: " << timeline->getTrackCount() << std::endl;

  if (duration <= 0.0) {
    std::cerr << "ERROR: Cannot export video - Timeline is empty (duration = 0)" << std::endl;
    return;
  }

  // Get current settings and potentially modify for video export
  ExportSettings settings = model->getSettings();
  std::cout << "Original export format: " << static_cast<int>(settings.format) << std::endl;

  // For video export, use a video-friendly default filename
  std::string filename = "timeline_export";

  // Use the current format's extension
  std::string ext = ExportFacade::getDefaultExtension(settings.format);
  std::cout << "Extension for format: '" << ext << "'" << std::endl;

  filename += ext;

  // Warn if not using a video format
  if (settings.format != ExportFormat::MP4) {
    std::cout << "NOTE: Exporting timeline as single frame " << ext << " (video format not selected)" << std::endl;
    std::cout << "      To export as video, MP4 support needs to be implemented" << std::endl;
  }

  std::cout << "Final output filename: '" << filename << "'" << std::endl;

  // Default video dimensions (can be made configurable)
  int width = 640;
  int height = 480;
  std::cout << "Output dimensions: " << width << "x" << height << std::endl;

  // Export the timeline
  std::cout << "Calling facade->exportTimeline()..." << std::endl;
  bool success = facade->exportTimeline(timeline, filename, settings, width, height);

  if (success) {
    std::cout << "SUCCESS: Timeline export completed: " << filename << std::endl;
  } else {
    std::cerr << "FAILED: Timeline export failed!" << std::endl;
    std::cerr << "Error: " << facade->getLastError() << std::endl;
  }

  std::cout << "=== Export Video Complete ===" << std::endl;
}

void ExportMenuController::setFilename(const std::string& filename) {
  if (model) {
    model->setFilename(filename);
  }
}

void ExportMenuController::onMouseMove(float x, float y) {
  if (!view) {
    return;
  }

  // Update button highlighting
  for (Button* button : view->getButtons()) {
    if (button) {
      if (button->contains(x, y)) {
        button->setHighlighted(true);
      } else {
        button->setHighlighted(false);
      }
    }
  }
}

void ExportMenuController::onMouseClick(int button, int action, int mods) {
  if (!view) {
    return;
  }

  // Handle button clicks
  for (Button* btn : view->getButtons()) {
    if (btn) {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (btn->isHighlighted()) {
          btn->setClicked(true);
          btn->onClick();
        }
      }
      if (action == GLFW_RELEASE) {
        btn->setClicked(false);
      }
    }
  }
}

bool ExportMenuController::executeCommand(ICommand* command) {
  if (!command) {
    return false;
  }

  if (!command->canExecute()) {
    std::cerr << "Command cannot execute" << std::endl;
    return false;
  }

  try {
    command->execute();
    return true;
  } catch (const std::exception& e) {
    std::cerr << "Command execution failed: " << e.what() << std::endl;
    return false;
  }
}

} // namespace csci3081
