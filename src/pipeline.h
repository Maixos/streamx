#pragma once

#include <rtspx/rtspx.h>
#include <vcodecx/manager.h>

#include "config.h"

namespace streamx {
    class Pipeline {
    public:
        Pipeline() = default;

        Pipeline(
                StreamConfig stream_config, const std::shared_ptr<vcodecx::Manager> &codec_manager,
                const std::shared_ptr<rtspx::MediaSession> &session
        );

        ~Pipeline();

        void stop();

    private:
        void process() const;

        void on_decoded(const std::shared_ptr<vcodecx::FrameX> &framex) const;

        void on_encoded(const std::shared_ptr<vcodecx::EncodedX> &encodedx) const;

    private:
        std::atomic<bool> stopped_{false};

        StreamConfig stream_config_{};

        std::thread processor_{};
        std::shared_ptr<vcodecx::Decoder> decoder_;
        std::shared_ptr<vcodecx::Encoder> encoder_;
        std::shared_ptr<vcodecx::Manager> codec_manager_;

        std::shared_ptr<rtspx::MediaSession> session_;
    };
}
