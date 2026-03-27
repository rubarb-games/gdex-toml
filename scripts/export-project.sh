#!/usr/bin/env bash

source "$(pwd)/scripts/vars.sh"

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
  echo "Download Godot editor ${GODOT_VER} for ${OS}"
  zip_file="${FILE_EXE}.zip"
  url="https://github.com/godotengine/godot-builds/releases/download/${GODOT_VER}/${zip_file}"
  downloaded_file="$(pwd)/${FOLDER_TEMP}/${zip_file}"

  make_dir "${FOLDER_GODOT_EDITOR}"

  # Check if Godot editor has been installed, if not install it
  if [ -f "$(pwd)/${FOLDER_GODOT_EDITOR}/${FILE_EXE}" ]; then
    echo "No need to download ${FILE_EXE}, it is already installed"
  else

    # delete the downloaded file if it exists
    if [ -f "${downloaded_file}" ]; then
      rm "${downloaded_file}"
    fi

    # download and extract the godot editor to the godot-editor folder
    if curl -L# -o "${downloaded_file}" "${url}"; then
      echo "Downloaded ${url}"
      unpack "${downloaded_file}" "$(pwd)/${FOLDER_GODOT_EDITOR}"
      echo "Extracted ${zip_file}"
    else
      die "Cannot download ${url}"
    fi
  fi
}

download_export_templates() {
  ver="${GODOT_VER/-/.}" # replace dash with dot

  if [ ${OS} == "Linux" ]; then
    destination="${HOME}/.local/share/godot/export_templates/${ver}"
  elif [ ${OS} == "Windows" ]; then
    destination="${HOME}/AppData/Roaming/Godot/export_templates/${ver}"
  fi

  zip_file="Godot_v${GODOT_VER}_export_templates.tpz"
  url="https://github.com/godotengine/godot-builds/releases/download/${GODOT_VER}/${zip_file}"
  downloaded_file="$(pwd)/${FOLDER_TEMP}/${zip_file}"

  echo "Download Godot export templates"
  echo "url: ${url}"
  echo "downloaded_file: ${downloaded_file}"
  echo "zip_file: ${zip_file}"
  echo "destination: ${destination}"

  # Check if export templates has been installed, if not install it
  if [ -d "${destination}/" ]; then
    echo "No need to download export templates for Godot ${GODOT_VER} (${ver}), they have already been installed"
  else

    echo "Download export template for ${GODOT_VER} (${ver}) to ${destination}"

    # delete the temporary tpz file if it exists
    if [ -f "${downloaded_file}" ]; then
      rm "${downloaded_file}"
    fi

    # download and extract the templates
    if curl -L# -o "${downloaded_file}" "${url}"; then
      echo "Downloaded ${url}"
      make_dir "${destination}"
      unpack "${downloaded_file}" "${destination}"
      echo "Extracted ${zip_file}"
      mv -f "${destination}/templates/"* "${destination}"
      rm_dir "${destination}/templates/"
    else
      die "Cannot download ${url}"
    fi
  fi
}

# usage:
# export_project           -> exports project in debug mode
# export_project "release" -> exports project in release mode
# export_project "debug"   -> exports project in debug mode
export_project() {
  mode=$1

  echo "Export Godot project in ${mode} mode"

  if [[ -z $mode ]] ; then
    mode="debug"
  fi

  log_export="$(pwd)/${FOLDER_TEMP}/${FILE_EXPORT_LOG}"

  # Export Godot project
  rm -f "${log_export}"

  # empty output dir
  rm_dir "$(pwd)/${FOLDER_OUTPUT}"
  make_dir "$(pwd)/${FOLDER_OUTPUT}"

  if [ "$mode" = "debug" ]; then
    echo "Export debug build"
    ${GODOT_EDITOR} \
      --headless \
      --quit \
      --path "$(pwd)/${FOLDER_PROJECT}/" \
      --export-debug "${PRESET_DEBUG}" \
      "$(pwd)/$FOLDER_OUTPUT/${FILE_OUTPUT}.exe" \
      &> "${log_export}"
  else
    echo "Export release build"
    ${GODOT_EDITOR} \
      --headless \
      --quit \
      --verbose \
      --path "$(pwd)/${FOLDER_PROJECT}/" \
      --export-release "${PRESET_RELEASE}" \
      "$(pwd)/$FOLDER_OUTPUT/${FILE_OUTPUT}.exe" \
      &> "${log_export}"
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

# usage:
# export_pack           -> exports content pack in debug mode
# export_pack "release" -> exports content pack in release mode
# export_pack "debug"   -> exports content pack in debug mode
export_pack() {
  echo "Export content pack"

  mode=$2

  if [[ -z $mode ]] ; then
    mode="debug"
  fi

  #godot_editor="$(pwd)/${FOLDER_GODOT_EDITOR}/${FILE_EXE}"
  log_export="$(pwd)/${FOLDER_TEMP}/${FILE_EXPORT_LOG}"

  # Export Godot project
  rm -f "${log_export}"

  if [ "$mode" = "debug" ]; then
    echo "Export resource packs with debug settings"
    ${GODOT_EDITOR} \
      --headless \
      --quit \
      --path "$(pwd)/${FOLDER_PROJECT}/" \
      --export-pack "${PRESET_DEBUG}" \
      "$(pwd)/$FOLDER_OUTPUT/${FILE_OUTPUT}.pck" \
      &> "${log_export}"
  else
    echo "Export resource packs with release settings"
    ${GODOT_EDITOR} \
      --headless \
      --quit \
      --verbose \
      --path "$(pwd)/${FOLDER_PROJECT}/" \
      --export-pack "${PRESET_RELEASE}" \
      "$(pwd)/$FOLDER_OUTPUT/${FILE_OUTPUT}.pck" \
      &> "${log_export}"
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

#######################################################

export_type=$1
export_mode=$2

if [[ -z $export_mode ]] ; then
  export_mode="debug"
fi
if [[ -z $export_type ]] ; then
  export_type="project"
fi

rm_dir "${FOLDER_OUTPUT}"
make_dir "${FOLDER_TEMP}"

download_editor
download_export_templates

export_project "${export_mode}"

#if [ "$export_type" = "project" ]; then
#  export_project "${export_mode}"
#else
#  export_pack "${export_mode}"
#fi
