#ifndef GRAPHICS_TEXT_H_
#define GRAPHICS_TEXT_H_

#include "graphics/Color.h"
#include "Image.h"
#include <string>
#include <map>

namespace csci3081 {

struct Character {
  unsigned char* bitmap;
  int width;
  int height;
  int bearingX;
  int bearingY;
  int advance;
};

class Text {
public:
  /**
   * @brief Create a Text object for rendering
   * @param text The text string to render
   * @param color Text color (default white)
   * @param fontSize Font size in pixels (default 48)
   * @param fontFamily Font filename in assets/fonts/ (default "Roboto-Regular.ttf")
   */
  Text(const std::string& text,
       const Color& color = Color(255, 255, 255, 255),
       int fontSize = 48,
       const std::string& fontFamily = "Roboto-Regular.ttf");
  ~Text();

  Image* renderToImage();
  const std::string& getText() const { return text; }
  const Color& getColor() const { return color; }
  const std::string& getFontFamily() const { return fontFamily; }
  void setText(const std::string& newText) { text = newText; }
  void setColor(const Color& newColor) { color = newColor; }

private:
  static void initFreeType();
  static void loadFont(int fontSize, const std::string& fontPath);
  // Cache key: "fontSize_fontFamily" -> Character map
  static std::map<std::string, std::map<char, Character>> fontCache;
  static bool freetypeInitialized;

  std::string text;
  Color color;
  int fontSize;
  std::string fontFamily;

  /**
   * @brief Get the full path to a font file
   * @param fontFilename The font filename (e.g., "Roboto-Regular.ttf")
   * @return Full path "assets/fonts/fontFilename"
   */
  static std::string getFontPath(const std::string& fontFilename);

  /**
   * @brief Get cache key for font+size combination
   * @return Key string like "48_Roboto-Regular.ttf"
   */
  std::string getCacheKey() const;
};

}

#endif
