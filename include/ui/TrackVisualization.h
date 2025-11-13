#ifndef TRACK_VISUALIZATION_H_
#define TRACK_VISUALIZATION_H_

#include "timeline/Timeline.h"
#include "graphics/ColorRect.h"
#include "graphics/Glyph.h"
#include "graphics/Text.h"
#include <vector>

namespace csci3081 {

/**
 * @brief Visual representation of the timeline tracks
 *
 * TrackVisualization displays the timeline as horizontal rows showing:
 * - Each track as a separate row
 * - Timeline entries as colored rectangles with duration
 * - Time markers at top
 * - Current playback position indicator
 * - Selection highlighting
 */
class TrackVisualization {
public:
  /**
   * @brief Create a track visualization
   * @param x X position (normalized 0-1)
   * @param y Y position (normalized 0-1)
   * @param w Width (normalized 0-1)
   * @param h Height (normalized 0-1)
   * @param timeline The timeline to visualize
   */
  TrackVisualization(float x, float y, float w, float h, Timeline* timeline);

  ~TrackVisualization();

  /**
   * @brief Draw the track visualization
   * @param selectedTrack Index of selected track (-1 for none)
   * @param selectedEntry Index of selected entry (-1 for none)
   * @param currentTime Current playback time for playhead
   */
  void draw(int selectedTrack, int selectedEntry, double currentTime) const;

  /**
   * @brief Check if a point is inside a track
   * @param x X coordinate (normalized)
   * @param y Y coordinate (normalized)
   * @return Track index, or -1 if not in any track
   */
  int getTrackAtPosition(float x, float y) const;

  /**
   * @brief Check if a point is inside a timeline entry
   * @param x X coordinate (normalized)
   * @param y Y coordinate (normalized)
   * @param outTrack Output: track index
   * @param outEntry Output: entry index within track
   * @return true if clicked on an entry, false otherwise
   */
  bool getEntryAtPosition(float x, float y, int& outTrack, int& outEntry) const;

  /**
   * @brief Check if a point is near the right edge of an entry (resize handle)
   * @param x X coordinate (normalized)
   * @param y Y coordinate (normalized)
   * @param outTrack Output: track index
   * @param outEntry Output: entry index within track
   * @return true if near resize handle, false otherwise
   */
  bool isNearEntryRightEdge(float x, float y, int& outTrack, int& outEntry) const;

  /**
   * @brief Get the time position from X coordinate
   * @param x X coordinate (normalized)
   * @return Time in seconds
   */
  double getTimeAtPosition(float x) const;

private:
  float x, y, w, h;
  Timeline* timeline;

  const float TRACK_HEIGHT = 0.08f;  // Height of each track row
  const float TRACK_SPACING = 0.01f; // Spacing between tracks
  const float TIME_RULER_HEIGHT = 0.03f; // Height of time ruler at top

  /**
   * @brief Get the Y position of a track
   * @param trackIndex Index of track
   * @return Y position (normalized)
   */
  float getTrackY(size_t trackIndex) const;

  /**
   * @brief Get the X position for a given time
   * @param time Time in seconds
   * @return X position (normalized)
   */
  float getXForTime(double time) const;

  /**
   * @brief Draw the time ruler at top
   * @param totalDuration Total timeline duration
   */
  void drawTimeRuler(double totalDuration) const;

  /**
   * @brief Draw a single track
   * @param track The track to draw
   * @param trackIndex Track index
   * @param isSelected Whether this track is selected
   * @param selectedEntryIndex Index of selected entry in this track (-1 for none)
   */
  void drawTrack(const Track* track, size_t trackIndex, bool isSelected, int selectedEntryIndex) const;

  /**
   * @brief Draw a timeline entry rectangle
   * @param entry The entry to draw
   * @param trackY Y position of the track
   * @param color Entry color
   * @param isSelected Whether this entry is selected
   */
  void drawEntry(const TimelineEntry& entry, float trackY, const Color& color, bool isSelected) const;

  /**
   * @brief Draw the playhead indicator
   * @param currentTime Current playback time
   */
  void drawPlayhead(double currentTime) const;
};

} // namespace csci3081

#endif // TRACK_VISUALIZATION_H_
