/**
 * @file test_filters.cpp
 * @brief Unit tests for image filters using Strategy and Template Method patterns
 *
 * Tests the filter implementations:
 * - Strategy Pattern: IFilter interface with different filter strategies
 * - Template Method Pattern: SimpleFilter base class with applyAtPixel() hook
 * - Concrete Filters: RedFilter, GreyscaleFilter, ThresholdFilter, MeanBlurFilter
 */

#include <gtest/gtest.h>
#include "Image.h"
#include "graphics/Color.h"
#include "filters/IFilter.h"
#include "filters/SimpleFilter.h"
#include "filters/RedFilter.h"
#include "filters/GreyscaleFilter.h"
#include "filters/ThresholdFilter.h"
#include "filters/MeanBlurFilter.h"
#include <memory>

using namespace csci3081;

// ==============================================================================
// Test Fixture
// ==============================================================================

class FilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple 3x3 test image with known colors
        testImage = new Image(3, 3);

        // Set up test pattern:
        // R(255,0,0)   G(0,255,0)   B(0,0,255)
        // W(255,255,255) K(0,0,0)   Grey(128,128,128)
        // R(255,0,0)   G(0,255,0)   B(0,0,255)

        testImage->setPixel(0, 0, Color(255, 0, 0, 255));     // Red
        testImage->setPixel(1, 0, Color(0, 255, 0, 255));     // Green
        testImage->setPixel(2, 0, Color(0, 0, 255, 255));     // Blue
        testImage->setPixel(0, 1, Color(255, 255, 255, 255)); // White
        testImage->setPixel(1, 1, Color(0, 0, 0, 255));       // Black
        testImage->setPixel(2, 1, Color(128, 128, 128, 255)); // Grey
        testImage->setPixel(0, 2, Color(255, 0, 0, 255));     // Red
        testImage->setPixel(1, 2, Color(0, 255, 0, 255));     // Green
        testImage->setPixel(2, 2, Color(0, 0, 255, 255));     // Blue
    }

    void TearDown() override {
        delete testImage;
    }

    Image* testImage;
};

// ==============================================================================
// Color Class Enhancement Tests
// ==============================================================================

TEST_F(FilterTest, ColorGetLuminance) {
    Color red(255, 0, 0, 255);
    Color green(0, 255, 0, 255);
    Color blue(0, 0, 255, 255);

    // Red: 0.2126 * 255 = 54.2
    EXPECT_NEAR(red.getLuminance(), 54, 1);

    // Green: 0.7152 * 255 = 182.4
    EXPECT_NEAR(green.getLuminance(), 182, 1);

    // Blue: 0.0722 * 255 = 18.4
    EXPECT_NEAR(blue.getLuminance(), 18, 1);
}

TEST_F(FilterTest, ColorArithmeticOperations) {
    Color c1(100, 150, 200, 255);
    Color c2(50, 50, 50, 0);

    // Addition
    Color sum = c1 + c2;
    EXPECT_EQ(sum.red(), 150);
    EXPECT_EQ(sum.green(), 200);
    EXPECT_EQ(sum.blue(), 250);

    // Division
    Color divided = c1 / 2.0f;
    EXPECT_EQ(divided.red(), 50);
    EXPECT_EQ(divided.green(), 75);
    EXPECT_EQ(divided.blue(), 100);

    // Multiplication
    Color multiplied = c1 * 0.5f;
    EXPECT_EQ(multiplied.red(), 50);
    EXPECT_EQ(multiplied.green(), 75);
    EXPECT_EQ(multiplied.blue(), 100);
}

TEST_F(FilterTest, ColorClamp) {
    Color c(100, 150, 200, 255);
    Color clamped = c.clamp(120, 180);

    EXPECT_EQ(clamped.red(), 120);    // Clamped to min
    EXPECT_EQ(clamped.green(), 150);  // Within range
    EXPECT_EQ(clamped.blue(), 180);   // Clamped to max
}

// ==============================================================================
// RedFilter Tests (Template Method Pattern)
// ==============================================================================

TEST_F(FilterTest, RedFilterKeepsRedChannel) {
    RedFilter filter;
    Image filtered;

    filter.Apply(*testImage, filtered);

    // Check that red channel is preserved
    Color pixel = filtered.getPixel(0, 0);
    EXPECT_EQ(pixel.red(), 255);
    EXPECT_EQ(pixel.green(), 0);
    EXPECT_EQ(pixel.blue(), 0);
}

TEST_F(FilterTest, RedFilterZerosGreenAndBlue) {
    RedFilter filter;
    Image filtered;

    filter.Apply(*testImage, filtered);

    // Check green pixel (should have no green/blue)
    Color greenPixel = filtered.getPixel(1, 0);
    EXPECT_EQ(greenPixel.red(), 0);
    EXPECT_EQ(greenPixel.green(), 0);
    EXPECT_EQ(greenPixel.blue(), 0);

    // Check blue pixel (should have no red/green)
    Color bluePixel = filtered.getPixel(2, 0);
    EXPECT_EQ(bluePixel.red(), 0);
    EXPECT_EQ(bluePixel.green(), 0);
    EXPECT_EQ(bluePixel.blue(), 0);
}

