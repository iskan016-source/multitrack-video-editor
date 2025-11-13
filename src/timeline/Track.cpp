#include "timeline/Track.h"
#include <iostream>

namespace csci3081 {

Track::Track(const std::string& name, const Color& color)
  : name(name), color(color), visible(true) {
}

Track::~Track() {
  // Entries are copied, so no cleanup needed
  // Assets are not owned by Track
}

bool Track::addEntry(const TimelineEntry& entry) {
  // Check for overlap
  if (wouldOverlap(entry)) {
    std::cerr << "Cannot add entry to track '" << name
              << "': overlaps with existing entry" << std::endl;
    return false;
  }

  // Add entry and sort by start time
  entries.push_back(entry);
  std::sort(entries.begin(), entries.end(),
            [](const TimelineEntry& a, const TimelineEntry& b) {
              return a.getStartTime() < b.getStartTime();
            });

  return true;
}

bool Track::removeEntry(size_t index) {
  if (index >= entries.size()) {
    return false;
  }

  entries.erase(entries.begin() + index);
  return true;
}

bool Track::updateEntryStartTime(size_t index, double newStartTime) {
  if (index >= entries.size()) {
    return false;
  }

  // Don't allow negative start times
  if (newStartTime < 0.0) {
    return false;
  }

  // Create a temporary entry with the new start time to test for overlaps
  TimelineEntry& entry = entries[index];
  double oldStartTime = entry.getStartTime();

  // Temporarily update the start time
  entry.setStartTime(newStartTime);

  // Check if it would overlap with other entries (excluding itself)
  for (size_t i = 0; i < entries.size(); i++) {
    if (i != index && entry.overlapsWith(entries[i])) {
      // Restore old start time and return false
      entry.setStartTime(oldStartTime);
      return false;
    }
  }

  // No overlap, keep the new start time and re-sort
  std::sort(entries.begin(), entries.end(),
            [](const TimelineEntry& a, const TimelineEntry& b) {
              return a.getStartTime() < b.getStartTime();
            });

  return true;
}

bool Track::updateEntryDuration(size_t index, double newDuration) {
  if (index >= entries.size()) {
    return false;
  }

  // Don't allow negative or zero durations
  if (newDuration <= 0.1) {  // Minimum 0.1 seconds
    return false;
  }

  TimelineEntry& entry = entries[index];
  double oldDuration = entry.getDuration();

  // Temporarily update the duration
  entry.setDuration(newDuration);

  // Check if it would overlap with other entries (excluding itself)
  for (size_t i = 0; i < entries.size(); i++) {
    if (i != index && entry.overlapsWith(entries[i])) {
      // Restore old duration and return false
      entry.setDuration(oldDuration);
      return false;
    }
  }

  // No overlap, keep the new duration
  return true;
}

void Track::clearEntries() {
  entries.clear();
}

const TimelineEntry* Track::getEntryAt(double time) const {
  // Find the entry active at the given time
  for (const TimelineEntry& entry : entries) {
    if (entry.isActiveAt(time)) {
      return &entry;
    }
  }
  return nullptr;
}

double Track::getTotalDuration() const {
  if (entries.empty()) {
    return 0.0;
  }

  // Entries are sorted by start time
  // Find the entry with the latest end time
  double maxEndTime = 0.0;
  for (const TimelineEntry& entry : entries) {
    double endTime = entry.getEndTime();
    if (endTime > maxEndTime) {
      maxEndTime = endTime;
    }
  }

  return maxEndTime;
}

bool Track::wouldOverlap(const TimelineEntry& entry) const {
  for (const TimelineEntry& existing : entries) {
    if (entry.overlapsWith(existing)) {
      return true;
    }
  }
  return false;
}

} // namespace csci3081
