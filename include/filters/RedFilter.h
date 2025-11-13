#ifndef REDFILTER_H_
#define REDFILTER_H_

#include "filters/SimpleFilter.h"

namespace csci3081 {

class RedFilter : public SimpleFilter {
protected:
  virtual Color applyAtPixel(const Color& color) override {
    return Color(color.red(), 0, 0, color.alpha());
  }
};

} // namespace csci3081

#endif // REDFILTER_H_
