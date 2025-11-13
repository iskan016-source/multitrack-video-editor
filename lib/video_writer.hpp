#ifndef video_writer_hpp
#define video_writer_hpp

extern "C" {
#include <inttypes.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}

struct VideoWriterState {
  // Public configuration
  int width, height;
  int fps;

  // Private internal state
  AVFormatContext *av_format_ctx;
  AVCodecContext *av_codec_ctx;
  AVStream *video_stream;
  AVFrame *av_frame;
  AVPacket *av_packet;
  SwsContext *sws_scaler_ctx;
  int frame_count;
};

/**
 * @brief Open a video file for writing
 * @param state Video writer state structure
 * @param filename Output filename (should end in .mp4)
 * @param width Video width in pixels
 * @param height Video height in pixels
 * @param fps Frames per second (e.g., 30)
 * @return true if successful, false otherwise
 */
bool video_writer_open(VideoWriterState *state, const char *filename,
                       int width, int height, int fps);

/**
 * @brief Write a single frame to the video
 * @param state Video writer state
 * @param frame_buffer RGBA pixel data (width * height * 4 bytes)
 * @return true if successful, false otherwise
 */
bool video_writer_write_frame(VideoWriterState *state, const uint8_t *frame_buffer);

/**
 * @brief Close the video file and finalize encoding
 * @param state Video writer state
 */
void video_writer_close(VideoWriterState *state);

#endif
