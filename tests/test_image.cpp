/**
 * @file test_image.cpp
 * @brief Unit tests for the Image class
 *
 * This file demonstrates various types of unit tests for testing file I/O,
 * pixel manipulation, and property access in the Image class.
 */

#include <gtest/gtest.h>
#include "Image.h"
#include "graphics/Color.h"
#include <string>

using namespace csci3081;

// ==============================================================================
// Test Fixture for Image Tests
// ==============================================================================

/**
 * @class ImageTest
 * @brief Test fixture that provides setup and teardown for Image tests
 *
 * Test fixtures allow you to reuse common setup code across multiple tests.
 * The SetUp() method runs before each test, and TearDown() runs after.
 */
class ImageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code runs before each test
        // Path is relative to where the test executable is run (build directory)
        testImagePath = "../tests/fixtures/test_image.jpeg";
        testWidth = 100;
        testHeight = 100;
    }

    void TearDown() override {
        // Cleanup code runs after each test
    }

    // Helper variables available to all tests in this fixture
    std::string testImagePath;
    int testWidth;
    int testHeight;
};

// ==============================================================================
// Constructor Tests - Testing different ways to create Image objects
// ==============================================================================

/**
 * Test: Default constructor creates a valid image
 * Purpose: Verify that the default constructor initializes an Image object
 */
TEST_F(ImageTest, DefaultConstructorCreatesValidImage) {
    Image img;
    // We don't make assumptions about default dimensions, just that it doesn't crash
    EXPECT_GE(img.getWidth(), 0);
    EXPECT_GE(img.getHeight(), 0);
}

/**
 * Test: Constructor with width and height creates image with correct dimensions
 * Purpose: Verify parametrized constructor creates image of specified size
 */
TEST_F(ImageTest, WidthHeightConstructorCreatesCorrectSize) {
    Image img(testWidth, testHeight);
    EXPECT_EQ(img.getWidth(), testWidth);
    EXPECT_EQ(img.getHeight(), testHeight);
}

// ==============================================================================
// File I/O Tests - Testing loading and saving images
// ==============================================================================

/**
 * Test: Loading a valid image file succeeds
 * Purpose: Verify that Image can load actual image files from disk
 * Coverage: Tests file I/O and constructor from filename
 */
TEST_F(ImageTest, LoadValidImageFile) {
    Image img(testImagePath);

    // Image should have non-zero dimensions
    EXPECT_GT(img.getWidth(), 0);
    EXPECT_GT(img.getHeight(), 0);
}

/**
 * Test: Loading a non-existent file handles error gracefully
 * Purpose: Verify error handling for invalid file paths
 * Coverage: Edge case testing
 *
 * UPDATE: This test revealed a nullptr dereference bug in Image.cpp which
 * has now been FIXED! The Image constructor now checks if stbi_load returns
 * nullptr and creates an invalid (0x0) image instead of crashing.
 *
 * This is a perfect example of how unit tests help discover and fix bugs!
 */
TEST_F(ImageTest, LoadNonExistentFileHandlesGracefully) {
    Image img("nonexistent_file.png");

    // After fix: Image constructor creates 0x0 image for failed loads
    EXPECT_EQ(img.getWidth(), 0);
    EXPECT_EQ(img.getHeight(), 0);
}

// TODO: Uncomment this test once you implement saveAs() method testing
/*
TEST_F(ImageTest, SaveAndLoadImagePreservesData) {
    // Create an image with known data
    Image original(10, 10);
    Color testColor(255, 128, 64, 255);
    original.setPixel(5, 5, testColor);

    // Save to temporary file
    std::string tempFile = "tests/fixtures/temp_test_image.png";
    original.saveAs(tempFile);

    // Load the saved image
    Image loaded(tempFile);

    // Verify dimensions match
    EXPECT_EQ(loaded.getWidth(), original.getWidth());
    EXPECT_EQ(loaded.getHeight(), original.getHeight());

    // Verify pixel data matches (note: compression may affect exact values)
    Color loadedColor = loaded.getPixel(5, 5);
    EXPECT_NEAR(loadedColor.r, testColor.r, 5); // Allow small tolerance for compression

    // Cleanup
    std::remove(tempFile.c_str());
}
*/

