#include "ui/TrackVisualization.h"
#include "graphics/ColorRect.h"
#include "graphics/Glyph.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace csci3081 {

TrackVisualization::TrackVisualization(float x, float y, float w, float h, Timeline* timeline)
  : x(x), y(y), w(w), h(h), timeline(timeline) {
}

TrackVisualization::~TrackVisualization() {
}

void TrackVisualization::draw(int selectedTrack, int selectedEntry, double currentTime) const {
  if (!timeline) return;

  double totalDuration = timeline->getTotalDuration();
  if (totalDuration <= 0) {
    totalDuration = 10.0;  // Show at least 10 seconds if empty
  }

  // Draw time ruler at top
  drawTimeRuler(totalDuration);

  // Draw each track
  for (size_t i = 0; i < timeline->getTrackCount(); i++) {
    const Track* track = timeline->getTrack(i);
    if (track) {
      drawTrack(track, i, i == selectedTrack, selectedEntry);
    }
  }

  // Draw playhead indicator
  drawPlayhead(currentTime);
}

void TrackVisualization::drawTimeRuler(double totalDuration) const {
  // Create colored image for ruler background
  ColorRect rulerBgColor(10, 10, Color(40, 40, 40, 255));
  Glyph rulerBg(x, y, w, TIME_RULER_HEIGHT, rulerBgColor.getImage());
  rulerBg.draw();

  // Draw time markers every second (or scaled based on duration)
  int numMarkers = 10;
  double timeStep = totalDuration / numMarkers;

  ColorRect markerColor(2, 10, Color(150, 150, 150, 255));

  for (int i = 0; i <= numMarkers; i++) {
    double time = i * timeStep;
    float markerX = getXForTime(time);

    // Draw marker line
    Glyph marker(markerX, y, 0.002f, TIME_RULER_HEIGHT, markerColor.getImage());
    marker.draw();

    // TODO: Draw time text labels (would need Text rendering)
  }
}

void TrackVisualization::drawTrack(const Track* track, size_t trackIndex, bool isSelected, int selectedEntryIndex) const {
  float trackY = getTrackY(trackIndex);

  // Draw track background
  Color bgColor = isSelected ? Color(80, 80, 100, 255) : Color(50, 50, 60, 255);
  ColorRect trackBgColor(10, 10, bgColor);
  Glyph trackBg(x, trackY, w, TRACK_HEIGHT, trackBgColor.getImage());
  trackBg.draw();

  // Draw track border
  if (isSelected) {
    ColorRect borderColor(10, 2, Color(0, 255, 255, 255));
    // Top border
    Glyph border1(x, trackY, w, 0.002f, borderColor.getImage());
    border1.draw();
    // Bottom border
    Glyph border2(x, trackY + TRACK_HEIGHT, w, 0.002f, borderColor.getImage());
    border2.draw();
  }

  // Draw each entry in the track
  const std::vector<TimelineEntry>& entries = track->getEntries();
  for (size_t i = 0; i < entries.size(); i++) {
    const TimelineEntry& entry = entries[i];
    // Entry is selected if this track is selected AND this entry index matches
    bool entryIsSelected = isSelected && (static_cast<int>(i) == selectedEntryIndex);
    drawEntry(entry, trackY, track->getColor(), entryIsSelected);
  }
}

void TrackVisualization::drawEntry(const TimelineEntry& entry, float trackY,
                                    const Color& color, bool isSelected) const {
  float entryX = getXForTime(entry.getStartTime());
  float entryW = getXForTime(entry.getDuration()) - getXForTime(0);

  // Draw entry with asset thumbnail
  IAsset* asset = entry.getAsset();
  if (asset) {
    const Image& thumbnail = asset->getThumbnail();
    Glyph entryRect(entryX, trackY + 0.005f, entryW, TRACK_HEIGHT - 0.01f, thumbnail);
    entryRect.draw();
  } else {
    // Fallback to colored rectangle if no asset
    ColorRect entryColor(10, 10, color);
    Glyph entryRect(entryX, trackY + 0.005f, entryW, TRACK_HEIGHT - 0.01f, entryColor.getImage());
    entryRect.draw();
  }

  // Draw selection border
  if (isSelected) {
    ColorRect borderColor(10, 3, Color(255, 255, 0, 255));
    // Top border
    Glyph border1(entryX, trackY + 0.005f, entryW, 0.003f, borderColor.getImage());
    border1.draw();
    // Bottom border
    Glyph border2(entryX, trackY + TRACK_HEIGHT - 0.008f, entryW, 0.003f, borderColor.getImage());
    border2.draw();
    // Left border
    Glyph border3(entryX, trackY + 0.005f, 0.003f, TRACK_HEIGHT - 0.01f, borderColor.getImage());
    border3.draw();
    // Right border
    Glyph border4(entryX + entryW - 0.003f, trackY + 0.005f, 0.003f, TRACK_HEIGHT - 0.01f, borderColor.getImage());
    border4.draw();
  }
}

