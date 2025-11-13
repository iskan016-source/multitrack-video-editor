#ifndef EXPORT_IMAGE_COMMAND_H_
#define EXPORT_IMAGE_COMMAND_H_

#include "commands/ICommand.h"
#include "export/ExportFacade.h"
#include "Image.h"
#include <string>
#include <stdexcept>

namespace csci3081 {

/**
 * @brief Command to export an image to a file
 *
 * ExportImageCommand encapsulates the action of exporting an image
 * using the ExportFacade. It stores all parameters needed for the export.
 *
 * Design Pattern: Command Pattern
 * - ExportImageCommand is a ConcreteCommand
 * - ExportFacade is the Receiver that performs the actual work
 * - ICommand is the Command interface
 */
class ExportImageCommand : public ICommand {
public:
  /**
   * @brief Create an export image command
   * @param facade The export facade to use
   * @param image The image to export
   * @param filename Output filename
   * @param settings Export settings
   */
  ExportImageCommand(ExportFacade* facade, const Image* image,
                     const std::string& filename, const ExportSettings& settings)
    : facade(facade), image(image), filename(filename), settings(settings) {}

  virtual ~ExportImageCommand() {}

  /**
   * @brief Execute the export command
   */
  virtual void execute() override {
    if (!facade || !image) {
      throw std::runtime_error("Invalid facade or image pointer");
    }

    bool success = facade->exportImage(*image, filename, settings);
    if (!success) {
      throw std::runtime_error("Export failed: " + facade->getLastError());
    }
  }

  /**
   * @brief Get description of this command
   */
  virtual std::string getDescription() const override {
    return "Export image to " + filename;
  }

  /**
   * @brief Check if command can execute
   */
  virtual bool canExecute() const override {
    return facade != nullptr && image != nullptr && !filename.empty();
  }

private:
  ExportFacade* facade;
  const Image* image;
  std::string filename;
  ExportSettings settings;
};

} // namespace csci3081

#endif // EXPORT_IMAGE_COMMAND_H_
