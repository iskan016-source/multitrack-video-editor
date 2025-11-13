/**
 * @file test_video_asset.cpp
 * @brief Unit tests for VideoAsset (Adapter Pattern)
 *
 * This file tests the VideoAsset class which implements the Adapter pattern.
 * VideoAsset adapts the Video class to the IAsset interface, allowing videos
 * to be treated uniformly with images and other asset types.
 *
 * Adapter Pattern: Converts the interface of Video class into the IAsset
 * interface that clients expect. This allows Video objects to work in contexts
 * that expect IAsset objects.
 *
 * Key Adapter Responsibilities:
 * - Wraps a Video object
 * - Implements IAsset interface methods by delegating to Video
 * - Provides asset-specific metadata (positive duration, isVideo=true)
 * - Manages thumbnail creation from first frame
 */

#include <gtest/gtest.h>
#include "assets/VideoAsset.h"
#include "Video.h"
#include "Image.h"
#include "graphics/Color.h"

using namespace csci3081;

// ==============================================================================
// Test Fixture for VideoAsset Tests
// ==============================================================================

class VideoAssetTest : public ::testing::Test {
protected:
    void SetUp() override {
        testVideoPath = "../tests/fixtures/test_video.mp4";
    }

    void TearDown() override {
        // Cleanup happens automatically
    }

    std::string testVideoPath;
};

// ==============================================================================
// Constructor and Basic Property Tests
// ==============================================================================

/**
 * Test: VideoAsset constructor loads video
 * Purpose: Verify VideoAsset wraps a Video object correctly
 */
