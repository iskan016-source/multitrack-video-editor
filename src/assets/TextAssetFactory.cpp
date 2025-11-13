#include "assets/TextAssetFactory.h"
#include "assets/TextOverlay.h"
#include <sstream>
#include <vector>

namespace csci3081 {

IAsset* TextAssetFactory::create(const std::string& value) const {
  // Format: "text:Hello World"
  //     or: "text:Hello World:255,0,0"
  //     or: "text:Hello World:255,0,0:48"
  //     or: "text:Hello World:255,0,0:48:Roboto-Regular.ttf"
  if (value.find("text:") != 0) {
    return nullptr;
  }

  std::string remaining = value.substr(5); // Remove "text:" prefix

  // Split by colons
  std::vector<std::string> parts;
  size_t pos = 0;
  while ((pos = remaining.find(':')) != std::string::npos) {
    parts.push_back(remaining.substr(0, pos));
    remaining = remaining.substr(pos + 1);
  }
  if (!remaining.empty()) {
    parts.push_back(remaining);
  }

  if (parts.empty() || parts[0].empty()) {
    return nullptr;
  }

  // Parse components
  std::string text = parts[0];
  Color color(255, 255, 255, 255); // Default white
  int fontSize = 48; // Default size
  std::string fontFamily = "Roboto-Regular.ttf"; // Default font

  // Parse color if present (format: R,G,B)
  if (parts.size() >= 2) {
    std::stringstream ss(parts[1]);
    int r, g, b;
    char comma1, comma2;
    if (ss >> r >> comma1 >> g >> comma2 >> b && comma1 == ',' && comma2 == ',') {
      color = Color(r, g, b, 255);
    }
  }

  // Parse fontSize if present
  if (parts.size() >= 3) {
    try {
      fontSize = std::stoi(parts[2]);
    } catch (...) {
      // Keep default fontSize if parsing fails
    }
  }

  // Parse fontFamily if present
  if (parts.size() >= 4) {
    fontFamily = parts[3];
  }

  return new TextOverlay(text, color, fontSize, fontFamily);
}

}
