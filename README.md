# AI-StreamX
**Hardware-accelerated zero-copy multi-stream video processing framework for AI**  
_面向 AI 的硬件加速零拷贝多路视频流处理框架_

---

# 🧩 Overview | 概述
AI-StreamX is a high-performance video streaming framework designed for AI-centric workloads. It provides a zero-copy processing pipeline covering decoding → inference → encoding → streaming, supports parallel multi-stream management, and offers both synchronous and asynchronous execution modes. With unified scheduling and resource management, AI-StreamX enables efficient multi-task video pipelines with low latency and high throughput.  
_AI-StreamX 是一个面向 AI 场景的高性能视频流处理框架，提供从 解码 → 推理 → 编码 → 推流 的全链路零拷贝数据通道，支持多路视频流并行管理，并兼容同步 / 异步执行模式，同时提供 C++ 与 Python 接口。通过统一的调度与资源管理机制，AI-StreamX 能够构建并运行多任务视频处理流水线，实现低延迟、高吞吐的视频分析能力。_  

---

# ⚙️ Core Features | 核心特性
## Hardware Acceleration | 硬件加速
+ Supports NVIDIA NVDEC/NVENC, Rockchip RKMPP/RGA, and other hardware backends.
+ Fully leverages GPU/VPU acceleration to enable real-time processing with minimal CPU overhead.
+ _支持 NVIDIA NVDEC/NVENC、Rockchip RKMPP/RGA 等主流硬件加速后端。_
+ _充分利用 GPU/VPU 能力，实现低 CPU 占用的实时视频处理。_

## Multi-stream & Multi-task Processing | 多流多任务处理
+ Enables parallel processing of multiple video streams.
+ Each stream can attach multiple downstream AI inference tasks.
+ Built-in task-level scheduling and stream management.
+ _支持多路视频流并行运行。_
+ _单路视频可挂载多个下游 AI 推理任务。_
+ _内置任务级调度与流管理能力。_

## Zero-copy Pipeline | 零拷贝链路
+ End-to-end zero-copy pipeline: _Decode → Inference → Encode → Stream_.
+ Significantly reduces CPU usage and latency while improving throughput and real-time performance.
+ _构建从 解码 → 推理 → 编码 → 推流 的全程零拷贝数据通路。_
+ _大幅降低 CPU 占用与处理延迟，提升吞吐与实时性能。_

## Dual-mode API | 同步 / 异步接口模式
+ Synchronous read mode for pull-based workflows.
+ Asynchronous callback mode for high-performance reactive pipelines.
+ Provides both C++ and Python APIs.
+ _提供同步读取模式，适用于拉取式处理流程。_
+ _支持异步回调模式，适用于高性能流式处理。_
+ _同时提供 C++ 与 Python 两种接口形式。_

## Modular Architecture | 模块化可扩展架构
+ Decoding, inference, encoding, and transport modules are fully decoupled.
+ Easy to customize, extend, and integrate into existing AI systems.
+ _解码、推理、编码与传输模块完全解耦。_
+ _便于自定义扩展、组合部署或集成至现有 AI 系统。_

---

# 💻 Platform Support | 平台支持
| **Platform** | **Status** | **Acceleration** | **Notes** |
| :---: | :---: | :---: | :---: |
| Rockchip RK3588 | ✅ Supported | RKMPP / RGA | Verified on RKNN SDK |
| NVIDIA Jetson | 🚧 In Progress | NVDEC / NVENC | Verified on JetPack |
| x86 + CUDA GPU | 🚧 In Progress | CUDA / FFmpeg | Under development |
| ARM CPU (Software) | ⚙️ Available | Software decode | Lower performance |


---

# 📊 Performance Benchmarks | 性能测试
Below are performance results measured on real hardware platforms to showcase multi-stream throughput, latency, and system resource usage.  
**Key Metrics:**  
+ **Stream × Resolution** – number of video streams and their resolution.  
  _示例：4 × 1080p30 表示 4 路 1080p 30FPS 输入流。_  
+ **Decoders × Encoders** – number of downstream encoders bound to each decoded stream.  
  _示例：1 × 4 表示单路解码、多路编码。_
+ **RTSP Avg Latency** – end-to-end latency (decode → encode → stream → RTSP server).  
+ **CPU%** – average CPU usage under the workload.  
+ **Mode** – Callback (asynchronous) or Sync (pull-based).  

## Rockchip RK3588 Performance Test | RK3588 性能测试
| **Streams** | **Decoders × Encoders** | **RTSP Avg Latency** | **CPU%** | **Pipeline** | **Mode** |
| :---: | :---: | :---: | :---: | :---: | :---: |
| — | — | — | — | — | — |


## NVIDIA Jetson Orin Nano | Orin NANO 性能测试
| **Streams** | **Decoders × Encoders** | **RTSP Avg Latency** | **CPU%** | **GPU%** | **Pipeline** | **Mode** |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **—** | — | — | — | — | — | — |


---

# 🧰 Quick Start | 快速开始
Before using AI-StreamX, ensure that the proper firmware, SDK, and hardware acceleration modules are enabled for your platform.  
_在开始使用 __**AI-StreamX**__ 之前，请根据所选硬件平台安装对应的系统与开发套件，并确保相关硬件加速模块正常启用。_

## Supported Hardware & Required System | 硬件平台与系统要求
| **Hardware** | **System / SDK** | **Notes** |
| :---: | :---: | :---: |
| RK3588-PC | [ROC-RK3588-PC_Ubuntu20.04-Gnome-r30028_v1.1.1b_230914.7z](https://drive.google.com/drive/folders/1Pws2RrNfiCv67wKgDphBGqZ8lx-iidyR) | 需启用 **RKMPP / RGA** 硬件加速 |
| NVIDIA Jetson | JetPack **6.1.1** | 必须启用 **NVENC / NVDEC** |
| x86 + CUDA GPU | CUDA Toolkit / FFmpeg | 开发中 |


## Installation Notes | 安装说明
AI-StreamX dependencies are precompiled in **mirox**, and can be directly installed when using supported firmware. Other firmware versions may not be verified and could require manual compilation.  
_AI-StreamX 的大部分依赖已在 __**mirox**__ 中预编译完成。**使用上述指定固件版本时，可直接安装 __**mirox**__，无需手动编译（其他固件版本未验证）。**_

**Includes:**  
+ **FFmpeg** — video parsing + codec framework
+ **OpenCV (x86 / Jetson)** — image processing
+ **RKMPP / RGA (RK3588)** — hardware acceleration

### Install Basic Dependencies (Ubuntu) | Ubuntu 基础依赖
```shell
sudo apt update

sudo apt install build-essential 

sudo apt install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
```

### Get Pre-compiled Dependencies | 获取 **mirox** 预编译依赖
```shell
git clone https://github.com/Maixos/mirox.git

cd mirox && sudo ./setup.sh
```

## Example Run ｜ RK3588 示例
```shell

cd /path/to/ai-streamx

bash build.sh

cd release && source setup.sh

./ai_streamx /dev/video1
```

---

# 🧭 Roadmap | 发展规划
- [ ] CUDA/x86 平台支持
- [ ] Python SDK (PyPI 发布)

---

# 📜 License | 许可协议
This project is licensed under the **Apache License 2.0** – see the [LICENSE](https://github.com/MincoX/ai-streamx?tab=Apache-2.0-1-ov-file#) file for details.  
Copyright © 2025  
**MincoX**  
Part of the **AI Open Series** — A suite of open AI application frameworks.

---

# 💬 Contact | 联系方式
**GitHub**: https://github.com/mincox  
**Email**: mincox168@outlook.com
