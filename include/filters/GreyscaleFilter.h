#ifndef GREYSCALEFILTER_H_
#define GREYSCALEFILTER_H_

#include "filters/SimpleFilter.h"

namespace csci3081 {

class GreyscaleFilter : public SimpleFilter {
protected:
  virtual Color applyAtPixel(const Color& color) override {
    int luminance = color.getLuminance();
    return Color(luminance, luminance, luminance, color.alpha());
  }
};

} // namespace csci3081

#endif // GREYSCALEFILTER_H_