// ==============================================================================
// Dimension and Property Tests
// ==============================================================================

/**
 * Test: getWidth() returns correct width
 * Purpose: Verify width property accessor
 */
TEST_F(ImageTest, GetWidthReturnsCorrectValue) {
    Image img(testWidth, testHeight);
    EXPECT_EQ(img.getWidth(), testWidth);
}

/**
 * Test: getHeight() returns correct height
 * Purpose: Verify height property accessor
 */
TEST_F(ImageTest, GetHeightReturnsCorrectValue) {
    Image img(testWidth, testHeight);
    EXPECT_EQ(img.getHeight(), testHeight);
}

// ==============================================================================
// Pixel Manipulation Tests - Testing getPixel() and setPixel()
// ==============================================================================

/**
 * Test: setPixel() and getPixel() work correctly
 * Purpose: Verify basic pixel read/write operations
 * Coverage: Tests core image editing functionality
 */
TEST_F(ImageTest, SetAndGetPixelWorks) {
    Image img(testWidth, testHeight);
    Color testColor(255, 128, 64, 255); // Orange color (R, G, B, A)

    int x = testWidth / 2;
    int y = testHeight / 2;

    img.setPixel(x, y, testColor);
    Color retrievedColor = img.getPixel(x, y);

    EXPECT_EQ(retrievedColor.red(), testColor.red());
    EXPECT_EQ(retrievedColor.green(), testColor.green());
    EXPECT_EQ(retrievedColor.blue(), testColor.blue());
    EXPECT_EQ(retrievedColor.alpha(), testColor.alpha());
}

/**
 * Test: Setting multiple pixels independently
 * Purpose: Verify that setting one pixel doesn't affect others
 */
TEST_F(ImageTest, SetMultiplePixelsIndependently) {
    Image img(testWidth, testHeight);

    Color red(255, 0, 0, 255);
    Color green(0, 255, 0, 255);
    Color blue(0, 0, 255, 255);

    img.setPixel(0, 0, red);
    img.setPixel(testWidth - 1, 0, green);
    img.setPixel(0, testHeight - 1, blue);

    Color retrievedRed = img.getPixel(0, 0);
    Color retrievedGreen = img.getPixel(testWidth - 1, 0);
    Color retrievedBlue = img.getPixel(0, testHeight - 1);

    EXPECT_EQ(retrievedRed.red(), 255);
    EXPECT_EQ(retrievedGreen.green(), 255);
    EXPECT_EQ(retrievedBlue.blue(), 255);
}

/**
 * Test: Pixel operations at boundary positions
 * Purpose: Test edge cases at image boundaries
 * Coverage: Boundary testing
 */
TEST_F(ImageTest, PixelOperationsAtBoundaries) {
    Image img(testWidth, testHeight);
    Color testColor(200, 150, 100, 255);

    // Test all four corners
    img.setPixel(0, 0, testColor);                              // Top-left
    img.setPixel(testWidth - 1, 0, testColor);                  // Top-right
    img.setPixel(0, testHeight - 1, testColor);                 // Bottom-left
    img.setPixel(testWidth - 1, testHeight - 1, testColor);     // Bottom-right

    // Verify we can read them back
    EXPECT_EQ(img.getPixel(0, 0).red(), testColor.red());
    EXPECT_EQ(img.getPixel(testWidth - 1, 0).red(), testColor.red());
    EXPECT_EQ(img.getPixel(0, testHeight - 1).red(), testColor.red());
    EXPECT_EQ(img.getPixel(testWidth - 1, testHeight - 1).red(), testColor.red());
}

// TODO: Add test for out-of-bounds pixel access
// This depends on your error handling approach
/*
TEST_F(ImageTest, OutOfBoundsPixelAccessHandledGracefully) {
    Image img(testWidth, testHeight);

    // Access beyond image bounds
    // Adjust based on your implementation:
    // - May throw exception
    // - May return default color
    // - May clamp to valid range

    // Example for exception-based error handling:
    // EXPECT_THROW(img.getPixel(-1, 0), std::out_of_range);
    // EXPECT_THROW(img.getPixel(testWidth, 0), std::out_of_range);
}
*/

