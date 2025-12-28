#pragma once

#include <vector>
#include <string>
#include <utility>

#include <yaml-cpp/yaml.h>

namespace streamx {
    struct ProjectConfig {
        std::string mode;
        std::string name;
        std::string version;
        std::string describe;

        ProjectConfig() = default;

        explicit ProjectConfig(std::string mode, std::string name, std::string version, std::string describe)
            : mode(std::move(mode)), name(std::move(name)), version(std::move(version)), describe(std::move(describe)) {
        }

        [[nodiscard]] std::string to_string() const {
            std::ostringstream oss;
            oss << "ProjectConfig: { "
                    << "mode: " << mode
                    << ", name: " << name
                    << ", version: " << version
                    << ", describe: " << describe
                    << " }";
            return oss.str();
        }
    };

    struct RTSPConfig {
        int port{};
        int width{};
        int height{};
        std::string suffix;

        RTSPConfig() = default;

        explicit RTSPConfig(
            const bool enable, const int port, std::string suffix, const int width = 1280, const int height = 720
        ) : port(port), width(width), height(height), suffix(std::move(suffix)) {
        }

        [[nodiscard]] std::string to_string() const {
            std::ostringstream oss;
            oss << "RTSPConfig: { "
                    << "port: " << port
                    << ", width: " << width
                    << ", height: " << height
                    << ", suffix: " << suffix
                    << " }";
            return oss.str();
        }
    };

    struct StreamConfig {
        std::string id;
        std::string uri;

        StreamConfig() = default;

        StreamConfig(std::string id, std::string uri)
            : id(std::move(id)), uri(std::move(uri)) {
        }

        [[nodiscard]] std::string to_string() const {
            std::ostringstream oss;
            oss << "StreamConfig { "
                    << "id: " << id
                    << ", uri: " << uri
                    << " }";

            return oss.str();
        }
    };

    class GConfig {
    private:
        GConfig() = default;

    public:
        static GConfig &get_instance() {
            static GConfig instance;
            return instance;
        }

        bool load(const std::string &filename);

    public:
        ProjectConfig project_config_{};
        RTSPConfig rtsp_config_{};
        std::vector<StreamConfig> stream_configs_{};

    private:
        bool loaded_ = false;
        std::string config_path_{};

        void parse_project(const YAML::Node &node);

        void parse_rtsp(const YAML::Node &node);

        void parse_streams(const YAML::Node &node);
    };
}
