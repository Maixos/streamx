#!/bin/bash

set -euo pipefail

# 默认值
ARCH=""
JOBS=$(nproc)
SCRIPT_NAME=$(basename "$0")

# 映射架构到工具链文件
declare -A TOOLCHAIN_MAP=(
  ["armhf"]="../cmake/toolchain-linux-armhf.cmake"
  ["aarch64"]="../cmake/toolchain-linux-aarch64.cmake"
)

usage() {
  cat <<EOF
Usage: $SCRIPT_NAME [-a ARCH] [-j JOBS] [clean]

Options:
  -a ARCH    Target architecture for cross-compilation (e.g., armhf, aarch64)
  -j JOBS    Number of parallel build jobs (default: $JOBS)
  clean      Clean build, lib, install, and release directories

Examples:
  $SCRIPT_NAME -a aarch64 -j 8
  $SCRIPT_NAME clean
EOF
  exit 1
}

clean() {
  echo "Cleaning directories..."
  rm -rf build lib/* install/* release/* runs/*
  echo "✅ Clean complete."
}

build_and_install() {
  echo "Starting build for target architecture: ${ARCH:-host}"

  local TOOLCHAIN_FILE=""
  if [[ -n "$ARCH" ]]; then
    TOOLCHAIN_FILE="${TOOLCHAIN_MAP[$ARCH]:-}"
    if [[ -z "$TOOLCHAIN_FILE" ]]; then
      echo "❌ Unsupported architecture: $ARCH"
      usage
    fi
  fi

  # 检查 cmake 和 make 是否存在
  command -v cmake >/dev/null 2>&1 || { echo "❌ cmake not found."; exit 1; }
  command -v make >/dev/null 2>&1 || { echo "❌ make not found."; exit 1; }

  mkdir -p build
  cd build

  echo "Configuring project with CMake..."
  if [[ -n "$TOOLCHAIN_FILE" ]]; then
    cmake -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" ..
  else
    cmake ..
  fi

  echo "Building with $JOBS parallel jobs..."
  make -j"$JOBS"

  echo "Installing..."
  make install

  echo "✅ Build and install completed."
  cd ..
}

# 解析参数
while getopts "a:j:" opt; do
  case "$opt" in
    a) ARCH="$OPTARG" ;;
    j) JOBS="$OPTARG" ;;
    *) usage ;;
  esac
done

shift $((OPTIND - 1))

if [[ $# -eq 1 && "$1" == "clean" ]]; then
  clean
else
  build_and_install
fi
