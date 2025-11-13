#include "export/ExportFacade.h"
#include "timeline/Timeline.h"
#include "video_writer.hpp"
#include <algorithm>
#include <iostream>

// STB image write for multiple formats
#include "stb_image_write.h"

namespace csci3081 {

ExportFacade::ExportFacade() : lastError("") {}

ExportFacade::~ExportFacade() {}

bool ExportFacade::exportImage(const Image& image, const std::string& filename,
                                const ExportSettings& settings) {
  lastError = "";

  // Check if image is valid
  if (image.getWidth() <= 0 || image.getHeight() <= 0) {
    lastError = "Invalid image: dimensions must be positive";
    return false;
  }

  // Check if filename is valid for format
  if (!isValidFilenameForFormat(filename, settings.format)) {
    lastError = "Filename extension does not match export format";
    return false;
  }

  // Resize if needed
  Image* processedImage = resizeIfNeeded(image, settings);

  // Write to file
  bool success = writeImageFile(*processedImage, filename, settings.format, settings.quality);

  // Clean up
  if (processedImage != &image) {
    delete processedImage;
  }

  return success;
}

bool ExportFacade::exportAsset(IAsset* asset, const std::string& filename,
                                const ExportSettings& settings) {
  if (!asset) {
    lastError = "Asset is null";
    return false;
  }

  // Get the current frame (at time 0.0)
  const Image& frame = asset->getFrame(0.0);

  return exportImage(frame, filename, settings);
}

bool ExportFacade::exportVideo(const std::vector<const Image*>& frames,
                                const std::string& filename,
                                const ExportSettings& settings) {
  if (frames.empty()) {
    lastError = "No frames to export";
    return false;
  }

  // Get dimensions from first frame
  int width = frames[0]->getWidth();
  int height = frames[0]->getHeight();
  int fps = static_cast<int>(settings.frameRate);

  std::cout << "Exporting video: " << filename << " (" << width << "x" << height
            << " @ " << fps << " fps, " << frames.size() << " frames)" << std::endl;

  // Initialize video writer
  VideoWriterState writer;
  if (!video_writer_open(&writer, filename.c_str(), width, height, fps)) {
    lastError = "Failed to open video writer";
    return false;
  }

  // Write each frame
  for (size_t i = 0; i < frames.size(); i++) {
    const Image* frame = frames[i];

    // Validate frame dimensions
    if (frame->getWidth() != width || frame->getHeight() != height) {
      std::cerr << "Warning: Frame " << i << " has different dimensions ("
                << frame->getWidth() << "x" << frame->getHeight()
                << "), expected (" << width << "x" << height << ")" << std::endl;
      // Skip this frame or resize it
      continue;
    }

    // Write frame
    if (!video_writer_write_frame(&writer, frame->getData())) {
      lastError = "Failed to write frame " + std::to_string(i);
      video_writer_close(&writer);
      return false;
    }

    // Progress indicator every 30 frames
    if (i % 30 == 0 || i == frames.size() - 1) {
      std::cout << "Encoded " << (i + 1) << "/" << frames.size() << " frames" << std::endl;
    }
  }

  // Close video writer
  video_writer_close(&writer);

  std::cout << "Video export complete: " << filename << std::endl;
  return true;
}

bool ExportFacade::exportTimeline(class Timeline* timeline,
                                   const std::string& filename,
                                   const ExportSettings& settings,
                                   int width,
                                   int height) {
  if (!timeline) {
    lastError = "Timeline is null";
    return false;
  }

  // For now, just export a single frame at time 0.0 as an image
  // TODO: Implement full video export with FFmpeg

  double duration = timeline->getTotalDuration();
  if (duration <= 0.0) {
    lastError = "Timeline has no duration (empty or no tracks)";
    return false;
  }

  // For image export, render the first frame
  if (settings.format != ExportFormat::MP4) {
    std::cout << "Exporting timeline as single frame image at time 0.0s" << std::endl;
    Image* frame = timeline->renderFrameAt(0.0, width, height);
    if (!frame) {
      lastError = "Failed to render timeline frame";
      return false;
    }

    bool success = exportImage(*frame, filename, settings);
    delete frame;
    return success;
  }

  // For MP4, we need to render all frames and export as video
  std::cout << "Preparing to export timeline as MP4 video..." << std::endl;
  std::cout << "Timeline duration: " << duration << "s" << std::endl;
  std::cout << "Frame rate: " << settings.frameRate << " fps" << std::endl;

  // Calculate number of frames
  int numFrames = static_cast<int>(duration * settings.frameRate);
  std::cout << "Total frames to render: " << numFrames << std::endl;

  // Render all frames
  std::vector<const Image*> frames;
  for (int i = 0; i < numFrames; i++) {
    double time = i / settings.frameRate;
    Image* frame = timeline->renderFrameAt(time, width, height);
    if (!frame) {
      lastError = "Failed to render frame at time " + std::to_string(time);

      // Clean up already-rendered frames
      for (const Image* f : frames) {
        delete f;
      }
      return false;
    }
    frames.push_back(frame);

    // Progress indicator
    if (i % 30 == 0) {
      std::cout << "Rendered " << i << "/" << numFrames << " frames..." << std::endl;
    }
  }

  std::cout << "All frames rendered, exporting video..." << std::endl;

  // Export the frames as video
  bool success = exportVideo(frames, filename, settings);

  // Clean up frames
  for (const Image* frame : frames) {
    delete frame;
  }

  return success;
}

std::string ExportFacade::getDefaultExtension(ExportFormat format) {
  switch (format) {
    case ExportFormat::PNG:
      return ".png";
    case ExportFormat::JPEG:
      return ".jpg";
    case ExportFormat::BMP:
      return ".bmp";
    case ExportFormat::PPM:
      return ".ppm";
    case ExportFormat::MP4:
      return ".mp4";
    default:
      return ".png";
  }
}

bool ExportFacade::isValidFilenameForFormat(const std::string& filename, ExportFormat format) {
  std::string ext = getDefaultExtension(format);

  // Convert filename to lowercase for comparison
  std::string lower = filename;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  // Check if filename ends with the expected extension
  if (lower.length() < ext.length()) {
    return false;
  }

  std::string fileExt = lower.substr(lower.length() - ext.length());

  // Also accept .jpeg for JPEG format
  if (format == ExportFormat::JPEG && fileExt == ".jpeg") {
    return true;
  }

  return fileExt == ext;
}

Image* ExportFacade::resizeIfNeeded(const Image& image, const ExportSettings& settings) {
  // If no resize requested, return copy
  if (settings.width <= 0 && settings.height <= 0) {
    return new Image(image);
  }

  // For now, just return a copy (resize not implemented)
  // TODO: Implement image resizing using bilinear or bicubic interpolation
  lastError = "Warning: Image resizing not yet implemented, using original size";
  return new Image(image);
}

Image* ExportFacade::convertFormat(const Image& image, ExportFormat targetFormat) {
  // For now, all our images are RGBA, so no conversion needed
  // Just return a copy
  return new Image(image);
}

bool ExportFacade::writeImageFile(const Image& image, const std::string& filename,
                                   ExportFormat format, int quality) {
  int width = image.getWidth();
  int height = image.getHeight();
  const unsigned char* data = image.getData();

  // Components is always 4 (RGBA) for our Image class
  int components = 4;

  int result = 0;

  switch (format) {
    case ExportFormat::PNG:
      result = stbi_write_png(filename.c_str(), width, height, components, data, width * components);
      break;

    case ExportFormat::JPEG:
      // JPEG doesn't support alpha, so we'd need to convert to RGB
      // For now, just write with alpha (STB will ignore it)
      result = stbi_write_jpg(filename.c_str(), width, height, components, data, quality);
      break;

    case ExportFormat::BMP:
      result = stbi_write_bmp(filename.c_str(), width, height, components, data);
      break;

    case ExportFormat::PPM:
      // PPM is not directly supported by STB, but we can use TGA instead
      // Or implement custom PPM writer
      result = stbi_write_tga(filename.c_str(), width, height, components, data);
      break;

    case ExportFormat::MP4:
      lastError = "Use exportVideo() for MP4 format";
      return false;

    default:
      lastError = "Unknown export format";
      return false;
  }

  if (result == 0) {
    lastError = "Failed to write image file (disk full or permission denied)";
    return false;
  }

  return true;
}

} // namespace csci3081
