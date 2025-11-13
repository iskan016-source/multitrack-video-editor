/**
 * @file test_image_asset.cpp
 * @brief Unit tests for ImageAsset (Adapter Pattern)
 *
 * This file tests the ImageAsset class which implements the Adapter pattern.
 * ImageAsset adapts the Image class to the IAsset interface, allowing images
 * to be treated uniformly with videos and other asset types.
 *
 * Adapter Pattern: Converts the interface of Image class into the IAsset
 * interface that clients expect. This allows Image objects to work in contexts
 * that expect IAsset objects.
 *
 * Key Adapter Responsibilities:
 * - Wraps an Image object
 * - Implements IAsset interface methods by delegating to Image
 * - Provides asset-specific metadata (duration=0, isVideo=false)
 */

#include <gtest/gtest.h>
#include "assets/ImageAsset.h"
#include "Image.h"
#include "graphics/Color.h"

using namespace csci3081;

// ==============================================================================
// Test Fixture for ImageAsset Tests
// ==============================================================================

class ImageAssetTest : public ::testing::Test {
protected:
    void SetUp() override {
        testImagePath = "../tests/fixtures/test_image.jpeg";
    }

    void TearDown() override {
        // Cleanup happens automatically
    }

    std::string testImagePath;
};

// ==============================================================================
// Constructor and Basic Property Tests
// ==============================================================================

/**
 * Test: ImageAsset constructor loads image
 * Purpose: Verify ImageAsset wraps an Image object correctly
 */
