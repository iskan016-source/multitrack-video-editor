#ifndef GREYSCALEFILTER_H_
#define GREYSCALEFILTER_H_

#include "filters/SimpleFilter.h"

namespace csci3081 {

class ChromaFilter : public SimpleFilter {
protected:
  virtual Color applyAtPixel(const Color& color) override {
    constexpr int GreenSuppressionCoefficient = 50;
    // If there is sufficient green i.e. the green at this pixel is stronger than the average of red and blue
    // Soft remove
    int GreenStrengthAtPixel = color.green() - (color.red() + color.blue()) / 2;
    if (GreenStrengthAtPixel > 0) {
      int NewGreen = color.green() - GreenStrengthAtPixel * GreenSuppressionCoefficient;
      NewGreen = NewGreen < 0 ? 0 : NewGreen; // Make sure value doesn't go negative
      return {color.red(), NewGreen, color.blue(), NewGreen};
    } else {
      return color;
    }
  }
};

} // namespace csci3081

#endif // GREYSCALEFILTER_H_
