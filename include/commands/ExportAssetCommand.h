#ifndef EXPORT_ASSET_COMMAND_H_
#define EXPORT_ASSET_COMMAND_H_

#include "commands/ICommand.h"
#include "export/ExportFacade.h"
#include "assets/IAsset.h"
#include <string>
#include <stdexcept>

namespace csci3081 {

/**
 * @brief Command to export an asset to a file
 *
 * ExportAssetCommand encapsulates the action of exporting an asset
 * (which may be an image, video, or text) using the ExportFacade.
 *
 * Design Pattern: Command Pattern
 * - ExportAssetCommand is a ConcreteCommand
 * - ExportFacade is the Receiver
 * - ICommand is the Command interface
 */
class ExportAssetCommand : public ICommand {
public:
  /**
   * @brief Create an export asset command
   * @param facade The export facade to use
   * @param asset The asset to export
   * @param filename Output filename
   * @param settings Export settings
   */
  ExportAssetCommand(ExportFacade* facade, IAsset* asset,
                     const std::string& filename, const ExportSettings& settings)
    : facade(facade), asset(asset), filename(filename), settings(settings) {}

  virtual ~ExportAssetCommand() {}

  /**
   * @brief Execute the export command
   */
  virtual void execute() override {
    if (!facade || !asset) {
      throw std::runtime_error("Invalid facade or asset pointer");
    }

    bool success = facade->exportAsset(asset, filename, settings);
    if (!success) {
      throw std::runtime_error("Export failed: " + facade->getLastError());
    }
  }

  /**
   * @brief Get description of this command
   */
  virtual std::string getDescription() const override {
    return "Export asset to " + filename;
  }

  /**
   * @brief Check if command can execute
   */
  virtual bool canExecute() const override {
    return facade != nullptr && asset != nullptr && !filename.empty();
  }

private:
  ExportFacade* facade;
  IAsset* asset;
  std::string filename;
  ExportSettings settings;
};

} // namespace csci3081

#endif // EXPORT_ASSET_COMMAND_H_
