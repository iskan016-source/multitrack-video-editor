#ifndef VIDEO_H_
#define VIDEO_H_

#include "Image.h"
#include "video_reader.hpp"
#include <chrono>

namespace csci3081 {

class Video {
public:
  Video(const std::string &filename);
  virtual ~Video();

  bool nextFrame(double time);
  void seekFrame(double time);
  const Image &getFrame() const { return *frame; }
  double getNumFrames() const { return frameRate * getDuration(); }
  double getFramesPerSecond() const { return frameRate; }
  double getDuration() const { return videoState.duration; }

private:
  VideoReaderState videoState;
  double lastFrameTime = 0.0;
  int64_t currentPts = 0;
  double frameRate = 30.0;
  Image *frame;
};

} // namespace csci3081

#endif