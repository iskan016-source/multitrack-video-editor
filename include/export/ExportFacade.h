#ifndef EXPORT_FACADE_H_
#define EXPORT_FACADE_H_

#include "Image.h"
#include "assets/IAsset.h"
#include <string>
#include <vector>

namespace csci3081 {

/**
 * @brief Supported export formats
 */
enum class ExportFormat {
  PNG,
  JPEG,
  BMP,
  PPM,
  MP4
};

/**
 * @brief Export quality settings
 */
struct ExportSettings {
  ExportFormat format;
  int quality;        // 0-100 for JPEG, ignored for other formats
  int width;          // Output width, -1 to keep original
  int height;         // Output height, -1 to keep original
  double frameRate;   // For video exports only

  ExportSettings()
    : format(ExportFormat::PNG), quality(90), width(-1), height(-1), frameRate(30.0) {}
};

/**
 * @brief Facade for export operations
 *
 * ExportFacade provides a simplified interface for complex export operations.
 * It hides the complexity of different file formats, encoding, and error handling.
 *
 * Design Pattern: Facade Pattern
 * - ExportFacade is the Facade that provides a simple interface
 * - It coordinates multiple subsystems: Image I/O, Video encoding, format conversion
 * - Clients use ExportFacade instead of dealing with low-level details
 *
 * The facade also serves as the Receiver in the Command pattern, providing
 * the actual operations that commands will invoke.
 */
class ExportFacade {
public:
  ExportFacade();
  ~ExportFacade();

  /**
   * @brief Export a single image to a file
   * @param image The image to export
   * @param filename Output filename (with extension)
   * @param settings Export settings
   * @return true if export succeeded, false otherwise
   */
  bool exportImage(const Image& image, const std::string& filename,
                   const ExportSettings& settings = ExportSettings());

  /**
   * @brief Export an asset to a file
   * @param asset The asset to export
   * @param filename Output filename
   * @param settings Export settings
   * @return true if export succeeded, false otherwise
   */
  bool exportAsset(IAsset* asset, const std::string& filename,
                   const ExportSettings& settings = ExportSettings());

  /**
   * @brief Export multiple frames as a video
   * @param frames Vector of images to export as video
   * @param filename Output filename (should have .mp4 extension)
   * @param settings Export settings (frameRate is important)
   * @return true if export succeeded, false otherwise
   */
  bool exportVideo(const std::vector<const Image*>& frames,
                   const std::string& filename,
                   const ExportSettings& settings = ExportSettings());

  /**
   * @brief Export a timeline as a video by rendering each frame
   * @param timeline The timeline to export
   * @param filename Output filename (should have .mp4 extension)
   * @param settings Export settings (frameRate is important)
   * @param width Width of output video
   * @param height Height of output video
   * @return true if export succeeded, false otherwise
   */
  bool exportTimeline(class Timeline* timeline,
                      const std::string& filename,
                      const ExportSettings& settings,
                      int width,
                      int height);

  /**
   * @brief Get the last error message
   * @return String describing the last error, or empty if no error
   */
  std::string getLastError() const { return lastError; }

  /**
   * @brief Check if a filename has a valid extension for the given format
   * @param filename The filename to check
   * @param format The expected format
   * @return true if filename matches format, false otherwise
   */
  static bool isValidFilenameForFormat(const std::string& filename, ExportFormat format);

  /**
   * @brief Get the default file extension for a format
   * @param format The export format
   * @return File extension (with dot, e.g., ".png")
   */
  static std::string getDefaultExtension(ExportFormat format);

private:
  std::string lastError;

  /**
   * @brief Resize an image if needed based on settings
   * @param image Input image
   * @param settings Export settings with width/height
   * @return Resized image, or copy of original if no resize needed
   */
  Image* resizeIfNeeded(const Image& image, const ExportSettings& settings);

  /**
   * @brief Convert image format if needed
   * @param image Input image
   * @param targetFormat Target format
   * @return Converted image, or copy of original if no conversion needed
   */
  Image* convertFormat(const Image& image, ExportFormat targetFormat);

  /**
   * @brief Write image to file using STB library
   * @param image The image to write
   * @param filename Output filename
   * @param format Output format
   * @param quality JPEG quality (ignored for other formats)
   * @return true if write succeeded, false otherwise
   */
  bool writeImageFile(const Image& image, const std::string& filename,
                      ExportFormat format, int quality);
};

} // namespace csci3081

#endif // EXPORT_FACADE_H_
