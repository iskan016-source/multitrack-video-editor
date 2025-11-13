#include "timeline/Timeline.h"
#include <iostream>
#include <algorithm>

namespace csci3081 {

Timeline::Timeline() : currentTime(0.0) {
}

Timeline::~Timeline() {
  clearTracks();
}

size_t Timeline::addTrack(const std::string& name) {
  size_t index = tracks.size();
  Color trackColor = generateTrackColor(index);

  std::string trackName = name;
  if (trackName == "Track") {
    trackName = "Track " + std::to_string(index);
  }

  Track* track = new Track(trackName, trackColor);
  tracks.push_back(track);

  std::cout << "Added track " << index << ": " << trackName << std::endl;

  return index;
}

bool Timeline::removeTrack(size_t trackIndex) {
  if (trackIndex >= tracks.size()) {
    return false;
  }

  delete tracks[trackIndex];
  tracks.erase(tracks.begin() + trackIndex);
  return true;
}

void Timeline::clearTracks() {
  for (Track* track : tracks) {
    delete track;
  }
  tracks.clear();
}

Track* Timeline::getTrack(size_t trackIndex) {
  if (trackIndex >= tracks.size()) {
    return nullptr;
  }
  return tracks[trackIndex];
}

const Track* Timeline::getTrack(size_t trackIndex) const {
  if (trackIndex >= tracks.size()) {
    return nullptr;
  }
  return tracks[trackIndex];
}

bool Timeline::addEntryToTrack(size_t trackIndex, const TimelineEntry& entry) {
  Track* track = getTrack(trackIndex);
  if (!track) {
    std::cerr << "Track " << trackIndex << " does not exist" << std::endl;
    return false;
  }

  return track->addEntry(entry);
}

double Timeline::getTotalDuration() const {
  double maxDuration = 0.0;

  for (const Track* track : tracks) {
    double trackDuration = track->getTotalDuration();
    if (trackDuration > maxDuration) {
      maxDuration = trackDuration;
    }
  }

  return maxDuration;
}

Image* Timeline::renderFrameAt(double time, int width, int height) const {
  // Create blank image (dark gray background to show transparency better)
  Image* result = new Image(width, height);

  // Fill with dark gray (makes transparent areas visible against black UI)
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      result->setPixel(x, y, Color(32, 32, 32, 255));
    }
  }

  // Composite each track in order (bottom to top)
  for (size_t i = 0; i < tracks.size(); i++) {
    const Track* track = tracks[i];

    if (!track->isVisible()) {
      // Uncomment for debugging:
      // std::cout << "Track " << i << " (" << track->getName() << ") is not visible, skipping" << std::endl;
      continue;
    }

    // Get the active entry at this time
    const TimelineEntry* entry = track->getEntryAt(time);
    if (!entry) {
      // Uncomment for verbose debugging:
      // std::cout << "Track " << i << " (" << track->getName() << ") has no entry at time " << time << "s" << std::endl;
      continue; // No entry active on this track at this time
    }

    // Uncomment for debugging:
    // std::cout << "Rendering track " << i << " (" << track->getName() << ") at time " << time << "s" << std::endl;

    // Get the frame from the entry
    const Image& layerImage = entry->getFrameAt(time);

    //(*result) = layerImage;
    //return result;

    // Composite this layer onto the result
    compositeImages(*result, layerImage);
  }

  return result;
}

void Timeline::compositeImages(Image& bottom, const Image& top) const {
  // Alpha blending composite with scaling
  // If top and bottom are different sizes, scale the top image to fit

  int bottomWidth = bottom.getWidth();
  int bottomHeight = bottom.getHeight();
  int topWidth = top.getWidth();
  int topHeight = top.getHeight();

  // Composite with nearest-neighbor scaling
  for (int y = 0; y < bottomHeight; y++) {
    for (int x = 0; x < bottomWidth; x++) {
      // Map bottom coordinates to top coordinates (scaling)
      int topX = (x * topWidth) / bottomWidth;
      int topY = (y * topHeight) / bottomHeight;

      // Clamp to ensure we don't go out of bounds
      topX = std::min(topX, topWidth - 1);
      topY = std::min(topY, topHeight - 1);

      Color topPixel = top.getPixel(topX, topY);
      Color bottomPixel = bottom.getPixel(x, y);

      // Simple alpha blending: result = top * alpha + bottom * (1 - alpha)
      float alpha = topPixel.alpha() / 255.0f;

      int r = static_cast<int>(topPixel.red()   * alpha + bottomPixel.red()   * (1 - alpha));
      int g = static_cast<int>(topPixel.green() * alpha + bottomPixel.green() * (1 - alpha));
      int b = static_cast<int>(topPixel.blue()  * alpha + bottomPixel.blue()  * (1 - alpha));
      int a = 255; // Result is always opaque

      bottom.setPixel(x, y, Color(r, g, b, a));
    }
  }
}

Color Timeline::generateTrackColor(size_t index) const {
  // Generate different colors for each track
  const Color colors[] = {
    Color(100, 150, 200, 255),  // Blue
    Color(200, 100, 150, 255),  // Pink
    Color(150, 200, 100, 255),  // Green
    Color(200, 150, 100, 255),  // Orange
    Color(150, 100, 200, 255),  // Purple
    Color(100, 200, 200, 255),  // Cyan
  };

  return colors[index % 6];
}

} // namespace csci3081
