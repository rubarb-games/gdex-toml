#!/usr/bin/env bash

case $(uname | tr '[:upper:]' '[:lower:]') in
  linux*)
    export OS=Linux
    ;;
  darwin*)
    export OS=macOS
    ;;
  msys*)
    export OS=Windows
    ;;
  *)
    export OS=notset
    ;;
esac

export GODOT_VER=4.6.1-stable
export PRESET_RELEASE="${OS} Release"
export PRESET_DEBUG="${OS} Debug"

case $OS in
  "Linux")
    export FILE_EXE="Godot_v${GODOT_VER}_linux.x86_64"
    ;;
  "macOS")
    ;;
  "Windows")
    export FILE_EXE="Godot_v${GODOT_VER}_win64.exe"
    ;;
  *)
    ;;
esac

export FOLDER_PROJECT="godot-project"
export FOLDER_TEMP=".temp"
export FOLDER_OUTPUT="output"
export FOLDER_GODOT_EDITOR="godot-editor"
export GODOT_EDITOR="${FOLDER_GODOT_EDITOR}/${FILE_EXE}"

export FILE_EXPORT_LOG="godot-export.log"
export FILE_OUTPUT="project"
