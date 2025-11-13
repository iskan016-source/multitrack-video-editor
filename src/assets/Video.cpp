#include "Video.h"
#include <iostream>

namespace csci3081 {

Video::Video(const std::string &filename) {
  memset(&videoState, 0, sizeof(videoState));

  if (video_reader_open(&videoState, filename.c_str())) {
    // video_loaded = true;
    frame = new Image(videoState.width, videoState.height);
    frameRate = av_q2d(videoState.time_base);
    frameRate = 1.0 / frameRate; // Convert to fps
    if (frameRate > 60 || frameRate < 1) {
      frameRate = 30.0; // Fallback to reasonable frame rate
    }
    std::cout << "Video loaded: " << videoState.width << "x"
              << videoState.height << " @ " << frameRate << " fps"
              << " (" << videoState.duration << " seconds)" << std::endl;
    // Load the first frame immediately
    uint8_t* frame_buffer = static_cast<uint8_t*>(frame->getData());
    int64_t pts;
    video_reader_read_frame(&videoState, frame_buffer, &pts);
    currentPts = pts;
  } else {
    std::cout << "Failed to load video" << std::endl;
  }
}

Video::~Video() {
  delete frame;
  video_reader_close(&videoState);
}

bool Video::nextFrame(double time) {
  // If time jumped backwards significantly (more than 2 frames), we need to seek/restart
  double timeJump = time - lastFrameTime;
  if (timeJump < -(2.0 / frameRate)) {
    // Significant backwards jump - need to restart video
    std::cout << "[Video] Backward jump detected: " << lastFrameTime << "s -> " << time << "s (seeking)" << std::endl;
    seekFrame(time);
    // Note: seekFrame() sets lastFrameTime, so we don't set it again here
    return true;
  }

  // If time jumped forward significantly (more than 1 second), seek to catch up
  if (timeJump > 1.0) {
    std::cout << "[Video] Forward jump detected: " << lastFrameTime << "s -> " << time << "s (seeking)" << std::endl;
    seekFrame(time);
    // Note: seekFrame() sets lastFrameTime, so we don't set it again here
    return true;
  }

  double time_since_last_frame = time - lastFrameTime;

  if (time_since_last_frame >= (1.0 / frameRate)) {
    uint8_t *frame_buffer = static_cast<uint8_t *>(frame->getData());
    int64_t pts;
    if (video_reader_read_frame(&videoState, frame_buffer, &pts)) {
      currentPts = pts;
    } else {
      std::cerr << "[Video] FAILED to read frame at time " << time << "s" << std::endl;
      return false;
    }

    lastFrameTime = time;
  }

  return true;
}

void Video::seekFrame(double time) {
  // Seek to specific time in video
  int64_t target_pts = (int64_t)(time / av_q2d(videoState.time_base));

  // Use ffmpeg seek to jump to the target time
  if (video_reader_seek_frame(&videoState, target_pts)) {
    // Read the frame at the target position
    uint8_t *frame_buffer = static_cast<uint8_t *>(frame->getData());
    int64_t pts;
    if (video_reader_read_frame(&videoState, frame_buffer, &pts)) {
      currentPts = pts;
      lastFrameTime = time;
    }
  } else {
    // If seek fails, try to seek to beginning as fallback
    std::cout << "Seek to " << time << "s failed, trying beginning" << std::endl;
    if (video_reader_seek_frame(&videoState, 0)) {
      uint8_t *frame_buffer = static_cast<uint8_t *>(frame->getData());
      int64_t pts;
      if (video_reader_read_frame(&videoState, frame_buffer, &pts)) {
        currentPts = pts;
        // We seeked to beginning (0), not to requested time, so set lastFrameTime accordingly
        lastFrameTime = 0.0;
      }
    } else {
      std::cerr << "WARNING: Video seeking completely failed, playback may be broken" << std::endl;
    }
  }
}

} // namespace csci3081