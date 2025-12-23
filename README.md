# StreamX
Hardware-accelerated zero-copy multi-stream video processing framework for AI  
_面向 AI 的硬件加速零拷贝多路视频流处理框架_   

# 🧩 Overview | 概述
StreamX is a high-performance video streaming framework designed for AI-centric workloads. It provides a zero-copy processing pipeline covering decoding → inference → encoding → streaming, supports parallel multi-stream management, and offers both synchronous and asynchronous execution modes. With unified scheduling and resource management, StreamX enables efficient multi-task video pipelines with low latency and high throughput.  
_StreamX 是一个面向 AI 场景的高性能视频流处理框架，提供从 解码 → 推理 → 编码 → 推流 的全链路零拷贝数据通道，支持多路视频流并行管理，并兼容同步 / 异步执行模式，同时提供 C++ 与 Python 接口。通过统一的调度与资源管理机制，StreamX 能够构建并运行多任务视频处理流水线，实现低延迟、高吞吐的视频分析能力。_  

# 🎬 Demo | 演示

## 相关链接
+ Bilibili：https://www.bilibili.com/video/BV1HvUYBEEtz/?vd_source=cf873886c731eb05ae070722ce19f5dc  

![Demo](asset/images/demo.gif)  

# ⚙️ Core Features | 核心特性
## Hardware Acceleration | 硬件加速
+ Supports NVIDIA NVDEC/NVENC, Rockchip RKMPP/RGA, and other hardware backends（_支持多种主流硬件加速后端_）  
+ Fully leverages GPU/VPU acceleration to enable real-time processing with minimal CPU overhead（_充分利用硬件能力，实现高效处理_）  

## Multi-stream & Multi-task Processing | 多流多任务处理
+ Enables parallel processing of multiple video streams（_支持多路视频流并行运行_）  
+ Each stream can attach multiple downstream AI inference tasks（_单路视频可挂载多个下游 AI 推理任务_）  
+ Built-in task-level scheduling and stream management（_内置任务级调度与流管理能力_）  

## Zero-copy Pipeline | 零拷贝链路
+ End-to-end zero-copy pipeline: _Decode → Inference → Encode → Stream（_构建从解码 → 推理 → 编码 → 推流的全程零拷贝数据通路_）  
+ Reduces CPU load and latency while boosting throughput（_降低 CPU 占用与延迟，提升吞吐_）  

## Dual-mode API | 同步 / 异步接口模式
+ Synchronous read mode for pull-based workflows（_提供同步读取模式，适用于拉取式处理流程_）  
+ Asynchronous callback mode for high-performance reactive pipelines（_支持异步回调模式，适用于高性能流式处理_）  
+ Provides both C++ and Python APIs（_同时提供 C++ 与 Python 两种接口形式_）  

## Modular Architecture | 模块化可扩展架构
+ Decoding, inference, encoding, and transport modules are fully decoupled（_解码、推理、编码与传输模块完全解耦_）  
+ Easy to customize, extend, and integrate into existing AI systems（_便于自定义扩展、组合部署或集成至现有 AI 系统_）  

# 💻 Platform Support | 平台支持
| **Platform** | **Status** | **Acceleration** | **Notes** |
| :---: | :---: | :---: | :---: |
| Rockchip RK3588 | ✅ Supported | RKMPP / RGA | Verified on RKNN SDK |
| NVIDIA Jetson | 🚧 In Progress | NVDEC / NVENC | Verified on JetPack |
| x86 + CUDA GPU | 🚧 In Progress | CUDA / FFmpeg | Under development |
| ARM CPU (Software) | ⚙️ Available | Software decode | Lower performance |


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
|:-----------:|:-----------------------:|:--------------------:|:--------:| :---: | :---: |
| 1 × 1080p30 |          1 × 1          |       < 380 ms       |  < 10%   | Zero-copy | Callback |
|      〃      |          1 × 2          |       < 380 ms       |  < 10%   | Zero-copy | Callback |
|      〃      |          1 × 4          |        < 380 ms        |  < 15%   | Zero-copy | Callback |
| 2 × 1080p30 |          2 × 2          |        < 380 ms        |  < 10%   | Zero-copy | Callback |
|      〃      |          2 × 4          |        < 380 ms        |  < 15%   | Zero-copy | Callback |
| 4 × 1080p60 |          4 × 4          |        < 400 ms        |  < 30%   | Zero-copy | Callback |
|      〃      |          4 × 8          |       < 400 ms       |  < 50%   | Zero-copy | Callback |
| 6 × 1080p60 |          6 × 6          |      < 400 ms       |  < 80%   | Zero-copy | Callback |