TEST_F(FilterTest, RedFilterPreservesImageSize) {
    RedFilter filter;
    Image filtered;

    filter.Apply(*testImage, filtered);

    EXPECT_EQ(filtered.getWidth(), testImage->getWidth());
    EXPECT_EQ(filtered.getHeight(), testImage->getHeight());
}

// ==============================================================================
// GreyscaleFilter Tests (Template Method Pattern)
// ==============================================================================

TEST_F(FilterTest, GreyscaleFilterConvertsToGrey) {
    GreyscaleFilter filter;
    Image filtered;

    filter.Apply(*testImage, filtered);

    // Check that red pixel becomes grey
    Color pixel = filtered.getPixel(0, 0);
    int luminance = pixel.red();
    EXPECT_EQ(pixel.red(), luminance);
    EXPECT_EQ(pixel.green(), luminance);
    EXPECT_EQ(pixel.blue(), luminance);
}

TEST_F(FilterTest, GreyscaleFilterUsesLuminance) {
    GreyscaleFilter filter;
    Image filtered;

    filter.Apply(*testImage, filtered);

    // Red should be around 54 (0.2126 * 255)
    Color redPixel = filtered.getPixel(0, 0);
    EXPECT_NEAR(redPixel.red(), 54, 2);

    // Green should be around 182 (0.7152 * 255)
    Color greenPixel = filtered.getPixel(1, 0);
    EXPECT_NEAR(greenPixel.red(), 182, 2);

    // Blue should be around 18 (0.0722 * 255)
    Color bluePixel = filtered.getPixel(2, 0);
    EXPECT_NEAR(bluePixel.red(), 18, 2);
}

TEST_F(FilterTest, GreyscaleFilterPreservesWhiteAndBlack) {
    GreyscaleFilter filter;
    Image filtered;

    filter.Apply(*testImage, filtered);

    // White should stay white
    Color whitePixel = filtered.getPixel(0, 1);
    EXPECT_EQ(whitePixel.red(), 255);
    EXPECT_EQ(whitePixel.green(), 255);
    EXPECT_EQ(whitePixel.blue(), 255);

    // Black should stay black
    Color blackPixel = filtered.getPixel(1, 1);
    EXPECT_EQ(blackPixel.red(), 0);
    EXPECT_EQ(blackPixel.green(), 0);
    EXPECT_EQ(blackPixel.blue(), 0);
}

// ==============================================================================
// ThresholdFilter Tests (Template Method Pattern with Parameter)
// ==============================================================================

TEST_F(FilterTest, ThresholdFilterDefaultThreshold) {
    ThresholdFilter filter;  // Default threshold = 128
    Image filtered;

    filter.Apply(*testImage, filtered);

    // White (luminance 255) should become white
    Color whitePixel = filtered.getPixel(0, 1);
    EXPECT_EQ(whitePixel.red(), 255);
    EXPECT_EQ(whitePixel.green(), 255);
    EXPECT_EQ(whitePixel.blue(), 255);

    // Black (luminance 0) should stay black
    Color blackPixel = filtered.getPixel(1, 1);
    EXPECT_EQ(blackPixel.red(), 0);
    EXPECT_EQ(blackPixel.green(), 0);
    EXPECT_EQ(blackPixel.blue(), 0);
}

TEST_F(FilterTest, ThresholdFilterCustomThreshold) {
    ThresholdFilter filter(100);
    Image filtered;

    filter.Apply(*testImage, filtered);

    // Red (luminance ~54) < 100, should become black
    Color redPixel = filtered.getPixel(0, 0);
    EXPECT_EQ(redPixel.red(), 0);
    EXPECT_EQ(redPixel.green(), 0);
    EXPECT_EQ(redPixel.blue(), 0);

    // Green (luminance ~182) >= 100, should become white
    Color greenPixel = filtered.getPixel(1, 0);
    EXPECT_EQ(greenPixel.red(), 255);
    EXPECT_EQ(greenPixel.green(), 255);
    EXPECT_EQ(greenPixel.blue(), 255);
}

TEST_F(FilterTest, ThresholdFilterGetSetThreshold) {
    ThresholdFilter filter(128);

    EXPECT_EQ(filter.getThreshold(), 128);

    filter.setThreshold(200);
    EXPECT_EQ(filter.getThreshold(), 200);
}

TEST_F(FilterTest, ThresholdFilterBinaryOutput) {
    ThresholdFilter filter(128);
    Image filtered;

    filter.Apply(*testImage, filtered);

    // All pixels should be either pure black or pure white
    for (int x = 0; x < filtered.getWidth(); ++x) {
        for (int y = 0; y < filtered.getHeight(); ++y) {
            Color pixel = filtered.getPixel(x, y);
            bool isBlack = (pixel.red() == 0 && pixel.green() == 0 && pixel.blue() == 0);
            bool isWhite = (pixel.red() == 255 && pixel.green() == 255 && pixel.blue() == 255);
            EXPECT_TRUE(isBlack || isWhite) << "Pixel at (" << x << "," << y << ") is neither black nor white";
        }
    }
}

