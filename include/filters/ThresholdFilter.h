#ifndef THRESHOLDFILTER_H_
#define THRESHOLDFILTER_H_

#include "filters/SimpleFilter.h"

namespace csci3081 {

class ThresholdFilter : public SimpleFilter {
public:
  ThresholdFilter(int threshold = 128) : thresholdValue(threshold) {}

  int getThreshold() const { return thresholdValue; }
  void setThreshold(int threshold) { thresholdValue = threshold; }

protected:
  virtual Color applyAtPixel(const Color& color) override {
    int luminance = color.getLuminance();
    if (luminance < thresholdValue) {
      return Color(0, 0, 0, 255);
    } else {
      return Color(255, 255, 255, 255);
    }
  }

private:
  int thresholdValue;
};

} // namespace csci3081

#endif // THRESHOLDFILTER_H_
