#include "video_writer.hpp"
#include <iostream>

// Helper function for error messages
static const char *av_make_error(int errnum) {
  static char str[AV_ERROR_MAX_STRING_SIZE];
  memset(str, 0, sizeof(str));
  return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}

bool video_writer_open(VideoWriterState *state, const char *filename,
                       int width, int height, int fps) {
  state->width = width;
  state->height = height;
  state->fps = fps;
  state->frame_count = 0;

  // Allocate output format context
  avformat_alloc_output_context2(&state->av_format_ctx, NULL, NULL, filename);
  if (!state->av_format_ctx) {
    std::cerr << "Could not allocate output format context" << std::endl;
    return false;
  }

  // Find H.264 encoder
  const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
  if (!codec) {
    std::cerr << "H.264 codec not found" << std::endl;
    return false;
  }

  // Create video stream
  state->video_stream = avformat_new_stream(state->av_format_ctx, NULL);
  if (!state->video_stream) {
    std::cerr << "Could not create video stream" << std::endl;
    return false;
  }

  // Allocate codec context
  state->av_codec_ctx = avcodec_alloc_context3(codec);
  if (!state->av_codec_ctx) {
    std::cerr << "Could not allocate codec context" << std::endl;
    return false;
  }

  // Set codec parameters
  state->av_codec_ctx->width = width;
  state->av_codec_ctx->height = height;
  state->av_codec_ctx->time_base = AVRational{1, fps};
  state->av_codec_ctx->framerate = AVRational{fps, 1};
  state->av_codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
  state->av_codec_ctx->bit_rate = 2000000; // 2 Mbps

  // Set GOP size (keyframe interval)
  state->av_codec_ctx->gop_size = 12;
  state->av_codec_ctx->max_b_frames = 2;

  // Some formats require global headers
  if (state->av_format_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
    state->av_codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
  }

  // Set H.264 specific options for better compression
  av_opt_set(state->av_codec_ctx->priv_data, "preset", "medium", 0);
  av_opt_set(state->av_codec_ctx->priv_data, "crf", "23", 0);

  // Open codec
  if (avcodec_open2(state->av_codec_ctx, codec, NULL) < 0) {
    std::cerr << "Could not open codec" << std::endl;
    return false;
  }

  // Copy codec parameters to stream
  if (avcodec_parameters_from_context(state->video_stream->codecpar,
                                      state->av_codec_ctx) < 0) {
    std::cerr << "Could not copy codec parameters" << std::endl;
    return false;
  }

  state->video_stream->time_base = state->av_codec_ctx->time_base;

  // Allocate frame
  state->av_frame = av_frame_alloc();
  if (!state->av_frame) {
    std::cerr << "Could not allocate frame" << std::endl;
    return false;
  }

  state->av_frame->format = state->av_codec_ctx->pix_fmt;
  state->av_frame->width = width;
  state->av_frame->height = height;

  if (av_frame_get_buffer(state->av_frame, 0) < 0) {
    std::cerr << "Could not allocate frame buffer" << std::endl;
    return false;
  }

  // Allocate packet
  state->av_packet = av_packet_alloc();
  if (!state->av_packet) {
    std::cerr << "Could not allocate packet" << std::endl;
    return false;
  }

  // Create scaler context to convert RGBA to YUV420P
  state->sws_scaler_ctx = sws_getContext(
      width, height, AV_PIX_FMT_RGBA,
      width, height, AV_PIX_FMT_YUV420P,
      SWS_BILINEAR, NULL, NULL, NULL);

  if (!state->sws_scaler_ctx) {
    std::cerr << "Could not create scaler context" << std::endl;
    return false;
  }

  // Open output file
  if (!(state->av_format_ctx->oformat->flags & AVFMT_NOFILE)) {
    if (avio_open(&state->av_format_ctx->pb, filename, AVIO_FLAG_WRITE) < 0) {
      std::cerr << "Could not open output file: " << filename << std::endl;
      return false;
    }
  }

  // Write file header
  if (avformat_write_header(state->av_format_ctx, NULL) < 0) {
    std::cerr << "Could not write header" << std::endl;
    return false;
  }

  std::cout << "Video writer opened: " << filename << " (" << width << "x" << height
            << " @ " << fps << " fps)" << std::endl;

  return true;
}

bool video_writer_write_frame(VideoWriterState *state, const uint8_t *frame_buffer) {
  // Make frame writable
  if (av_frame_make_writable(state->av_frame) < 0) {
    std::cerr << "Could not make frame writable" << std::endl;
    return false;
  }

  // Convert RGBA to YUV420P
  const uint8_t *src_data[1] = {frame_buffer};
  int src_linesize[1] = {state->width * 4}; // RGBA has 4 bytes per pixel

  sws_scale(state->sws_scaler_ctx, src_data, src_linesize, 0, state->height,
            state->av_frame->data, state->av_frame->linesize);

  // Set frame PTS (presentation timestamp)
  state->av_frame->pts = state->frame_count;
  state->frame_count++;

  // Send frame to encoder
  int ret = avcodec_send_frame(state->av_codec_ctx, state->av_frame);
  if (ret < 0) {
    std::cerr << "Error sending frame to encoder: " << av_make_error(ret) << std::endl;
    return false;
  }

  // Receive encoded packets
  while (ret >= 0) {
    ret = avcodec_receive_packet(state->av_codec_ctx, state->av_packet);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
      break;
    } else if (ret < 0) {
      std::cerr << "Error receiving packet from encoder: " << av_make_error(ret)
                << std::endl;
      return false;
    }

    // Rescale packet timestamps
    av_packet_rescale_ts(state->av_packet, state->av_codec_ctx->time_base,
                         state->video_stream->time_base);
    state->av_packet->stream_index = state->video_stream->index;

    // Write packet to file
    ret = av_interleaved_write_frame(state->av_format_ctx, state->av_packet);
    if (ret < 0) {
      std::cerr << "Error writing packet: " << av_make_error(ret) << std::endl;
      av_packet_unref(state->av_packet);
      return false;
    }

    av_packet_unref(state->av_packet);
  }

  return true;
}

void video_writer_close(VideoWriterState *state) {
  // Flush encoder
  avcodec_send_frame(state->av_codec_ctx, NULL);

  int ret;
  while (ret >= 0) {
    ret = avcodec_receive_packet(state->av_codec_ctx, state->av_packet);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
      break;
    } else if (ret < 0) {
      break;
    }

    av_packet_rescale_ts(state->av_packet, state->av_codec_ctx->time_base,
                         state->video_stream->time_base);
    state->av_packet->stream_index = state->video_stream->index;
    av_interleaved_write_frame(state->av_format_ctx, state->av_packet);
    av_packet_unref(state->av_packet);
  }

  // Write file trailer
  av_write_trailer(state->av_format_ctx);

  // Clean up
  if (state->sws_scaler_ctx) {
    sws_freeContext(state->sws_scaler_ctx);
  }

  if (state->av_frame) {
    av_frame_free(&state->av_frame);
  }

  if (state->av_packet) {
    av_packet_free(&state->av_packet);
  }

  if (state->av_codec_ctx) {
    avcodec_free_context(&state->av_codec_ctx);
  }

  if (state->av_format_ctx) {
    if (!(state->av_format_ctx->oformat->flags & AVFMT_NOFILE)) {
      avio_closep(&state->av_format_ctx->pb);
    }
    avformat_free_context(state->av_format_ctx);
  }

  std::cout << "Video writer closed (" << state->frame_count << " frames written)" << std::endl;
}
