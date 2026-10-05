#!/usr/bin/env bash
# Records br with call stacks and renders a flame graph to br.svg.
set -euo pipefail
cd "$(dirname "$0")"
[[ -d FlameGraph ]] || git clone --depth 1 https://github.com/brendangregg/FlameGraph
perf record -F 999 -g ./br > /dev/null
perf script | FlameGraph/stackcollapse-perf.pl | FlameGraph/flamegraph.pl > br.svg
echo "open br.svg in a browser"
