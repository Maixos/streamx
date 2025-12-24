#include <thread>
#include <atomic>
#include <csignal>
#include <iostream>

#include <opencv2/highgui.hpp>

#include <rtspx/rtspx.h>
#include <vcodecx/manager.h>
#include <toolkitx/logx/logx.h>

#include "config.h"
#include "pipeline.h"

using namespace streamx;

static std::atomic<bool> g_running{true};

void handle_sigint(int) {
    g_running = false;
}

void register_signal_handlers() {
    std::signal(SIGINT, handle_sigint);
    std::signal(SIGTERM, handle_sigint);
}

int main(int argc, char* argv[]) {
    std::cout << "StreamX Starting..." << std::endl;

    register_signal_handlers();
    
    /// ----------------------------------------
    ///                 LOGX
    /// ----------------------------------------
    logx::Logger::init("vcodec", "debug", false);

    /// ----------------------------------------
    ///                 CONFIG
    /// ----------------------------------------
    GConfig &g_config = GConfig::get_instance();
    if (!g_config.load("config.yaml")) {
        LOG_ERROR("test", "load config.yaml failed");
        return -1;
    }

    /// ----------------------------------------
    ///                 RTSP
    /// ----------------------------------------
    const auto rtsp_server = rtspx::RtspServer::create();
    if (!rtsp_server->start("0.0.0.0", g_config.rtsp_config_.port)) {
        LOG_ERROR("test", "start rtsp server failed");
        return -1;
    }

    /// ----------------------------------------
    ///                 PIPELINES
    /// ----------------------------------------
    std::vector<std::unique_ptr<Pipeline> > stream_pipelines;
    const auto codec_manager = vcodecx::Manager::instance();
    for (const auto &stream_config: g_config.stream_configs_) {
        LOG_INFO("test", "loaded stream config: %s", stream_config.to_string().c_str());
        const std::string suffix = g_config.rtsp_config_.suffix + "/" + stream_config.id;
        auto session = rtsp_server->add_session(suffix);
        session->add_source(rtspx::Video, rtspx::H265);
        stream_pipelines.emplace_back(std::make_unique<Pipeline>(stream_config, codec_manager, session));
    }

    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    stream_pipelines.clear();

    rtsp_server->stop();

    return 0;
}
