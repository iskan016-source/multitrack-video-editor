#include "ui/export/ExportMenuModel.h"

namespace csci3081 {

ExportMenuModel::ExportMenuModel()
  : asset(nullptr), timeline(nullptr), filename("") {
  // Initialize with default settings
  settings.format = ExportFormat::PNG;
  settings.quality = 90;
  settings.width = -1;
  settings.height = -1;
  settings.frameRate = 30.0;
}

ExportMenuModel::~ExportMenuModel() {
  // Model does not own the asset, so don't delete it
}

std::vector<ExportFormat> ExportMenuModel::getAvailableFormats() const {
  std::vector<ExportFormat> formats;

  if (!asset) {
    return formats;
  }

  // All assets can be exported as images (we capture current frame)
  formats.push_back(ExportFormat::PNG);
  formats.push_back(ExportFormat::JPEG);
  formats.push_back(ExportFormat::BMP);

  // MP4 format available (renders all frames, but actual encoding not yet implemented)
  formats.push_back(ExportFormat::MP4);

  return formats;
}

bool ExportMenuModel::canExport() const {
  if (!asset) {
    return false;
  }

  if (filename.empty()) {
    return false;
  }

  // Check if filename matches format
  if (!ExportFacade::isValidFilenameForFormat(filename, settings.format)) {
    return false;
  }

  return true;
}

std::string ExportMenuModel::getValidationError() const {
  if (!asset) {
    return "No asset selected";
  }

  if (filename.empty()) {
    return "No filename specified";
  }

  if (!ExportFacade::isValidFilenameForFormat(filename, settings.format)) {
    return "Filename extension does not match format";
  }

  return "";
}

} // namespace csci3081
