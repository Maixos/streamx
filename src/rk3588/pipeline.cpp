#include "pipeline.h"

#include <opencv2/opencv.hpp>

#include <toolkitx/logx/logx.h>

namespace streamx {
    Pipeline::Pipeline(
            StreamConfig cfg, const std::shared_ptr<vcodecx::Manager> &codec_manager,
            const std::shared_ptr<rtspx::MediaSession> &session
    ) : stream_config_(std::move(cfg)), codec_manager_(codec_manager), session_(session) {
        GConfig &g_config = GConfig::get_instance();

        int fps = 30;
        int width = g_config.rtsp_config_.width;
        int height = g_config.rtsp_config_.height;

        const vcodecx::StreamInfo stream_info{stream_config_.id, stream_config_.uri};

        /// ----------------------------------------
        ///            Polling Mode
        /// ----------------------------------------
        const vcodecx::DecodeConfig decode_cfg{
                width, height, vcodecx::ImageFormat::BGR24, vcodecx::WorkerMode::Polling, fps, 3
        };
        decoder_ = codec_manager->create_decoder(stream_info, decode_cfg);
        if (!decoder_) {
            LOG_ERROR("pipeline", "failed to create decoder");
            return;
        }

        vcodecx::EncodeConfig encode_cfg{
                width, height, vcodecx::WorkerMode::Callback, fps, 3, vcodecx::CodecType::H265
        };
        encoder_ = codec_manager->create_encoder(encode_cfg);
        if (!decoder_) {
            LOG_ERROR("pipeline", "failed to create encoder");
            return;
        }

        encoder_->subscribe([this](const auto &e) { on_encoded(e); });

        processor_ = std::thread(&Pipeline::process, this);

        /// ----------------------------------------
        ///            Callback Mode
        /// ----------------------------------------
        
        // const vcodecx::DecodeConfig decode_cfg{
        //         width, height, vcodecx::ImageFormat::BGR24, vcodecx::WorkerMode::Callback, fps, 3
        // };
        // decoder_ = codec_manager->create_decoder(stream_info, decode_cfg);
        // if (!decoder_) {
        //     LOG_ERROR("pipeline", "failed to create decoder");
        //     return;
        // }

        // vcodecx::EncodeConfig encode_cfg{
        //         width, height, vcodecx::WorkerMode::Callback, fps, 3, vcodecx::CodecType::H265
        // };
        // encoder_ = codec_manager->create_encoder(encode_cfg);
        // if (!decoder_) {
        //     LOG_ERROR("pipeline", "failed to create encoder");
        //     return;
        // }

        // decoder_->subscribe([this](const auto &e) { on_decoded(e); });
        // encoder_->subscribe([this](const auto &e) { on_encoded(e); });
    }

    Pipeline::~Pipeline() {
        stop();
    }

    void Pipeline::stop() {
        if (stopped_.exchange(true)) return;

        if (processor_.joinable()) {
            processor_.join();
        }

        if (decoder_) decoder_->release();
        if (encoder_) encoder_->release();
        LOG_INFO("pipeline", "stream pipeline %s stopped", stream_config_.id.c_str());
    }

    void Pipeline::process() const {
        while (!stopped_ && !decoder_->is_released()) {
            std::shared_ptr<vcodecx::FrameX> framex;
            if (decoder_->read(framex, 10)) {
                encoder_->write(framex, 3);
            }
        }
    }

    void Pipeline::on_decoded(const std::shared_ptr<vcodecx::FrameX> &fremex) const {
        if (!fremex) return;
        encoder_->write(fremex, 3);
    }

    void Pipeline::on_encoded(const std::shared_ptr<vcodecx::EncodedX> &encodedx) const {
        if (encodedx->size == 0) return;

        rtspx::EncodedShared packet{};
        packet.frame_type = encodedx->is_keyframe ? rtspx::VIDEO_FRAME_I : rtspx::VIDEO_FRAME_P;
        packet.size = encodedx->size;
        packet.pts = encodedx->pts;
        packet.data = encodedx->data;
        packet.holder = encodedx->holder;

        session_->push_data(rtspx::MediaTrack::Video, packet);
    }
};