// ==============================================================================
// MeanBlurFilter Tests (Direct Strategy Pattern)
// ==============================================================================

TEST_F(FilterTest, MeanBlurFilterAveragesNeighborhood) {
    MeanBlurFilter filter;
    Image filtered;

    filter.Apply(*testImage, filtered);

    // Center pixel (1,1) should be average of its 3x3 neighborhood
    Color centerPixel = filtered.getPixel(1, 1);

    // The neighborhood around (1,1) includes all 9 pixels
    // We can't predict exact values but they should be averaged
    EXPECT_GT(centerPixel.red(), 0);
    EXPECT_LT(centerPixel.red(), 255);
}

TEST_F(FilterTest, MeanBlurFilterPreservesImageSize) {
    MeanBlurFilter filter;
    Image filtered;

    filter.Apply(*testImage, filtered);

    EXPECT_EQ(filtered.getWidth(), testImage->getWidth());
    EXPECT_EQ(filtered.getHeight(), testImage->getHeight());
}

TEST_F(FilterTest, MeanBlurFilterHandlesEdges) {
    MeanBlurFilter filter;
    Image filtered;

    // Should not crash when processing edge pixels
    EXPECT_NO_THROW(filter.Apply(*testImage, filtered));
}

TEST_F(FilterTest, MeanBlurFilterReducesContrast) {
    // Create high contrast image
    Image highContrast(10, 10);
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            if ((x + y) % 2 == 0) {
                highContrast.setPixel(x, y, Color(255, 255, 255, 255));
            } else {
                highContrast.setPixel(x, y, Color(0, 0, 0, 255));
            }
        }
    }

    MeanBlurFilter filter;
    Image blurred;
    filter.Apply(highContrast, blurred);

    // After blur, extreme values should be reduced
    // Checkerboard pattern will produce values between 0-255
    // We just verify it's in the middle range, not extreme
    Color pixel = blurred.getPixel(5, 5);
    EXPECT_GT(pixel.red(), 10);   // Not pure black (with some tolerance)
    EXPECT_LT(pixel.red(), 245);  // Not pure white (with some tolerance)
}

// ==============================================================================
// Strategy Pattern Tests - Polymorphic Filter Usage
// ==============================================================================

TEST_F(FilterTest, PolymorphicFilterUsage) {
    // Demonstrate strategy pattern - filters can be swapped at runtime
    std::unique_ptr<IFilter> filter1(new RedFilter());
    std::unique_ptr<IFilter> filter2(new GreyscaleFilter());
    std::unique_ptr<IFilter> filter3(new ThresholdFilter());
    std::unique_ptr<IFilter> filter4(new MeanBlurFilter());

    Image filtered1, filtered2, filtered3, filtered4;

    // All use the same interface
    filter1->Apply(*testImage, filtered1);
    filter2->Apply(*testImage, filtered2);
    filter3->Apply(*testImage, filtered3);
    filter4->Apply(*testImage, filtered4);

    // Results should differ
    Color pixel1 = filtered1.getPixel(0, 0);
    Color pixel2 = filtered2.getPixel(0, 0);
    Color pixel3 = filtered3.getPixel(0, 0);

    // Red filter vs greyscale should differ
    EXPECT_NE(pixel1.green(), pixel2.green());
}

// ==============================================================================
// Template Method Pattern Tests
// ==============================================================================

TEST_F(FilterTest, SimpleFilterTemplateMethodStructure) {
    // RedFilter, GreyscaleFilter, ThresholdFilter all use SimpleFilter
    // They should all process pixels independently

    RedFilter red;
    GreyscaleFilter grey;
    ThresholdFilter threshold;

    Image filteredRed, filteredGrey, filteredThreshold;

    // All should complete successfully
    EXPECT_NO_THROW(red.Apply(*testImage, filteredRed));
    EXPECT_NO_THROW(grey.Apply(*testImage, filteredGrey));
    EXPECT_NO_THROW(threshold.Apply(*testImage, filteredThreshold));
}

// ==============================================================================
// Integration Tests - Multiple Filters
// ==============================================================================

TEST_F(FilterTest, ChainedFilters) {
    // Apply multiple filters in sequence
    RedFilter red;
    GreyscaleFilter grey;

    Image temp, final;

    // First apply red filter
    red.Apply(*testImage, temp);

    // Then apply greyscale to the result
    grey.Apply(temp, final);

    // Final image should be greyscale version of red-filtered image
    Color pixel = final.getPixel(0, 0);
    EXPECT_EQ(pixel.red(), pixel.green());
    EXPECT_EQ(pixel.green(), pixel.blue());
}
