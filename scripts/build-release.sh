#!/usr/bin/env bash
# Optimized build with no sanitizers — your day-to-day "is it fast?" build.
set -euo pipefail

# Project root = parent of this scripts/ directory, regardless of where you
# invoke the script from.
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT/build/release"

# Configure (idempotent) then build. Extra args passed to this script are
# forwarded to the configure step, e.g. ./build-release.sh -DFOO=bar
cmake -S "$ROOT" -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=Release \
  -DPHYS_SANITIZER=OFF \
  "$@"

cmake --build "$BUILD_DIR" -j "$(nproc)"

echo "release build -> $BUILD_DIR"
