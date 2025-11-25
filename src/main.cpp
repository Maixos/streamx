#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

#include <opencv2/highgui.hpp>

#include "rtspx/rtspx.h"
#include "vcodecx/manager.h"
#include "toolkitx/logx/logx.h"

// -----------------------------------------------------------
// Global Runtime State
// -----------------------------------------------------------
static std::atomic<bool> g_running{true};

// Video pipeline components
static std::shared_ptr<vcodecx::Decoder> g_decoder;
static std::shared_ptr<vcodecx::Encoder> g_encoder;

// RTSP components
static std::shared_ptr<rtspx::RtspServer> g_rtsp_server;
static std::shared_ptr<rtspx::MediaSession> g_rtsp_session;

// Default input source
static std::string g_source_uri = "/dev/video1";

// -----------------------------------------------------------
// Signal Handling
// -----------------------------------------------------------
void handle_sigterm(int) {
    g_running = false;
}

void register_signal_handlers() {
    std::signal(SIGINT, handle_sigterm);
    std::signal(SIGTERM, handle_sigterm);
}

// -----------------------------------------------------------
// Encode Callback (asynchronous push)
// -----------------------------------------------------------
void on_encoded_frame(const std::shared_ptr<vcodecx::EncodedX>& encoded) {
    if (!encoded || encoded->size == 0) return;

    rtspx::EncodedShared packet;
    packet.frame_type = encoded->is_keyframe ? rtspx::VIDEO_FRAME_I : rtspx::VIDEO_FRAME_P;
    packet.size       = encoded->size;
    packet.pts        = encoded->pts;
    packet.data       = encoded->data;
    packet.holder     = encoded->holder;

    g_rtsp_session->push_data(rtspx::MediaTrack::Video, packet);
}

// -----------------------------------------------------------
// Decode Callback (asynchronous push)
// -----------------------------------------------------------
void on_decoded_frame(const std::shared_ptr<vcodecx::FrameX>& frame) {
    if (!frame) return;
    g_encoder->write(frame, 10);
}

// -----------------------------------------------------------
// Callback Mode Pipeline
// -----------------------------------------------------------
void run_callback_mode() {
    auto manager = vcodecx::Manager::create();

    vcodecx::StreamInfo stream_info{"stream1", g_source_uri};
    vcodecx::DecodeConfig decode_cfg{
        1280, 720,
        vcodecx::ImageFormat::NV12,
        vcodecx::WorkerMode::Callback,
        30
    };

    g_decoder = manager->create_decoder(stream_info, decode_cfg);
    if (!g_decoder) {
        LOG_ERROR("demo", "Failed to create decoder");
        return;
    }

    vcodecx::EncodeConfig encode_cfg{
        1280, 720,
        vcodecx::WorkerMode::Callback,
        30, 5,
        vcodecx::CodecType::H265
    };

    g_encoder = manager->create_encoder(encode_cfg);
    if (!g_encoder) {
        LOG_ERROR("demo", "Failed to create encoder");
        return;
    }

    g_decoder->subscribe(on_decoded_frame);
    g_encoder->subscribe(on_encoded_frame);

    // Main loop — idle until SIGINT
    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    g_decoder->release();
    g_encoder->release();
}

// -----------------------------------------------------------
// Read Mode Pipeline
// -----------------------------------------------------------
void run_read_mode() {
    auto manager = vcodecx::Manager::create();

    vcodecx::StreamInfo stream_info{"stream1", g_source_uri};
    vcodecx::DecodeConfig decode_cfg{
        1280, 720,
        vcodecx::ImageFormat::BGR24,
        vcodecx::WorkerMode::QueueRead,
        30
    };

    g_decoder = manager->create_decoder(stream_info, decode_cfg);
    if (!g_decoder) {
        LOG_ERROR("demo", "Failed to create decoder");
        return;
    }

    vcodecx::EncodeConfig encode_cfg{
        1280, 720,
        vcodecx::WorkerMode::QueueRead,
        30, 5
    };

    g_encoder = manager->create_encoder(encode_cfg);
    if (!g_encoder) {
        LOG_ERROR("demo", "Failed to create encoder");
        return;
    }

    // Decoder reading thread
    std::thread decode_worker([] {
        while (g_running && !g_decoder->is_released()) {
            std::shared_ptr<vcodecx::FrameX> frame;
            g_decoder->read(frame, 10);
            if (frame) {
                g_encoder->write(frame, 10);
            }
        }
    });

    // Encoder reading thread
    std::thread encode_worker([] {
        while (g_running && !g_encoder->is_released()) {
            std::shared_ptr<vcodecx::EncodedX> encoded;
            g_encoder->read(encoded, 10);
            if (encoded && encoded->size > 0) {
                on_encoded_frame(encoded);
            }
        }
    });

    // Keep running
    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    decode_worker.join();
    encode_worker.join();

    g_decoder->release();
    g_encoder->release();
}

// -----------------------------------------------------------
// Main Entry
// -----------------------------------------------------------
int main(int argc, char* argv[]) {
    std::cout << "AI-StreamX Demo Starting..." << std::endl;

    logx::Logger::init("vcodec", "debug", false);
    register_signal_handlers();

    if (argc > 1) {
        g_source_uri = argv[1];
    }

    // Start RTSP server
    g_rtsp_server = rtspx::RtspServer::create();
    if (!g_rtsp_server->start("0.0.0.0", 8554)) {
        LOG_ERROR("demo", "RTSP server failed to listen on port 8554");
        return -1;
    }

    g_rtsp_session = g_rtsp_server->add_session("live");
    g_rtsp_session->add_source(rtspx::Video, rtspx::H265);

    run_callback_mode();

    // run_read_mode();

    return 0;
}