// ==============================================================================
// Copy Constructor and Assignment Tests
// ==============================================================================

/**
 * Test: Copy constructor creates independent copy
 * Purpose: Verify deep copy behavior - modifying copy doesn't affect original
 * Coverage: Tests copy semantics
 */
TEST_F(ImageTest, CopyConstructorCreatesIndependentCopy) {
    Image original(50, 50);
    Color originalColor(255, 0, 0, 255);
    original.setPixel(25, 25, originalColor);

    // Create copy
    Image copy(original);

    // Verify dimensions match
    EXPECT_EQ(copy.getWidth(), original.getWidth());
    EXPECT_EQ(copy.getHeight(), original.getHeight());

    // Verify pixel data matches
    Color copiedColor = copy.getPixel(25, 25);
    EXPECT_EQ(copiedColor.red(), originalColor.red());

    // Modify the copy
    Color newColor(0, 255, 0, 255);
    copy.setPixel(25, 25, newColor);

    // Verify original is unchanged (deep copy)
    Color originalStillRed = original.getPixel(25, 25);
    EXPECT_EQ(originalStillRed.red(), 255);
    EXPECT_EQ(originalStillRed.green(), 0);
}

/**
 * Test: Assignment operator creates independent copy
 * Purpose: Verify assignment operator performs deep copy
 */
TEST_F(ImageTest, AssignmentOperatorCreatesIndependentCopy) {
    Image original(50, 50);
    Color originalColor(255, 0, 0, 255);
    original.setPixel(25, 25, originalColor);

    // Create and assign
    Image assigned(10, 10); // Different size initially
    assigned = original;

    // Verify dimensions match
    EXPECT_EQ(assigned.getWidth(), original.getWidth());
    EXPECT_EQ(assigned.getHeight(), original.getHeight());

    // Modify the assigned image
    Color newColor(0, 255, 0, 255);
    assigned.setPixel(25, 25, newColor);

    // Verify original is unchanged
    Color originalStillRed = original.getPixel(25, 25);
    EXPECT_EQ(originalStillRed.red(), 255);
}

// ==============================================================================
// Integration Tests - Testing with real file operations
// ==============================================================================

/**
 * Test: Loading real image file produces expected dimensions
 * Purpose: Integration test with actual image file
 * Note: This test requires knowledge of the test fixture dimensions
 */
TEST_F(ImageTest, LoadRealImageHasValidDimensions) {
    Image img(testImagePath);

    // Actual dimensions depend on your test_image.jpeg
    // We just verify they're reasonable (non-zero and not too large)
    EXPECT_GT(img.getWidth(), 0);
    EXPECT_GT(img.getHeight(), 0);
    EXPECT_LT(img.getWidth(), 10000);  // Sanity check
    EXPECT_LT(img.getHeight(), 10000);
}

/**
 * Test: Can read pixel data from loaded image
 * Purpose: Verify that loaded images have accessible pixel data
 */
TEST_F(ImageTest, LoadedImageHasAccessiblePixels) {
    Image img(testImagePath);

    if (img.getWidth() > 0 && img.getHeight() > 0) {
        // Should be able to read any pixel without crashing
        Color c = img.getPixel(0, 0);

        // Color values should be in valid range [0, 255]
        EXPECT_GE(c.red(), 0);
        EXPECT_LE(c.red(), 255);
        EXPECT_GE(c.green(), 0);
        EXPECT_LE(c.green(), 255);
        EXPECT_GE(c.blue(), 0);
        EXPECT_LE(c.blue(), 255);
    }
}

// ==============================================================================
// Main function - Entry point for test executable
// ==============================================================================

/**
 * Note: When using gtest_main in CMakeLists.txt, this main() is not needed.
 * GTest provides its own main function. This is here for reference only.
 */
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
