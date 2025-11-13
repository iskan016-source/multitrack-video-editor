#include "assets/CaptionAssetFactory.h"
#include "graphics/Color.h"
#include <algorithm>
#include <fstream>
#include <sstream>

namespace csci3081 {

IAsset *CaptionAssetFactory::create(const std::string &value) const {
  // Check if the string ends with a text extension
  std::string lower = value;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  if (lower.find(".txt") != std::string::npos) {
    // Read the text content from the file
    std::ifstream file(value);
    if (!file.is_open()) {
      return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();
    file.close();

    // Create Caption with default font, size, and color
    return new Caption(text,
                      "assets/fonts/GoogleSansCode-Regular.ttf",
                      48,
                      Color(255, 255, 255, 255)); // white text
  }

  return nullptr;
}

} // namespace csci3081