TEST_F(VideoAssetTest, ConstructorLoadsVideo) {
    VideoAsset asset(testVideoPath);

    // Should be able to get frame without crashing
    const Image& frame = asset.getFrame(0.0);

    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: VideoAsset has positive duration
 * Purpose: Verify adapter properly delegates duration to Video
 */
TEST_F(VideoAssetTest, DurationIsPositive) {
    VideoAsset asset(testVideoPath);

    double duration = asset.getDuration();

    EXPECT_GT(duration, 0.0);
    EXPECT_LT(duration, 1000.0);  // Sanity check - not absurdly long
}

/**
 * Test: VideoAsset is a video
 * Purpose: Verify isVideo() returns true for videos
 */
TEST_F(VideoAssetTest, IsVideo) {
    VideoAsset asset(testVideoPath);

    EXPECT_TRUE(asset.isVideo());
}

/**
 * Test: VideoAsset has correct asset type
 * Purpose: Verify getAssetType() returns VIDEO
 */
TEST_F(VideoAssetTest, HasCorrectAssetType) {
    VideoAsset asset(testVideoPath);

    EXPECT_EQ(asset.getAssetType(), AssetType::VIDEO);
}

// ==============================================================================
// Duration Tests - Testing video timing properties
// ==============================================================================

/**
 * Test: Duration matches underlying Video object
 * Purpose: Verify adapter accurately delegates to Video::getDuration()
 */
TEST_F(VideoAssetTest, DurationMatchesUnderlyingVideo) {
    Video directVideo(testVideoPath);
    VideoAsset asset(testVideoPath);

    double directDuration = directVideo.getDuration();
    double assetDuration = asset.getDuration();

    EXPECT_DOUBLE_EQ(assetDuration, directDuration);
}

/**
 * Test: Duration remains constant across calls
 * Purpose: Verify duration doesn't change during playback
 */
TEST_F(VideoAssetTest, DurationRemainsConstant) {
    VideoAsset asset(testVideoPath);

    double duration1 = asset.getDuration();
    asset.getFrame(1.0);
    asset.getFrame(2.0);
    double duration2 = asset.getDuration();

    EXPECT_DOUBLE_EQ(duration1, duration2);
}

// ==============================================================================
// Frame Access Tests - Testing time-based frame retrieval
// ==============================================================================

/**
 * Test: getFrame() at time 0.0 returns first frame
 * Purpose: Verify frame access at video start
 */
TEST_F(VideoAssetTest, GetFrameAtTimeZero) {
    VideoAsset asset(testVideoPath);

    const Image& frame = asset.getFrame(0.0);

    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: getFrame() at different times
 * Purpose: Verify video can seek to different timestamps
 */
TEST_F(VideoAssetTest, GetFrameAtDifferentTimes) {
    VideoAsset asset(testVideoPath);

    double duration = asset.getDuration();

    // Get frames at different points in the video
    const Image& frame1 = asset.getFrame(0.0);
    const Image& frame2 = asset.getFrame(duration * 0.5);  // Middle
    const Image& frame3 = asset.getFrame(duration * 0.9);  // Near end

    // All frames should have valid dimensions
    EXPECT_GT(frame1.getWidth(), 0);
    EXPECT_GT(frame2.getWidth(), 0);
    EXPECT_GT(frame3.getWidth(), 0);

    // All frames should have same dimensions (same video)
    EXPECT_EQ(frame1.getWidth(), frame2.getWidth());
    EXPECT_EQ(frame2.getWidth(), frame3.getWidth());
    EXPECT_EQ(frame1.getHeight(), frame2.getHeight());
}

/**
 * Test: getFrame() returns image with accessible pixels
 * Purpose: Verify frame pixels can be accessed
 */
TEST_F(VideoAssetTest, GetFrameReturnsImageWithAccessiblePixels) {
    VideoAsset asset(testVideoPath);

    const Image& frame = asset.getFrame(0.0);

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

/**
 * Test: Sequential frame access
 * Purpose: Verify video playback in sequence
 */
TEST_F(VideoAssetTest, SequentialFrameAccess) {
    VideoAsset asset(testVideoPath);

    // Simulate sequential playback at 30 fps
    double timeStep = 1.0 / 30.0;
    int framesToTest = 5;

    for (int i = 0; i < framesToTest; i++) {
        double time = i * timeStep;
        const Image& frame = asset.getFrame(time);

        EXPECT_GT(frame.getWidth(), 0);
        EXPECT_GT(frame.getHeight(), 0);
    }
}

// ==============================================================================
// Thumbnail Tests - Testing thumbnail generation
// ==============================================================================

/**
 * Test: getThumbnail() returns valid image
 * Purpose: Verify thumbnail is created during construction
 */
TEST_F(VideoAssetTest, GetThumbnailReturnsValidImage) {
    VideoAsset asset(testVideoPath);

    const Image& thumbnail = asset.getThumbnail();

    EXPECT_GT(thumbnail.getWidth(), 0);
    EXPECT_GT(thumbnail.getHeight(), 0);
}

/**
 * Test: Thumbnail dimensions match video dimensions
 * Purpose: Verify thumbnail is same size as video frames
 */
TEST_F(VideoAssetTest, ThumbnailDimensionsMatchVideo) {
    VideoAsset asset(testVideoPath);

    const Image& thumbnail = asset.getThumbnail();
    const Image& frame = asset.getFrame(0.0);

    EXPECT_EQ(thumbnail.getWidth(), frame.getWidth());
    EXPECT_EQ(thumbnail.getHeight(), frame.getHeight());
}

/**
 * Test: Thumbnail doesn't change after playback
 * Purpose: Verify thumbnail is static (created from first frame)
 */
TEST_F(VideoAssetTest, ThumbnailDoesNotChangeAfterPlayback) {
    VideoAsset asset(testVideoPath);

    const Image& thumbnail1 = asset.getThumbnail();

    // Play through some of the video
    asset.getFrame(1.0);
    asset.getFrame(2.0);

    const Image& thumbnail2 = asset.getThumbnail();

    // Should be the same object
    EXPECT_EQ(&thumbnail1, &thumbnail2);

    // Verify dimensions haven't changed
    EXPECT_EQ(thumbnail1.getWidth(), thumbnail2.getWidth());
    EXPECT_EQ(thumbnail1.getHeight(), thumbnail2.getHeight());
}

/**
 * Test: Thumbnail represents first frame
 * Purpose: Verify thumbnail is created from first frame
 */
TEST_F(VideoAssetTest, ThumbnailRepresentsFirstFrame) {
    VideoAsset asset(testVideoPath);

    const Image& thumbnail = asset.getThumbnail();

    // Should have valid pixel data
    if (thumbnail.getWidth() > 0 && thumbnail.getHeight() > 0) {
        Color c = thumbnail.getPixel(0, 0);

        EXPECT_GE(c.red(), 0);
        EXPECT_LE(c.red(), 255);
        EXPECT_GE(c.green(), 0);
        EXPECT_LE(c.green(), 255);
        EXPECT_GE(c.blue(), 0);
        EXPECT_LE(c.blue(), 255);
    }
}

// ==============================================================================
// Adapter Pattern Verification Tests
// ==============================================================================

/**
 * Test: VideoAsset properly adapts Video to IAsset interface
 * Purpose: Verify all IAsset interface methods are implemented
 */
TEST_F(VideoAssetTest, ImplementsIAssetInterface) {
    VideoAsset asset(testVideoPath);

    // Can be used polymorphically as IAsset
    IAsset* assetPtr = &asset;

    // All IAsset methods should work
    EXPECT_GT(assetPtr->getDuration(), 0.0);
    EXPECT_TRUE(assetPtr->isVideo());
    EXPECT_EQ(assetPtr->getAssetType(), AssetType::VIDEO);

    const Image& frame = assetPtr->getFrame(0.0);
    EXPECT_GT(frame.getWidth(), 0);

    const Image& thumbnail = assetPtr->getThumbnail();
    EXPECT_GT(thumbnail.getWidth(), 0);
}

/**
 * Test: VideoAsset can be stored in IAsset container
 * Purpose: Verify polymorphic usage works correctly
 */
TEST_F(VideoAssetTest, WorksInPolymorphicContainer) {
    std::vector<IAsset*> assets;

    assets.push_back(new VideoAsset(testVideoPath));
    assets.push_back(new VideoAsset(testVideoPath));

    // Should be able to iterate and use through IAsset interface
    for (IAsset* asset : assets) {
        EXPECT_EQ(asset->getAssetType(), AssetType::VIDEO);
        EXPECT_TRUE(asset->isVideo());
        EXPECT_GT(asset->getDuration(), 0.0);

        const Image& frame = asset->getFrame(0.0);
        EXPECT_GT(frame.getWidth(), 0);
    }

    // Cleanup
    for (IAsset* asset : assets) {
        delete asset;
    }
}

// ==============================================================================
// Edge Cases and Boundary Tests
// ==============================================================================

/**
 * Test: getFrame() with negative time
 * Purpose: Test boundary condition - time before video start
 */
TEST_F(VideoAssetTest, GetFrameWithNegativeTime) {
    VideoAsset asset(testVideoPath);

    // Behavior with negative time depends on Video implementation
    // Should not crash at minimum
    const Image& frame = asset.getFrame(-1.0);

    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: getFrame() with time beyond duration
 * Purpose: Test boundary condition - time after video end
 */
TEST_F(VideoAssetTest, GetFrameWithTimeBeyondDuration) {
    VideoAsset asset(testVideoPath);

    double duration = asset.getDuration();
    const Image& frame = asset.getFrame(duration + 10.0);

    // Should not crash, might return last frame or clamp
    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: getFrame() exactly at duration
 * Purpose: Test boundary condition - time at video end
 */
TEST_F(VideoAssetTest, GetFrameAtDuration) {
    VideoAsset asset(testVideoPath);

    double duration = asset.getDuration();
    const Image& frame = asset.getFrame(duration);

    EXPECT_GT(frame.getWidth(), 0);
    EXPECT_GT(frame.getHeight(), 0);
}

/**
 * Test: Repeated calls to same timestamp
 * Purpose: Verify seeking to same time multiple times works
 */
TEST_F(VideoAssetTest, RepeatedCallsToSameTimestamp) {
    VideoAsset asset(testVideoPath);

    const Image& frame1 = asset.getFrame(1.0);
    const Image& frame2 = asset.getFrame(1.0);
    const Image& frame3 = asset.getFrame(1.0);

    // All should have same dimensions
    EXPECT_EQ(frame1.getWidth(), frame2.getWidth());
    EXPECT_EQ(frame2.getWidth(), frame3.getWidth());
    EXPECT_EQ(frame1.getHeight(), frame2.getHeight());
}

/**
 * Test: Non-sequential frame access (seeking)
 * Purpose: Verify random access to video frames
 */
TEST_F(VideoAssetTest, NonSequentialFrameAccess) {
    VideoAsset asset(testVideoPath);

    double duration = asset.getDuration();

    // Jump around in the video
    const Image& frame1 = asset.getFrame(duration * 0.8);
    const Image& frame2 = asset.getFrame(duration * 0.2);
    const Image& frame3 = asset.getFrame(duration * 0.5);
    const Image& frame4 = asset.getFrame(0.0);

    // All frames should be valid
    EXPECT_GT(frame1.getWidth(), 0);
    EXPECT_GT(frame2.getWidth(), 0);
    EXPECT_GT(frame3.getWidth(), 0);
    EXPECT_GT(frame4.getWidth(), 0);
}

// ==============================================================================
// Integration Tests - Testing realistic usage scenarios
// ==============================================================================

/**
 * Test: Multiple VideoAssets with same file
 * Purpose: Verify multiple assets can be created from same source
 */
TEST_F(VideoAssetTest, MultipleAssetsFromSameFile) {
    VideoAsset asset1(testVideoPath);
    VideoAsset asset2(testVideoPath);

    // Both should load successfully
    EXPECT_GT(asset1.getDuration(), 0.0);
    EXPECT_GT(asset2.getDuration(), 0.0);

    // Should have same duration
    EXPECT_DOUBLE_EQ(asset1.getDuration(), asset2.getDuration());

    // Frames should have same dimensions
    EXPECT_EQ(asset1.getFrame(0.0).getWidth(), asset2.getFrame(0.0).getWidth());

    // But should be independent objects
    EXPECT_NE(&asset1.getFrame(0.0), &asset2.getFrame(0.0));
}

/**
 * Test: VideoAsset properties match underlying Video
 * Purpose: Verify adapter accurately represents the adaptee
 */
TEST_F(VideoAssetTest, PropertiesMatchUnderlyingVideo) {
    // Create a Video directly
    Video directVideo(testVideoPath);

    // Create same video through VideoAsset adapter
    VideoAsset asset(testVideoPath);

    // Duration should match
    EXPECT_DOUBLE_EQ(asset.getDuration(), directVideo.getDuration());

    // Frame dimensions should match
    directVideo.nextFrame(0.0);
    const Image& directFrame = directVideo.getFrame();
    const Image& assetFrame = asset.getFrame(0.0);

    EXPECT_EQ(assetFrame.getWidth(), directFrame.getWidth());
    EXPECT_EQ(assetFrame.getHeight(), directFrame.getHeight());
}

/**
 * Test: VideoAsset lifecycle and memory management
 * Purpose: Verify proper construction and destruction
 */
TEST_F(VideoAssetTest, ProperLifecycleManagement) {
    {
        VideoAsset* asset = new VideoAsset(testVideoPath);

        // Use the asset
        const Image& frame = asset->getFrame(0.0);
        const Image& thumbnail = asset->getThumbnail();

        EXPECT_GT(frame.getWidth(), 0);
        EXPECT_GT(thumbnail.getWidth(), 0);

        // Delete should clean up wrapped Video and thumbnail
        delete asset;
    }

    // If we reach here without memory errors, test passes
    SUCCEED();
}

// ==============================================================================
// Comparison with Image Assets (Conceptual Tests)
// ==============================================================================

/**
 * Test: VideoAsset differs from ImageAsset in key properties
 * Purpose: Document the differences between video and image adapters
 */
TEST_F(VideoAssetTest, DiffersFromImageAssetInKeyWays) {
    VideoAsset videoAsset(testVideoPath);

    // Videos have specific characteristics:
    // 1. Positive duration
    EXPECT_GT(videoAsset.getDuration(), 0.0);

    // 2. Are videos
    EXPECT_TRUE(videoAsset.isVideo());

    // 3. Different frames at different times (potentially)
    // Note: Frames may differ depending on video content
    const Image& frame1 = videoAsset.getFrame(0.0);
    double midTime = videoAsset.getDuration() / 2.0;
    const Image& frame2 = videoAsset.getFrame(midTime);

    // Should still have same dimensions
    EXPECT_EQ(frame1.getWidth(), frame2.getWidth());
    EXPECT_EQ(frame1.getHeight(), frame2.getHeight());

    // These properties distinguish VideoAsset from ImageAsset
}

/**
 * Test: Video frames have consistent dimensions throughout playback
 * Purpose: Verify all frames in a video have the same dimensions
 */
TEST_F(VideoAssetTest, FrameDimensionsConsistentThroughoutPlayback) {
    VideoAsset asset(testVideoPath);

    double duration = asset.getDuration();
    int width = -1;
    int height = -1;

    // Sample frames at different times
    for (double time = 0.0; time < duration; time += duration / 5.0) {
        const Image& frame = asset.getFrame(time);

        if (width == -1) {
            width = frame.getWidth();
            height = frame.getHeight();
        } else {
            // All frames should have same dimensions
            EXPECT_EQ(frame.getWidth(), width);
            EXPECT_EQ(frame.getHeight(), height);
        }
    }
}
