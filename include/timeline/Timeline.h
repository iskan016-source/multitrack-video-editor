#ifndef TIMELINE_H_
#define TIMELINE_H_

#include "timeline/Track.h"
#include "Image.h"
#include <vector>
#include <memory>

namespace csci3081 {

/**
 * @brief Manages multiple tracks and composites them into a single output
 *
 * Timeline is the main class for managing the multi-track composition.
 * It contains multiple Track objects, each representing a layer.
 *
 * Tracks are rendered in order (bottom to top):
 * - Track 0 is rendered first (background)
 * - Track 1 is rendered on top of Track 0
 * - Track 2 is rendered on top of Track 1
 * - etc.
 *
 * This allows for layering effects, overlays, and complex compositions.
 */
class Timeline {
public:
  Timeline();
  ~Timeline();

  /**
   * @brief Add a new track to the timeline
   * @param name Track name
   * @return Index of the newly created track
   */
  size_t addTrack(const std::string& name = "Track");

  /**
   * @brief Remove a track by index
   * @param trackIndex Index of track to remove
   * @return true if removed, false if index out of bounds
   */
  bool removeTrack(size_t trackIndex);

  /**
   * @brief Clear all tracks
   */
  void clearTracks();

  /**
   * @brief Get a track by index
   * @param trackIndex Index of track
   * @return Pointer to track, or nullptr if index out of bounds
   */
  Track* getTrack(size_t trackIndex);
  const Track* getTrack(size_t trackIndex) const;

  /**
   * @brief Get all tracks
   * @return Vector of track pointers
   */
  const std::vector<Track*>& getTracks() const { return tracks; }

  /**
   * @brief Get the number of tracks
   * @return Track count
   */
  size_t getTrackCount() const { return tracks.size(); }

  /**
   * @brief Add an entry to a specific track
   * @param trackIndex Index of track to add to
   * @param entry The timeline entry to add
   * @return true if added successfully, false if track doesn't exist or overlap
   */
  bool addEntryToTrack(size_t trackIndex, const TimelineEntry& entry);

  /**
   * @brief Get the total duration of the timeline
   * @return Duration in seconds (longest track duration)
   */
  double getTotalDuration() const;

  /**
   * @brief Render the composite frame at a given time
   * @param time The time to render (in seconds)
   * @param width Output image width
   * @param height Output image height
   * @return Composited image (caller owns the pointer)
   */
  Image* renderFrameAt(double time, int width, int height) const;

  /**
   * @brief Get the current playback time
   * @return Current time in seconds
   */
  double getCurrentTime() const { return currentTime; }

  /**
   * @brief Set the current playback time
   * @param time New time in seconds
   */
  void setCurrentTime(double time) { currentTime = time; }

private:
  std::vector<Track*> tracks;
  double currentTime;

  /**
   * @brief Composite (blend) two images together
   * @param bottom The bottom layer image (modified in place)
   * @param top The top layer image
   */
  void compositeImages(Image& bottom, const Image& top) const;

  /**
   * @brief Generate default track colors
   * @param index Track index
   * @return Color for this track
   */
  Color generateTrackColor(size_t index) const;
};

} // namespace csci3081

#endif // TIMELINE_H_
