#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include <string>

namespace csci3081 {

/**
 * @brief Command pattern interface for encapsulating actions
 *
 * ICommand defines the interface for all command objects in the system.
 * Commands encapsulate an action and its parameters as an object, allowing
 * for parameterization of operations, queuing, logging, and undo/redo support.
 *
 * Design Pattern: Command Pattern
 * - ICommand is the Command interface
 * - Concrete commands (ExportImageCommand, ExportVideoCommand) are ConcreteCommands
 * - ExportFacade acts as the Receiver that knows how to perform the operations
 */
class ICommand {
public:
  virtual ~ICommand() {}

  /**
   * @brief Execute the command
   *
   * This method performs the action encapsulated by the command.
   * It may throw exceptions if the operation fails.
   */
  virtual void execute() = 0;

  /**
   * @brief Get a description of what this command does
   * @return String description of the command
   */
  virtual std::string getDescription() const = 0;

  /**
   * @brief Check if the command can be executed in the current state
   * @return true if command can execute, false otherwise
   */
  virtual bool canExecute() const = 0;
};

} // namespace csci3081

#endif // ICOMMAND_H_
