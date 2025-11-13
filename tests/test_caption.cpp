/**
 * @file test_caption.cpp
 * @brief Unit tests for the Caption class
 *
 * This file tests the Caption asset functionality including:
 * - Text rendering to images
 * - Asset properties (duration, type, etc.)
 * - Dynamic text and color updates
 */

#include <gtest/gtest.h>
#include "assets/Caption.h"
#include "graphics/Color.h"
#include "Image.h"

using namespace csci3081;

// ==============================================================================
// Test Fixture for Caption Tests
// ==============================================================================

/**
 * @class CaptionTest
 * @brief Test fixture for Caption asset tests
 *
 * Provides common setup for Caption tests with default font and parameters
 */
class CaptionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Default test parameters - these will be used in tests
    }

    void TearDown() override {
        // Cleanup if needed
    }

    // Helper methods to get default test values
    std::string getTestText() { return "Hello World"; }
    std::string getTestFont() { return "GoogleSansCode-Regular.ttf"; }
    unsigned int getTestFontSize() { return 48; }
    Color getTestColor() { return Color(255, 255, 255, 255); } // White
    double getTestDuration() { return 5.0; }
};

// ==============================================================================
// Constructor Tests
// ==============================================================================

/**
 * Test: Caption constructor creates valid asset
 * Purpose: Verify Caption can be created with text and default parameters
 */
