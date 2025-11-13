#ifndef TIMELINE_ENTRY_H_
#define TIMELINE_ENTRY_H_

#include "assets/IAsset.h"

namespace csci3081 {

/**
 * @brief Represents an asset placed on a track with timing information
 *
 * TimelineEntry encapsulates an asset along with its position in time.
 * Each entry has a start time and duration, defining when and for how long
 * the asset should be displayed.
 *
 * For videos, the duration can be the full video length or trimmed.
 * For images, the duration determines how long the image is shown.
 */
class TimelineEntry {
public:
  /**
   * @brief Create a timeline entry
   * @param asset The asset to place on timeline (not owned by entry)
   * @param startTime When the asset should start (in seconds)
   * @param duration How long the asset should play (in seconds)
   */
  TimelineEntry(IAsset* asset, double startTime, double duration);

  /**
   * @brief Get the asset
   * @return Pointer to the asset
   */
  IAsset* getAsset() const { return asset; }

  /**
   * @brief Get the start time
   * @return Start time in seconds
   */
  double getStartTime() const { return startTime; }

  /**
   * @brief Get the duration
   * @return Duration in seconds
   */
  double getDuration() const { return duration; }

  /**
   * @brief Get the end time (start + duration)
   * @return End time in seconds
   */
  double getEndTime() const { return startTime + duration; }

  /**
   * @brief Set the start time
   * @param time New start time in seconds
   */
  void setStartTime(double time) { startTime = time; }

  /**
   * @brief Set the duration
   * @param dur New duration in seconds
   */
  void setDuration(double dur) { duration = dur; }

  /**
   * @brief Check if this entry is active at a given time
   * @param time The time to check (in seconds)
   * @return true if time is within [startTime, endTime), false otherwise
   */
  bool isActiveAt(double time) const {
    return time >= startTime && time < getEndTime();
  }

  /**
   * @brief Get the frame for this entry at a given global time
   * @param globalTime The global timeline time (in seconds)
   * @return The image frame to display
   */
  const Image& getFrameAt(double globalTime) const {
    // Convert global time to asset-local time
    double localTime = globalTime - startTime;
    return asset->getFrame(localTime);
  }

  /**
   * @brief Check if this entry overlaps with another entry
   * @param other The other entry to check
   * @return true if entries overlap in time, false otherwise
   */
  bool overlapsWith(const TimelineEntry& other) const {
    // Two intervals overlap if: start1 < end2 AND start2 < end1
    return startTime < other.getEndTime() && other.startTime < getEndTime();
  }

private:
  IAsset* asset;      // Not owned - Application manages asset lifetime
  double startTime;   // Start time in seconds
  double duration;    // Duration in seconds
};

} // namespace csci3081

#endif // TIMELINE_ENTRY_H_
