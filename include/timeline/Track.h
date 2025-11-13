#ifndef TRACK_H_
#define TRACK_H_

#include "timeline/TimelineEntry.h"
#include "graphics/Color.h"
#include <vector>
#include <string>
#include <algorithm>

namespace csci3081 {

/**
 * @brief Represents a single track/layer in the timeline
 *
 * Track is a layer that contains multiple TimelineEntry objects arranged
 * sequentially in time. Entries on the same track CANNOT overlap - this is
 * enforced by the addEntry() method.
 *
 * Tracks are rendered in order (track 0 first, then track 1 on top, etc.)
 * to create the final composite image.
 */
class Track {
public:
  /**
   * @brief Create a track with optional name and color
   * @param name Track name (e.g., "Video Layer 1")
   * @param color Track color for UI visualization
   */
  Track(const std::string& name = "Track", const Color& color = Color(100, 100, 200, 255));

  ~Track();

  /**
   * @brief Add an entry to the track
   * @param entry The timeline entry to add
   * @return true if added successfully, false if it would overlap with existing entry
   */
  bool addEntry(const TimelineEntry& entry);

  /**
   * @brief Remove an entry at a specific index
   * @param index Index of entry to remove
   * @return true if removed, false if index out of bounds
   */
  bool removeEntry(size_t index);

  /**
   * @brief Update the start time of an entry
   * @param index Index of entry to update
   * @param newStartTime New start time in seconds
   * @return true if updated successfully, false if would cause overlap or index invalid
   */
  bool updateEntryStartTime(size_t index, double newStartTime);

  /**
   * @brief Update the duration of an entry
   * @param index Index of entry to update
   * @param newDuration New duration in seconds
   * @return true if updated successfully, false if would cause overlap or duration invalid
   */
  bool updateEntryDuration(size_t index, double newDuration);

  /**
   * @brief Clear all entries from the track
   */
  void clearEntries();

  /**
   * @brief Get the entry active at a given time
   * @param time The time to check (in seconds)
   * @return Pointer to active entry, or nullptr if no entry at that time
   */
  const TimelineEntry* getEntryAt(double time) const;

  /**
   * @brief Get all entries on this track
   * @return Vector of timeline entries
   */
  const std::vector<TimelineEntry>& getEntries() const { return entries; }

  /**
   * @brief Get the number of entries
   * @return Entry count
   */
  size_t getEntryCount() const { return entries.size(); }

  /**
   * @brief Get track name
   * @return Track name
   */
  const std::string& getName() const { return name; }

  /**
   * @brief Set track name
   * @param n New name
   */
  void setName(const std::string& n) { name = n; }

  /**
   * @brief Get track color
   * @return Track color
   */
  const Color& getColor() const { return color; }

  /**
   * @brief Set track color
   * @param c New color
   */
  void setColor(const Color& c) { color = c; }

  /**
   * @brief Get track visibility
   * @return true if visible, false if hidden
   */
  bool isVisible() const { return visible; }

  /**
   * @brief Set track visibility
   * @param v Visibility state
   */
  void setVisible(bool v) { visible = v; }

  /**
   * @brief Get the total duration of all entries on this track
   * @return Duration in seconds (end time of last entry)
   */
  double getTotalDuration() const;

private:
  std::string name;
  Color color;
  bool visible;
  std::vector<TimelineEntry> entries;

  /**
   * @brief Check if an entry would overlap with existing entries
   * @param entry The entry to check
   * @return true if would overlap, false otherwise
   */
  bool wouldOverlap(const TimelineEntry& entry) const;
};

} // namespace csci3081

#endif // TRACK_H_