TEST_F(CaptionTest, ConstructorCreatesValidCaption) {
    Caption caption(getTestText());

    // Caption should create a valid image
    const Image& frame = caption.getFrame();
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: Caption constructor with full parameters
 * Purpose: Verify Caption accepts all constructor parameters
 */
TEST_F(CaptionTest, ConstructorWithFullParameters) {
    std::string text = getTestText();
    double duration = getTestDuration();
    Caption caption(text, "assets/fonts/" + getTestFont(), getTestFontSize(), getTestColor(), duration);

    EXPECT_EQ(caption.getDuration(), duration);
    EXPECT_EQ(caption.getText(), text);

    const Image& frame = caption.getFrame();
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: Empty text caption
 * Purpose: Verify Caption handles empty string gracefully
 */
TEST_F(CaptionTest, EmptyTextCaption) {
    Caption caption("");

    // Should create a valid (possibly small) image
    const Image& frame = caption.getFrame();
    EXPECT_GE(frame.getWidth(), 0);
    EXPECT_GE(frame.getHeight(), 0);
}

// ==============================================================================
// IAsset Interface Tests
// ==============================================================================

/**
 * Test: Caption returns correct asset type
 * Purpose: Verify Caption implements AssetType::TEXT
 */
TEST_F(CaptionTest, ReturnsTextAssetType) {
    Caption caption(getTestText());

    EXPECT_EQ(caption.getAssetType(), AssetType::TEXT);
}

/**
 * Test: Caption is not a video
 * Purpose: Verify isVideo() returns false for static text
 */
TEST_F(CaptionTest, IsNotVideo) {
    Caption caption(getTestText());

    EXPECT_FALSE(caption.isVideo());
}

/**
 * Test: getDuration returns configured duration
 * Purpose: Verify duration property is correctly stored and retrieved
 */
TEST_F(CaptionTest, GetDurationReturnsConfiguredValue) {
    double customDuration = 10.5;
    Caption caption(getTestText(), "assets/fonts/" + getTestFont(), getTestFontSize(), getTestColor(), customDuration);

    EXPECT_DOUBLE_EQ(caption.getDuration(), customDuration);
}

/**
 * Test: getFrame returns same image regardless of time
 * Purpose: Verify Caption is static (doesn't animate)
 */
TEST_F(CaptionTest, GetFrameIgnoresTimeParameter) {
    Caption caption(getTestText());

    const Image& frame1 = caption.getFrame(0.0);
    const Image& frame2 = caption.getFrame(5.0);
    const Image& frame3 = caption.getFrame(100.0);

    // All frames should be the same object (static image)
    EXPECT_EQ(&frame1, &frame2);
    EXPECT_EQ(&frame2, &frame3);
}

/**
 * Test: getThumbnail returns rendered text
 * Purpose: Verify thumbnail shows the caption text
 */
TEST_F(CaptionTest, GetThumbnailReturnsRenderedText) {
    Caption caption(getTestText());

    const Image& thumbnail = caption.getThumbnail();
    EXPECT_GT(thumbnail.getWidth(), 0);
    EXPECT_GT(thumbnail.getHeight(), 0);

    // Thumbnail should be the same as frame
    const Image& frame = caption.getFrame();
    EXPECT_EQ(&thumbnail, &frame);
}

// ==============================================================================
// Dynamic Text Update Tests
// ==============================================================================

/**
 * Test: setText updates the caption text
 * Purpose: Verify Caption can dynamically change text
 */
TEST_F(CaptionTest, SetTextUpdatesCaption) {
    Caption caption(getTestText());

    std::string newText = "Updated Text";
    caption.setText(newText);

    EXPECT_EQ(caption.getText(), newText);
}

/**
 * Test: setText regenerates the image
 * Purpose: Verify changing text creates a new rendered image
 */
TEST_F(CaptionTest, SetTextRegeneratesImage) {
    Caption caption("Short");
    const Image& frame1 = caption.getFrame();
    int width1 = frame1.getWidth();

    // Change to much longer text
    caption.setText("This is a much longer text string");
    const Image& frame2 = caption.getFrame();
    int width2 = frame2.getWidth();

    // Longer text should generally produce wider image
    EXPECT_NE(width1, width2);
}

/**
 * Test: setTextColor updates the caption color
 * Purpose: Verify Caption can dynamically change text color
 */
TEST_F(CaptionTest, SetTextColorUpdatesCaption) {
    Caption caption(getTestText());

    Color newColor(255, 0, 0, 255); // Red
    caption.setTextColor(newColor);

    // After color change, image should still be valid
    const Image& frame = caption.getFrame();
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

// ==============================================================================
// Font Handling Tests
// ==============================================================================

/**
 * Test: Caption with different font sizes
 * Purpose: Verify font size affects image dimensions
 */
TEST_F(CaptionTest, DifferentFontSizesProduceDifferentImages) {
    std::string text = getTestText();
    std::string font = "assets/fonts/" + getTestFont();
    Caption smallCaption(text, font, 24, getTestColor());
    Caption largeCaption(text, font, 96, getTestColor());

    const Image& smallFrame = smallCaption.getFrame();
    const Image& largeFrame = largeCaption.getFrame();

    // Larger font should produce larger image
    EXPECT_LT(smallFrame.getHeight(), largeFrame.getHeight());
}

/**
 * Test: Caption handles missing font file
 * Purpose: Verify Caption falls back gracefully when font is not found
 */
TEST_F(CaptionTest, HandlesMissingFontFile) {
    // Create caption with non-existent font
    Caption caption(getTestText(), "assets/fonts/NonExistent.ttf", getTestFontSize(), getTestColor());

    // Should still create a valid image (using fallback font)
    const Image& frame = caption.getFrame();
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

// ==============================================================================
// Image Property Tests
// ==============================================================================

/**
 * Test: Caption image has non-zero dimensions
 * Purpose: Verify rendered text produces meaningful image size
 */
TEST_F(CaptionTest, RenderedImageHasNonZeroDimensions) {
    Caption caption(getTestText(), "assets/fonts/" + getTestFont(), getTestFontSize(), getTestColor());

    const Image& frame = caption.getFrame();

    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);

    // For non-empty text, dimensions should be substantial
    EXPECT_GT(frame.getWidth(), 10);
    EXPECT_GT(frame.getHeight(), 10);
}

/**
 * Test: Longer text produces wider image
 * Purpose: Verify image width scales with text length
 */
TEST_F(CaptionTest, LongerTextProducesWiderImage) {
    Caption shortCaption("Hi");
    Caption longCaption("This is a very long caption with many characters");

    const Image& shortFrame = shortCaption.getFrame();
    const Image& longFrame = longCaption.getFrame();

    EXPECT_LT(shortFrame.getWidth(), longFrame.getWidth());
}

// ==============================================================================
// Color Tests
// ==============================================================================

/**
 * Test: Caption with different colors
 * Purpose: Verify Caption accepts different color values
 */
TEST_F(CaptionTest, AcceptsDifferentColors) {
    Color red(255, 0, 0, 255);
    Color green(0, 255, 0, 255);
    Color blue(0, 0, 255, 255);

    std::string text = getTestText();
    std::string font = "assets/fonts/" + getTestFont();
    unsigned int fontSize = getTestFontSize();

    Caption redCaption(text, font, fontSize, red);
    Caption greenCaption(text, font, fontSize, green);
    Caption blueCaption(text, font, fontSize, blue);

    // All should create valid images
    EXPECT_GT(redCaption.getFrame().getWidth(), 0);
    EXPECT_GT(greenCaption.getFrame().getWidth(), 0);
    EXPECT_GT(blueCaption.getFrame().getWidth(), 0);
}

/**
 * Test: Caption with transparent color
 * Purpose: Verify alpha channel is handled
 */
TEST_F(CaptionTest, HandlesTransparentColor) {
    Color transparent(255, 255, 255, 0); // Fully transparent white
    Caption caption(getTestText(), "assets/fonts/" + getTestFont(), getTestFontSize(), transparent);

    // Should still create valid image
    const Image& frame = caption.getFrame();
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}
