#!/usr/bin/env bash
# ThreadSanitizer build. Detects data races and lock-ordering problems — the
# bugs you'll hit in Phase 7 when you parallelize the solver. Run the binaries
# directly; TSan reports races at runtime. Note: TSan is incompatible with
# ASan, which is why it lives in its own build dir.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT/build/tsan"

cmake -S "$ROOT" -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DPHYS_SANITIZER=thread \
  "$@"

cmake --build "$BUILD_DIR" -j "$(nproc)"

echo "tsan build -> $BUILD_DIR"
