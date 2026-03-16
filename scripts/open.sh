#!/usr/bin/env bash

source "$(pwd)/scripts/vars.sh"

"$(pwd)/${GODOT_EDITOR}" -e --path "$(pwd)/godot-project/"
