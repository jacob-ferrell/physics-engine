#!/usr/bin/env bash
# AddressSanitizer + UndefinedBehaviorSanitizer build. Catches out-of-bounds
# access, use-after-free, leaks, and UB. Run the resulting binaries directly;
# ASan reports fire at runtime. Uses RelWithDebInfo so it's reasonably fast
# but keeps line numbers in the reports.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT/build/asan"

cmake -S "$ROOT" -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DPHYS_SANITIZER=address \
  "$@"

cmake --build "$BUILD_DIR" -j "$(nproc)"

echo "asan build -> $BUILD_DIR"
