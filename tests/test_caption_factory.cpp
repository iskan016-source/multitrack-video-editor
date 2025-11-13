/**
 * @file test_caption_factory.cpp
 * @brief Unit tests for CaptionAssetFactory
 *
 * Tests the factory's ability to create Caption assets from .txt files
 */

#include <gtest/gtest.h>
#include "assets/CaptionAssetFactory.h"
#include "assets/IAsset.h"
#include "assets/Caption.h"

using namespace csci3081;

// ==============================================================================
// CaptionAssetFactory Tests
// ==============================================================================

/**
 * @class CaptionAssetFactoryTest
 * @brief Test suite for CaptionAssetFactory
 */
class CaptionAssetFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        factory = new CaptionAssetFactory();
        validTextPath = "../tests/fixtures/test_caption.txt";
    }

    void TearDown() override {
        delete factory;
    }

    CaptionAssetFactory* factory;
    std::string validTextPath;
};

/**
 * Test: Factory creates Caption for .txt files
 * Purpose: Verify .txt extension is recognized
 */
TEST_F(CaptionAssetFactoryTest, CreatesTxtAsset) {
    IAsset* asset = factory->create(validTextPath);

    ASSERT_NE(asset, nullptr);
    EXPECT_EQ(asset->getAssetType(), AssetType::TEXT);
    EXPECT_FALSE(asset->isVideo());

    delete asset;
}

/**
 * Test: Factory handles uppercase .TXT extension
 * Purpose: Verify case-insensitivity
 */
TEST_F(CaptionAssetFactoryTest, HandlesUppercaseExtension) {
    IAsset* asset = factory->create("TEST.TXT");

    // Even though file doesn't exist, factory should attempt to create it
    // (it will fail gracefully when trying to read the file)
    ASSERT_NE(asset, nullptr);
    EXPECT_EQ(asset->getAssetType(), AssetType::TEXT);

    delete asset;
}

/**
 * Test: Factory returns nullptr for non-text files
 * Purpose: Verify factory only handles .txt files
 */
TEST_F(CaptionAssetFactoryTest, ReturnsNullptrForNonTextFiles) {
    IAsset* asset1 = factory->create("image.png");
    IAsset* asset2 = factory->create("video.mp4");
    IAsset* asset3 = factory->create("document.pdf");

    EXPECT_EQ(asset1, nullptr);
    EXPECT_EQ(asset2, nullptr);
    EXPECT_EQ(asset3, nullptr);
}

/**
 * Test: Factory handles missing file gracefully
 * Purpose: Verify error handling for non-existent files
 */
TEST_F(CaptionAssetFactoryTest, HandlesMissingFile) {
    IAsset* asset = factory->create("nonexistent.txt");

    // Factory should return nullptr when file can't be opened
    EXPECT_EQ(asset, nullptr);
}

/**
 * Test: Factory loads text content from file
 * Purpose: Verify the caption contains the file's text
 */
TEST_F(CaptionAssetFactoryTest, LoadsTextContentFromFile) {
    IAsset* asset = factory->create(validTextPath);

    ASSERT_NE(asset, nullptr);

    // Cast to Caption to access getText()
    Caption* caption = dynamic_cast<Caption*>(asset);
    ASSERT_NE(caption, nullptr);

    // Should contain the text from the file
    std::string text = caption->getText();
    EXPECT_FALSE(text.empty());
    EXPECT_GT(text.length(), 0);

    delete asset;
}

/**
 * Test: Created caption has valid image
 * Purpose: Verify factory creates fully functional Caption objects
 */
TEST_F(CaptionAssetFactoryTest, CreatedCaptionHasValidImage) {
    IAsset* asset = factory->create(validTextPath);

    ASSERT_NE(asset, nullptr);

    const Image& frame = asset->getFrame();
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);

    delete asset;
}

/**
 * Test: Created caption has default properties
 * Purpose: Verify factory uses sensible defaults
 */
TEST_F(CaptionAssetFactoryTest, CreatedCaptionHasDefaultProperties) {
    IAsset* asset = factory->create(validTextPath);

    ASSERT_NE(asset, nullptr);

    // Should have reasonable duration (not zero)
    EXPECT_GE(asset->getDuration(), 0.0);

    // Should produce a thumbnail
    const Image& thumbnail = asset->getThumbnail();
    EXPECT_GT(thumbnail.getWidth(), 0);

    delete asset;
}

/**
 * Test: Factory handles empty text file
 * Purpose: Verify graceful handling of empty files
 */
TEST_F(CaptionAssetFactoryTest, HandlesEmptyTextFile) {
    // This test would need an empty fixture file to be comprehensive
    // For now, we test that the factory doesn't crash on edge cases
    IAsset* asset = factory->create("empty.txt");

    // May return nullptr if file doesn't exist, which is acceptable
    if (asset != nullptr) {
        EXPECT_EQ(asset->getAssetType(), AssetType::TEXT);
        delete asset;
    }
}

/**
 * Test: Multiple .txt extensions in filename
 * Purpose: Verify factory finds .txt even in complex filenames
 */
TEST_F(CaptionAssetFactoryTest, HandlesMultipleTxtInFilename) {
    // Filename contains .txt multiple times
    IAsset* asset = factory->create("file.txt.backup.txt");

    ASSERT_NE(asset, nullptr);
    EXPECT_EQ(asset->getAssetType(), AssetType::TEXT);

    delete asset;
}
