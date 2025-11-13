#ifndef IFILTER_H_
#define IFILTER_H_

#include "Image.h"

namespace csci3081 {

/**
 * @brief Strategy Pattern Interface for Image Filters
 *
 * IFilter defines the common interface for all image filtering strategies.
 * Different filter implementations can be swapped at runtime while maintaining
 * a consistent interface.
 *
 * Design Pattern: Strategy Pattern
 * - Context: The image processing system
 * - Strategy: IFilter (this interface)
 * - Concrete Strategies: RedFilter, GreyscaleFilter, ThresholdFilter, MeanBlurFilter
 */
class IFilter {
public:
    virtual ~IFilter() {}

    /**
     * @brief Apply the filter to an image
     *
     * Takes an original image and produces a filtered result.
     * The filtered image is passed by reference and will be modified.
     *
     * @param original The input image (not modified)
     * @param filtered The output image (will be modified)
     */
    virtual void Apply(const Image& original, Image& filtered) = 0;
};

} // namespace csci3081

#endif // IFILTER_H_
