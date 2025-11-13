#ifndef MEANBLURFILTER_H_
#define MEANBLURFILTER_H_

#include "filters/IFilter.h"

namespace csci3081 {

class MeanBlurFilter : public IFilter {
public:
  virtual void Apply(const Image& original, Image& filtered) override {
    filtered = original;
    for (int x = 0; x < original.getWidth(); ++x) {
      for (int y = 0; y < original.getHeight(); ++y) {
        Color sum(0, 0, 0, 0);
        for (int dx = -1; dx <= 1; ++dx) {
          for (int dy = -1; dy <= 1; ++dy) {
            sum = sum + original.getPixel(x + dx, y + dy);
          }
        }
        filtered.setPixel(x, y, sum / 9.0f);
      }
    }
  }
};

} // namespace csci3081

#endif // MEANBLURFILTER_H_