void TrackVisualization::drawPlayhead(double currentTime) const {
  float playheadX = getXForTime(currentTime);

  // Draw playhead line (red vertical line)
  ColorRect playheadColor(3, 10, Color(255, 50, 50, 255));
  Glyph playhead(playheadX, y, 0.003f, h, playheadColor.getImage());
  playhead.draw();
}

float TrackVisualization::getTrackY(size_t trackIndex) const {
  return y + TIME_RULER_HEIGHT + trackIndex * (TRACK_HEIGHT + TRACK_SPACING);
}

float TrackVisualization::getXForTime(double time) const {
  double totalDuration = timeline->getTotalDuration();
  if (totalDuration <= 0) {
    totalDuration = 10.0;
  }

  float normalizedTime = time / totalDuration;
  return x + normalizedTime * w;
}

double TrackVisualization::getTimeAtPosition(float xPos) const {
  double totalDuration = timeline->getTotalDuration();
  if (totalDuration <= 0) {
    totalDuration = 10.0;
  }

  float normalizedX = (xPos - x) / w;
  return normalizedX * totalDuration;
}

int TrackVisualization::getTrackAtPosition(float xPos, float yPos) const {
  if (xPos < x || xPos > x + w || yPos < y || yPos > y + h) {
    return -1;
  }

  // Skip time ruler
  if (yPos < y + TIME_RULER_HEIGHT) {
    return -1;
  }

  for (size_t i = 0; i < timeline->getTrackCount(); i++) {
    float trackY = getTrackY(i);
    if (yPos >= trackY && yPos <= trackY + TRACK_HEIGHT) {
      return i;
    }
  }

  return -1;
}

bool TrackVisualization::getEntryAtPosition(float xPos, float yPos, int& outTrack, int& outEntry) const {
  int trackIndex = getTrackAtPosition(xPos, yPos);
  if (trackIndex < 0) {
    return false;
  }

  const Track* track = timeline->getTrack(trackIndex);
  if (!track) {
    return false;
  }

  double clickTime = getTimeAtPosition(xPos);

  // Check each entry in the track
  const std::vector<TimelineEntry>& entries = track->getEntries();
  for (size_t i = 0; i < entries.size(); i++) {
    const TimelineEntry& entry = entries[i];
    if (clickTime >= entry.getStartTime() && clickTime < entry.getEndTime()) {
      outTrack = trackIndex;
      outEntry = i;
      return true;
    }
  }

  return false;
}

bool TrackVisualization::isNearEntryRightEdge(float xPos, float yPos, int& outTrack, int& outEntry) const {
  int trackIndex = getTrackAtPosition(xPos, yPos);
  if (trackIndex < 0) {
    return false;
  }

  const Track* track = timeline->getTrack(trackIndex);
  if (!track) {
    return false;
  }

  double clickTime = getTimeAtPosition(xPos);

  // Check each entry in the track
  const std::vector<TimelineEntry>& entries = track->getEntries();
  for (size_t i = 0; i < entries.size(); i++) {
    const TimelineEntry& entry = entries[i];
    double endTime = entry.getEndTime();

    // Check if within entry bounds
    if (clickTime >= entry.getStartTime() && clickTime < endTime) {
      // Check if near right edge (within 5% of entry duration or 0.02 normalized coords)
      double edgeThreshold = std::min(entry.getDuration() * 0.1, 0.5);  // 10% of duration or 0.5s
      double distanceFromEnd = endTime - clickTime;

      if (distanceFromEnd <= edgeThreshold) {
        outTrack = trackIndex;
        outEntry = i;
        return true;
      }
    }
  }

  return false;
}

} // namespace csci3081