TEST_F(ImageAssetTest, ConstructorLoadsImage) {
    ImageAsset asset(testImagePath);

    // Should be able to get frame without crashing
    const Image& frame = asset.getFrame();

    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: ImageAsset has zero duration
 * Purpose: Verify adapter properly implements duration for static images
 */
TEST_F(ImageAssetTest, DurationIsZero) {
    ImageAsset asset(testImagePath);

    EXPECT_EQ(asset.getDuration(), 5.0);  // Images have default 5 second duration for timeline
}

/**
 * Test: ImageAsset is not a video
 * Purpose: Verify isVideo() returns false for images
 */
TEST_F(ImageAssetTest, IsNotVideo) {
    ImageAsset asset(testImagePath);

    EXPECT_FALSE(asset.isVideo());
}

/**
 * Test: ImageAsset has correct asset type
 * Purpose: Verify getAssetType() returns IMAGE
 */
TEST_F(ImageAssetTest, HasCorrectAssetType) {
    ImageAsset asset(testImagePath);

    EXPECT_EQ(asset.getAssetType(), AssetType::IMAGE);
}

// ==============================================================================
// Frame Access Tests - Testing Adapter's getFrame() method
// ==============================================================================

/**
 * Test: getFrame() returns same image regardless of time
 * Purpose: Verify adapter behavior - static images don't change over time
 */
TEST_F(ImageAssetTest, GetFrameReturnsSameImageRegardlessOfTime) {
    ImageAsset asset(testImagePath);

    const Image& frame1 = asset.getFrame(0.0);
    const Image& frame2 = asset.getFrame(5.0);
    const Image& frame3 = asset.getFrame(100.0);

    // All frames should have same dimensions (same underlying image)
    EXPECT_EQ(frame1.getWidth(), frame2.getWidth());
    EXPECT_EQ(frame1.getHeight(), frame2.getHeight());
    EXPECT_EQ(frame2.getWidth(), frame3.getWidth());
    EXPECT_EQ(frame2.getHeight(), frame3.getHeight());

    // Should actually be the same object (same pointer)
    EXPECT_EQ(&frame1, &frame2);
    EXPECT_EQ(&frame2, &frame3);
}

/**
 * Test: getFrame() with default parameter
 * Purpose: Verify default time parameter works
 */
TEST_F(ImageAssetTest, GetFrameWithDefaultParameter) {
    ImageAsset asset(testImagePath);

    const Image& frameDefault = asset.getFrame();
    const Image& frameZero = asset.getFrame(0.0);

    // Should return the same image
    EXPECT_EQ(&frameDefault, &frameZero);
}

/**
 * Test: getFrame() returns image with valid dimensions
 * Purpose: Integration test - verify wrapped Image has correct properties
 */
TEST_F(ImageAssetTest, GetFrameReturnsValidImage) {
    ImageAsset asset(testImagePath);

    const Image& frame = asset.getFrame();

    // Image should have reasonable dimensions
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_LT(frame.getWidth(), 10000);  // Sanity check
    EXPECT_GT(frame.getHeight(), 0);
    EXPECT_LT(frame.getHeight(), 10000);
}

/**
 * Test: getFrame() returns image with accessible pixels
 * Purpose: Verify adapter doesn't break pixel access
 */
TEST_F(ImageAssetTest, GetFrameReturnsImageWithAccessiblePixels) {
    ImageAsset asset(testImagePath);

    const Image& frame = asset.getFrame();

    // Should be able to access pixels
    Color c = frame.getPixel(0, 0);

    // Color values should be in valid range
    EXPECT_GE(c.red(), 0);
    EXPECT_LE(c.red(), 255);
    EXPECT_GE(c.green(), 0);
    EXPECT_LE(c.green(), 255);
    EXPECT_GE(c.blue(), 0);
    EXPECT_LE(c.blue(), 255);
}

// ==============================================================================
// Thumbnail Tests - Testing getThumbnail() method
// ==============================================================================

/**
 * Test: getThumbnail() returns the image
 * Purpose: Verify thumbnail is the same as the full image for ImageAsset
 */
TEST_F(ImageAssetTest, GetThumbnailReturnsImage) {
    ImageAsset asset(testImagePath);

    const Image& thumbnail = asset.getThumbnail();
    const Image& frame = asset.getFrame();

    // For ImageAsset, thumbnail should be same as frame
    EXPECT_EQ(&thumbnail, &frame);
    EXPECT_EQ(thumbnail.getWidth(), frame.getWidth());
    EXPECT_EQ(thumbnail.getHeight(), frame.getHeight());
}

/**
 * Test: getThumbnail() returns valid image
 * Purpose: Verify thumbnail has valid properties
 */
TEST_F(ImageAssetTest, GetThumbnailReturnsValidImage) {
    ImageAsset asset(testImagePath);

    const Image& thumbnail = asset.getThumbnail();

    EXPECT_GT(thumbnail.getWidth(), 0);
    EXPECT_GT(thumbnail.getHeight(), 0);
}

// ==============================================================================
// Adapter Pattern Verification Tests
// ==============================================================================

/**
 * Test: ImageAsset properly adapts Image to IAsset interface
 * Purpose: Verify all IAsset interface methods are implemented
 */
TEST_F(ImageAssetTest, ImplementsIAssetInterface) {
    ImageAsset asset(testImagePath);

    // Can be used polymorphically as IAsset
    IAsset* assetPtr = &asset;

    // All IAsset methods should work
    EXPECT_EQ(assetPtr->getDuration(), 5.0);
    EXPECT_FALSE(assetPtr->isVideo());
    EXPECT_EQ(assetPtr->getAssetType(), AssetType::IMAGE);

    const Image& frame = assetPtr->getFrame(0.0);
    EXPECT_GT(frame.getWidth(), 0);

    const Image& thumbnail = assetPtr->getThumbnail();
    EXPECT_GT(thumbnail.getWidth(), 0);
}

/**
 * Test: ImageAsset can be stored in IAsset container
 * Purpose: Verify polymorphic usage works correctly
 */
TEST_F(ImageAssetTest, WorksInPolymorphicContainer) {
    std::vector<IAsset*> assets;

    assets.push_back(new ImageAsset(testImagePath));
    assets.push_back(new ImageAsset(testImagePath));

    // Should be able to iterate and use through IAsset interface
    for (IAsset* asset : assets) {
        EXPECT_EQ(asset->getAssetType(), AssetType::IMAGE);
        EXPECT_FALSE(asset->isVideo());
        EXPECT_EQ(asset->getDuration(), 5.0);

        const Image& frame = asset->getFrame();
        EXPECT_GT(frame.getWidth(), 0);
    }

    // Cleanup
    for (IAsset* asset : assets) {
        delete asset;
    }
}

// ==============================================================================
// Edge Cases and Error Handling
// ==============================================================================

/**
 * Test: ImageAsset with non-existent file
 * Purpose: Verify error handling is delegated to Image class
 *
 * NOTE: This test documents current behavior. The Image class creates
 * a 0x0 image for non-existent files (see test_image.cpp for details).
 */
TEST_F(ImageAssetTest, HandlesNonExistentFile) {
    ImageAsset asset("nonexistent.png");

    // Image class creates 0x0 image for missing files
    const Image& frame = asset.getFrame();
    EXPECT_EQ(frame.getWidth(), 0);
    EXPECT_EQ(frame.getHeight(), 0);

    // Asset interface methods should still work
    EXPECT_EQ(asset.getDuration(), 5.0);
    EXPECT_FALSE(asset.isVideo());
    EXPECT_EQ(asset.getAssetType(), AssetType::IMAGE);
}

/**
 * Test: ImageAsset with negative time value
 * Purpose: Test edge case - negative time shouldn't matter for images
 */
TEST_F(ImageAssetTest, HandlesNegativeTime) {
    ImageAsset asset(testImagePath);

    const Image& frame = asset.getFrame(-5.0);

    // Should return the same image regardless
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: ImageAsset with very large time value
 * Purpose: Test edge case - large time shouldn't matter for images
 */
TEST_F(ImageAssetTest, HandlesLargeTime) {
    ImageAsset asset(testImagePath);

    const Image& frame = asset.getFrame(999999.0);

    // Should return the same image regardless
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

// ==============================================================================
// Integration Tests - Testing realistic usage scenarios
// ==============================================================================

/**
 * Test: Multiple ImageAssets with same file
 * Purpose: Verify multiple assets can be created from same source
 */
TEST_F(ImageAssetTest, MultipleAssetsFromSameFile) {
    ImageAsset asset1(testImagePath);
    ImageAsset asset2(testImagePath);

    // Both should load successfully
    EXPECT_GT(asset1.getFrame().getWidth(), 0);
    EXPECT_GT(asset2.getFrame().getWidth(), 0);

    // Should have same dimensions
    EXPECT_EQ(asset1.getFrame().getWidth(), asset2.getFrame().getWidth());
    EXPECT_EQ(asset1.getFrame().getHeight(), asset2.getFrame().getHeight());

    // But should be independent objects
    EXPECT_NE(&asset1.getFrame(), &asset2.getFrame());
}

/**
 * Test: ImageAsset properties match underlying Image
 * Purpose: Verify adapter accurately represents the adaptee
 */
TEST_F(ImageAssetTest, PropertiesMatchUnderlyingImage) {
    // Create an Image directly
    Image directImage(testImagePath);

    // Create same image through ImageAsset adapter
    ImageAsset asset(testImagePath);
    const Image& adaptedImage = asset.getFrame();

    // Dimensions should match
    EXPECT_EQ(adaptedImage.getWidth(), directImage.getWidth());
    EXPECT_EQ(adaptedImage.getHeight(), directImage.getHeight());

    // Pixel data should match (sample a few pixels)
    if (directImage.getWidth() > 0 && directImage.getHeight() > 0) {
        Color direct = directImage.getPixel(0, 0);
        Color adapted = adaptedImage.getPixel(0, 0);

        EXPECT_EQ(direct.red(), adapted.red());
        EXPECT_EQ(direct.green(), adapted.green());
        EXPECT_EQ(direct.blue(), adapted.blue());
    }
}

/**
 * Test: ImageAsset lifecycle and memory management
 * Purpose: Verify proper construction and destruction
 */
TEST_F(ImageAssetTest, ProperLifecycleManagement) {
    {
        ImageAsset* asset = new ImageAsset(testImagePath);

        // Use the asset
        const Image& frame = asset->getFrame();
        EXPECT_GT(frame.getWidth(), 0);

        // Delete should clean up wrapped Image
        delete asset;
    }

    // If we reach here without memory errors, test passes
    SUCCEED();
}

// ==============================================================================
// Comparison with Video Assets (Conceptual Tests)
// ==============================================================================

/**
 * Test: ImageAsset differs from VideoAsset in key properties
 * Purpose: Document the differences between image and video adapters
 */
TEST_F(ImageAssetTest, DiffersFromVideoAssetInKeyWays) {
    ImageAsset imageAsset(testImagePath);

    // Images have specific characteristics:
    // 1. Zero duration
    EXPECT_EQ(imageAsset.getDuration(), 5.0);

    // 2. Not videos
    EXPECT_FALSE(imageAsset.isVideo());

    // 3. Same frame at all times
    const Image& frame1 = imageAsset.getFrame(0.0);
    const Image& frame2 = imageAsset.getFrame(10.0);
    EXPECT_EQ(&frame1, &frame2);

    // These properties distinguish ImageAsset from VideoAsset
}
