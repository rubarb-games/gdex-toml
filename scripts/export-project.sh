#!/bin/bash

GODOT_VER=4.5-stable

export_preset_rel="Windows Release"
export_preset_dbg="Windows Debug"

folder_project="godot-project"
folder_temp=".temp"
folder_output="output"
folder_godot_editor="godot-editor"

file_export_log="godot-export.log"
file_output="project"

die() {
  printf '%s\n' "$1" >&2
  exit 1
}

make_dir() {
  mkdir -p $1
  echo "Made dir $1"
}

rm_dir() {
  rm -rf $1
  echo "Removed dir $1"
}

unpack() {
  zip_file=$1
  target_folder=$2
  unzip "${zip_file}" -d "${target_folder}"
}

download_editor() {
  echo "Download Godot editor ${GODOT_VER}"
  exe_file="Godot_v${GODOT_VER}_win64.exe"
  zip_file="${exe_file}.zip"
  url="https://github.com/godotengine/godot-builds/releases/download/${GODOT_VER}/${zip_file}"
  downloaded_file="$(pwd)/${folder_temp}/${zip_file}"

  make_dir ${folder_godot_editor}

  # Check if Godot editor has been installed, if not install it
  if [ -f "$(pwd)/${folder_godot_editor}/${exe_file}" ]; then
    echo "No need to download ${exe_file}, it is already installed"
  else

    # delete the downloaded file if it exists
    if [ -f "${downloaded_file}" ]; then
      rm "${downloaded_file}"
    fi

    # download and extract the godot editor to the godot-editor folder
    if curl -L# -o "${downloaded_file}" "${url}"; then
      echo "Downloaded ${url}"
      unpack "${downloaded_file}" "$(pwd)/${folder_godot_editor}"
      echo "Extracted ${zip_file}"
    else
      die "Cannot download ${url}"
    fi
  fi
}

# usage:
# export                     -> exports project in debug mode
# export "project" "release" -> exports project in release mode
# export "project" "debug"   -> exports project in debug mode
# export "pack" "release"    -> exports resource pack with release settings
# export "pack" "debug"      -> exports resource pack with debug settings
export_project() {
  echo "Export Godot project"
  
  build_type=$1
  variant=$2

  if [[ -z $variant ]] ; then
    variant="debug"
  fi
  if [[ -z $build_type ]] ; then
    build_type="project"
  fi

  godot_editor="$(pwd)/${folder_godot_editor}/Godot_v${GODOT_VER}_win64.exe"
  log_export="$(pwd)/${folder_temp}/${file_export_log}"

  # Export Godot project
  rm -f "${log_export}"

  if [ "$build_type" = "project" ]; then

    # empty output dir
    rm_dir "$(pwd)/${folder_output}"
    make_dir "$(pwd)/${folder_output}"

    if [ "$variant" = "debug" ]; then
      echo "Export debug build"
      ${godot_editor} \
        --headless \
        --quit \
        --path "$(pwd)/${folder_project}/" \
        --export-debug "${export_preset_dbg}" \
        "$(pwd)/$folder_output/${file_output}.exe" \
        &> "${log_export}"
    else
      echo "Export release build"
      ${godot_editor} \
        --headless \
        --quit \
        --verbose \
        --path "$(pwd)/${folder_project}/" \
        --export-release "${export_preset_rel}" \
        "$(pwd)/$folder_output/${file_output}.exe" \
        &> "${log_export}"
    fi

  elif [ "$build_type" = "pack" ]; then
    if [ "$variant" = "debug" ]; then
      echo "Export resource packs with debug settings"
      ${godot_editor} \
        --headless \
        --quit \
        --path "$(pwd)/${folder_project}/" \
        --export-pack "${export_preset_dbg}" \
        "$(pwd)/$folder_output/${file_output}.pck" \
        &> "${log_export}"
    else
      echo "Export resource packs with release settings"
      ${godot_editor} \
        --headless \
        --quit \
        --verbose \
        --path "$(pwd)/${folder_project}/" \
        --export-pack "${export_preset_rel}" \
        "$(pwd)/$folder_output/${file_output}.pck" \
        &> "${log_export}"
    fi
  fi

  echo "$(<"${log_export}")"

  # Look for errors in export log
  if grep -Fq "ERROR" "${log_export}"; then
    die "Errors found in export. Abort build"
  fi

  # Look for warnings in export log
  if grep -Fq "WARNING" "${log_export}"; then
    # die "Warnings found in export. Abort build"
    echo "Warnings found in export. Continue"
  fi

  echo "No errors or warnings found in export. Continue"
}

make_dir ${folder_temp}

download_editor
export_project project debug
