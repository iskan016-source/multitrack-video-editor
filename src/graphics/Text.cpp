#include "graphics/Text.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace csci3081 {

std::map<std::string, std::map<char, Character>> Text::fontCache;
bool Text::freetypeInitialized = false;

void Text::initFreeType() {
  if (freetypeInitialized) return;
  freetypeInitialized = true;
}

std::string Text::getFontPath(const std::string& fontFilename) {
  return "assets/fonts/" + fontFilename;
}

std::string Text::getCacheKey() const {
  return std::to_string(fontSize) + "_" + fontFamily;
}

void Text::loadFont(int fontSize, const std::string& fontPath) {
  // Create cache key from fontSize and fontPath
  std::string cacheKey = std::to_string(fontSize) + "_" + fontPath;

  if (fontCache.find(cacheKey) != fontCache.end()) {
    return; // Already loaded
  }

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    return;
  }

  FT_Face face;
  std::string fullPath = getFontPath(fontPath);

  if (FT_New_Face(ft, fullPath.c_str(), 0, &face) != 0) {
    std::cerr << "ERROR::FREETYPE: Failed to load font: " << fullPath << std::endl;
    FT_Done_FreeType(ft);
    return;
  }

  FT_Set_Pixel_Sizes(face, 0, fontSize);

  // Load ASCII characters with better rendering
  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT)) {
      std::cerr << "ERROR::FREETYTPE: Failed to load Glyph " << c << std::endl;
      continue;
    }

    // Copy bitmap data
    int width = face->glyph->bitmap.width;
    int height = face->glyph->bitmap.rows;
    unsigned char* bitmap = new unsigned char[width * height];
    memcpy(bitmap, face->glyph->bitmap.buffer, width * height);

    Character character = {
      bitmap,
      width,
      height,
      face->glyph->bitmap_left,
      face->glyph->bitmap_top,
      static_cast<int>(face->glyph->advance.x >> 6)
    };

    fontCache[cacheKey][c] = character;
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

Text::Text(const std::string& text, const Color& color, int fontSize, const std::string& fontFamily)
  : text(text), color(color), fontSize(fontSize), fontFamily(fontFamily) {
  initFreeType();
  loadFont(fontSize, fontFamily);
}

Text::~Text() {
  // Font cache is static, so we don't clean it up here
}

Image* Text::renderToImage() {
  std::string cacheKey = getCacheKey();

  if (fontCache.find(cacheKey) == fontCache.end() || text.empty()) {
    return new Image(100, 100); // Return empty image if font not loaded
  }

  // Calculate total width and max height
  int totalWidth = 0;
  int maxHeight = 0;
  int maxBearingY = 0;

  for (char c : text) {
    if (fontCache[cacheKey].find(c) != fontCache[cacheKey].end()) {
      Character ch = fontCache[cacheKey][c];
      totalWidth += ch.advance;
      maxHeight = std::max(maxHeight, ch.height);
      maxBearingY = std::max(maxBearingY, ch.bearingY);
    }
  }

  if (totalWidth == 0 || maxHeight == 0) {
    return new Image(100, 100);
  }

  // Create image with padding
  int imageHeight = maxHeight + 20;
  Image* image = new Image(totalWidth + 20, imageHeight);

  // Fill with transparent background
  for (int y = 0; y < image->getHeight(); y++) {
    for (int x = 0; x < image->getWidth(); x++) {
      image->setPixel(x, y, Color(0, 0, 0, 0));
    }
  }

  // Render each character
  int xOffset = 10;
  for (char c : text) {
    if (fontCache[cacheKey].find(c) == fontCache[cacheKey].end()) {
      continue;
    }

    Character ch = fontCache[cacheKey][c];
    int xPos = xOffset + ch.bearingX;
    int yPos = maxBearingY - ch.bearingY + 10;

    // Render character bitmap
    for (int y = 0; y < ch.height; y++) {
      for (int x = 0; x < ch.width; x++) {
        int imgX = xPos + x;
        int imgY = yPos + y;

        if (imgX >= 0 && imgX < image->getWidth() &&
            imgY >= 0 && imgY < image->getHeight()) {
          unsigned char alpha = ch.bitmap[y * ch.width + x];
          if (alpha > 0) {
            // Blend text color with alpha from glyph
            Color finalColor = color;
            finalColor.setAlpha(alpha);
            image->setPixel(imgX, imgY, finalColor);
          }
        }
      }
    }

    xOffset += ch.advance;
  }

  return image;
}

}
