#ifndef SIMPLEFILTER_H_
#define SIMPLEFILTER_H_

#include "filters/IFilter.h"

namespace csci3081 {

/**
 * @brief Base class using Template Method Pattern for pixel-by-pixel filters
 */
class SimpleFilter : public IFilter {
public:
  virtual void Apply(const Image& original, Image& filtered) final {
    filtered = original;
    for (int x = 0; x < original.getWidth(); ++x) {
      for (int y = 0; y < original.getHeight(); ++y) {
        Color color = original.getPixel(x, y);
        Color newColor = applyAtPixel(color);
        filtered.setPixel(x, y, newColor);
      }
    }
  }

protected:
  virtual Color applyAtPixel(const Color& color) = 0;
};

} // namespace csci3081

#endif // SIMPLEFILTER_H_
