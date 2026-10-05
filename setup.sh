#!/usr/bin/env bash
# Installs the lab toolchain and sets the kernel parameters the labs need.
# Usage: ./setup.sh            install packages + set sysctls
#        ./setup.sh --sysctl-only   re-apply sysctls after a reboot
set -euo pipefail

if [[ "${1:-}" != "--sysctl-only" ]]; then
  sudo apt-get update
  sudo apt-get install -y \
    build-essential g++ clang gdb valgrind strace ltrace sysstat elfutils \
    linux-tools-common linux-tools-generic \
    bpftrace bpfcc-tools heaptrack rr hyperfine numactl hwloc git
  # perf must match the running kernel; this can fail on HWE/OEM kernels,
  # in which case linux-tools-generic usually still provides a working perf.
  sudo apt-get install -y "linux-tools-$(uname -r)" || \
    echo "note: linux-tools-$(uname -r) not found; using linux-tools-generic"
fi

# perf without root, and readable kernel symbols
sudo sysctl -w kernel.perf_event_paranoid=1
sudo sysctl -w kernel.kptr_restrict=0
# Core files written to the current directory as core.<exe>.<pid>
# (Ubuntu's default pipes them to apport, which hides them)
sudo sysctl -w kernel.core_pattern=core.%e.%p
# TSan in gcc 13 fails with "unexpected memory mapping" when ASLR uses
# more than 28 random bits, which is the default on recent Ubuntu kernels
sudo sysctl -w vm.mmap_rnd_bits=28

echo
echo "Done. Also run this in each new shell before lab 01:"
echo "  ulimit -c unlimited"
