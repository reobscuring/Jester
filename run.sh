#!/usr/bin/env bash
set -euo pipefail
root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
# Plugins are generated outside builtins/ so that directory remains source-only.
make -s -C "$root" plugins
exec env JESTER_BUILTINS="$root/build/plugins" "$root/jester" "$@"