## NVIDIA Jetson Orin Nano | Orin NANO 性能测试
| **Streams** | **Decoders × Encoders** | **RTSP Avg Latency** | **CPU%** | **GPU%** | **Pipeline** | **Mode** |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| — | — | — | — | — | — | — |


# 🧰 Quick Start ｜快速开始

Before using **StreamX**, please ensure that your target platform meets the **verified system requirements** below.

> ℹ️ **Ubuntu Version Notice**  
> StreamX has been **fully verified on Ubuntu 20.04 (focal)**.  
> Support for other Ubuntu releases is **planned and currently under testing**.

_在使用 **StreamX** 之前，请确认目标平台满足下述 **已验证系统要求**。  
目前 StreamX 已在 **Ubuntu 20.04（focal）** 上完成完整验证，**其他版本正在测试与适配中**。_

## Environment Requirements ｜ 平台与环境要求
| **Platform** | **System / SDK** | **Additional Requirements** | **Status** |
| :---: | :---: | :---: | :---: |
| RK3588-PC | [ROC-RK3588-PC_Ubuntu20.04-Gnome-r30028_v1.1.1b_230914.7z](https://drive.google.com/drive/folders/1Pws2RrNfiCv67wKgDphBGqZ8lx-iidyR) | OpenCV ≥ 4.2.0 | ✅ Verified |
| NVIDIA Jetson | JetPack **6.1.1** | OpenCV ≥ 4.2.0 | ✅ Verified |
| x86 + CUDA GPU | CUDA Toolkit / FFmpeg | OpenCV ≥ 4.2.0 | 🚧 In Development |

## Installation & Setup | 安装与配置

### Basic Dependencies (Ubuntu) | Ubuntu 基础依赖
```shell
sudo apt update

sudo apt install build-essential cmake git libgtk2.0-dev pkg-config
```

### Get Pre-compiled Dependencies ｜获取 **mirox** 预编译依赖

> ⚠️ **Pre-compiled Package Notice**  
> The current pre-compiled packages are built on **Ubuntu 20.04 (focal)**.  
> Compatibility with other Ubuntu releases is **under evaluation**.

```shell
sudo add-apt-repository ppa:maixos/mirox -y

sudo apt update

sudo apt install libvcodecx-dev librtspx-dev
```

### Build & Run ｜ 编译与运行
```shell
cd /path/to/streamx

# Build for target platform
bash build.sh <platform>
```

### Example: RK3588 ｜ RK3588 平台示例
```shell
bash build.sh rk3588

./bin/test_streamx_rk3588 /dev/video1
```

# 🧭 Roadmap | 发展规划
- [ ] CUDA/x86 平台支持
- [ ] Python SDK (PyPI 发布)

# 📜 License | 许可协议
This project is licensed under the **Apache License 2.0** – see the [LICENSE](https://github.com/MincoX/ai-streamx?tab=Apache-2.0-1-ov-file#) file for details.  
Copyright © 2025  
**MincoX**  
Part of the **AI Open Series** — A suite of open AI application frameworks.  

# 💬 Contact | 联系方式
**Bilibili**: https://space.bilibili.com/382756182?spm_id_from=333.1007.0.0  
**Email**: maixos@outlook.com  
