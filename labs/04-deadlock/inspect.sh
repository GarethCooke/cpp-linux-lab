#!/usr/bin/env bash
# Starts the deadlocking program, attaches gdb, prints all stacks and the
# owner of each mutex, then kills the program.
set -euo pipefail
cd "$(dirname "$0")"
./dead &
pid=$!
sleep 1
sudo gdb -p "$pid" -batch -x owners.gdb
kill "$pid"
