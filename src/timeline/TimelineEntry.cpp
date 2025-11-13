#include "timeline/TimelineEntry.h"

namespace csci3081 {

TimelineEntry::TimelineEntry(IAsset* asset, double startTime, double duration)
  : asset(asset), startTime(startTime), duration(duration) {
}

} // namespace csci3081
