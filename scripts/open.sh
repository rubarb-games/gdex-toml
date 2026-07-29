#!/usr/bin/env bash
cd "$(dirname -- "${BASH_SOURCE[0]}")"/..

source "$(pwd)/scripts/vars.sh"

"$(pwd)/${GODOT_EDITOR}" -e --path "$(pwd)/godot-project/"
