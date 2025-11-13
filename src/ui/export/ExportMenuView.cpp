#include "ui/export/ExportMenuView.h"
#include "ui/export/ExportMenuController.h"
#include "graphics/Color.h"
#include <iostream>

namespace csci3081 {

ExportMenuView::ExportMenuView(float x, float y, float w, float h, ExportMenuModel* model)
  : model(model), controller(nullptr), x(x), y(y), w(w), h(h) {

  // Create container with dark background
  container = new Container(x, y, w, h, Color(40, 40, 40, 255));

  createUI();
}

ExportMenuView::~ExportMenuView() {
  clearUI();
  delete container;
}

void ExportMenuView::createUI() {
  clearUI();

  if (!model || !controller) {
    return;
  }

  // Create format selection buttons
  std::vector<ExportFormat> formats = model->getAvailableFormats();

  float buttonHeight = 0.04f;
  float buttonSpacing = 0.01f;
  float startY = y + 0.08f;  // Leave space for title

  for (size_t i = 0; i < formats.size(); ++i) {
    ExportFormat format = formats[i];
    std::string label;

    switch (format) {
      case ExportFormat::PNG:
        label = "PNG";
        break;
      case ExportFormat::JPEG:
        label = "JPEG";
        break;
      case ExportFormat::BMP:
        label = "BMP";
        break;
      case ExportFormat::MP4:
        label = "MP4";
        break;
      default:
        label = "Unknown";
        break;
    }

    float buttonY = startY + i * (buttonHeight + buttonSpacing);

    TextButton* button = new TextButton(
      x + 0.01f,
      buttonY,
      w - 0.02f,
      buttonHeight,
      label,
      Color(255, 255, 255, 255),
      32,
      "Roboto-Regular.ttf",
      [this, format]() {
        if (this->controller) {
          this->controller->onFormatSelected(format);
        }
      }
    );

    buttons.push_back(button);
  }

  // Create export buttons at bottom
  float exportVideoButtonY = y + h - 2 * buttonHeight - 2 * buttonSpacing - 0.01f;
  float exportButtonY = y + h - buttonHeight - 0.01f;

  // Export Video button (for composited timeline)
  TextButton* exportVideoButton = new TextButton(
    x + 0.01f,
    exportVideoButtonY,
    w - 0.02f,
    buttonHeight,
    "Export Video",
    Color(100, 200, 255, 255),  // Blue color
    32,
    "Roboto-Regular.ttf",
    [this]() {
      if (this->controller) {
        this->controller->onExportVideoClicked();
      }
    }
  );

  buttons.push_back(exportVideoButton);

  // Export button (for single asset/frame)
  TextButton* exportButton = new TextButton(
    x + 0.01f,
    exportButtonY,
    w - 0.02f,
    buttonHeight,
    "Export",
    Color(100, 255, 100, 255),  // Green color
    32,
    "Roboto-Regular.ttf",
    [this]() {
      if (this->controller) {
        this->controller->onExportClicked();
      }
    }
  );

  buttons.push_back(exportButton);
}

void ExportMenuView::clearUI() {
  for (Button* button : buttons) {
    delete button;
  }
  buttons.clear();
}

void ExportMenuView::draw() const {
  if (container) {
    container->draw();
  }

  for (const Button* button : buttons) {
    if (button) {
      button->draw();
    }
  }
}

void ExportMenuView::update() {
  // Recreate UI based on model changes
  createUI();
}

Button* ExportMenuView::getButtonAt(float x, float y) const {
  for (Button* button : buttons) {
    if (button && button->contains(x, y)) {
      return button;
    }
  }
  return nullptr;
}

} // namespace csci3081
