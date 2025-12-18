#!/bin/bash
set -euo pipefail

SCRIPT_NAME=$(basename "$0")
JOBS=$(nproc)

# 默认平台
TARGET_PLATFORM="rk3588"

usage() {
  echo "Usage:"
  echo "  $SCRIPT_NAME [platform]"
  echo "  $SCRIPT_NAME clean"
  echo
  echo "Platforms:"
  echo "  rk3588 (default)"
  echo "  jetson"
  exit 1
}

clean() {
  echo "Cleaning directories..."
  rm -rf build lib/* install/* release/* runs/*
  echo "✅ Clean complete."
}

# ----------------------------
# 参数解析
# ----------------------------
if [[ $# -eq 1 && "$1" == "clean" ]]; then
  clean
  exit 0
fi

if [[ $# -gt 1 ]]; then
  usage
fi

if [[ $# -eq 1 ]]; then
  TARGET_PLATFORM="$1"
fi

case "$TARGET_PLATFORM" in
  rk3588|jetson)
    ;;
  *)
    echo "❌ Unsupported platform: $TARGET_PLATFORM"
    usage
    ;;
esac

# ----------------------------
# 构建
# ----------------------------
command -v cmake >/dev/null || { echo "❌ cmake not found"; exit 1; }
command -v make  >/dev/null || { echo "❌ make not found"; exit 1; }

mkdir -p build
cd build

echo "Configuring project with CMake..."
cmake -DTARGET_PLATFORM="$TARGET_PLATFORM" ..

echo "Building..."
make -j"$JOBS"

echo "Installing..."
make install

echo "✅ Build and install completed."
