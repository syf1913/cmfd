#!/usr/bin/env bash

set -eu
# Enable pipefail where supported (bash/zsh/ksh). dash/sh may not support it.
(set -o pipefail) >/dev/null 2>&1 && set -o pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
THIRD_PART_DIR="${ROOT_DIR}/third-part"
TMP_DIR="${ROOT_DIR}/.tmp-third-part"

JSON_VERSION="v3.11.2"
SPDLOG_VERSION="v1.14.1"
XTL_VERSION="0.8.0"
XTENSOR_VERSION="0.26.0"
SPDLOG_VERSION_NUM="${SPDLOG_VERSION#v}"
JSON_DIR_NAME="json-${JSON_VERSION}"
SPDLOG_DIR_NAME="spdlog-${SPDLOG_VERSION_NUM}"
XTL_DIR_NAME="xtl-${XTL_VERSION}"
XTENSOR_DIR_NAME="xtensor-${XTENSOR_VERSION}"

mkdir -p "${THIRD_PART_DIR}" "${TMP_DIR}"
rm -rf \
  "${THIRD_PART_DIR}/json" "${THIRD_PART_DIR}/spdlog" "${THIRD_PART_DIR}/boost" "${THIRD_PART_DIR}/xtl" "${THIRD_PART_DIR}/xtensor" \
  "${THIRD_PART_DIR}/${JSON_DIR_NAME}" "${THIRD_PART_DIR}/${SPDLOG_DIR_NAME}" "${THIRD_PART_DIR}/${XTL_DIR_NAME}" "${THIRD_PART_DIR}/${XTENSOR_DIR_NAME}"
mkdir -p \
  "${THIRD_PART_DIR}/${JSON_DIR_NAME}/include/nlohmann" \
  "${THIRD_PART_DIR}/${SPDLOG_DIR_NAME}" \
  "${THIRD_PART_DIR}/${XTL_DIR_NAME}" \
  "${THIRD_PART_DIR}/${XTENSOR_DIR_NAME}"

echo "[1/4] Fetch nlohmann/json single header (${JSON_VERSION})"
curl -fL "https://raw.githubusercontent.com/nlohmann/json/${JSON_VERSION}/single_include/nlohmann/json.hpp" \
  -o "${THIRD_PART_DIR}/${JSON_DIR_NAME}/include/nlohmann/json.hpp"

echo "[2/4] Fetch spdlog include tree (${SPDLOG_VERSION})"
SPDLOG_ARCHIVE="${TMP_DIR}/spdlog-${SPDLOG_VERSION}.tar.gz"
curl -fL "https://github.com/gabime/spdlog/archive/refs/tags/${SPDLOG_VERSION}.tar.gz" -o "${SPDLOG_ARCHIVE}"
mkdir -p "${THIRD_PART_DIR}/${SPDLOG_DIR_NAME}"
tar -xzf "${SPDLOG_ARCHIVE}" -C "${THIRD_PART_DIR}/${SPDLOG_DIR_NAME}" --strip-components=1 "spdlog-${SPDLOG_VERSION_NUM}/include"

echo "[3/4] Fetch xtl include tree (${XTL_VERSION})"
XTL_ARCHIVE="${TMP_DIR}/xtl-${XTL_VERSION}.tar.gz"
curl -fL "https://github.com/xtensor-stack/xtl/archive/refs/tags/${XTL_VERSION}.tar.gz" -o "${XTL_ARCHIVE}"
tar -xzf "${XTL_ARCHIVE}" -C "${THIRD_PART_DIR}/${XTL_DIR_NAME}" --strip-components=1 "xtl-${XTL_VERSION}/include"

echo "[4/4] Fetch xtensor include tree (${XTENSOR_VERSION})"
XTENSOR_ARCHIVE="${TMP_DIR}/xtensor-${XTENSOR_VERSION}.tar.gz"
curl -fL "https://github.com/xtensor-stack/xtensor/archive/refs/tags/${XTENSOR_VERSION}.tar.gz" -o "${XTENSOR_ARCHIVE}"
tar -xzf "${XTENSOR_ARCHIVE}" -C "${THIRD_PART_DIR}/${XTENSOR_DIR_NAME}" --strip-components=1 "xtensor-${XTENSOR_VERSION}/include"

rm -rf "${TMP_DIR}"

echo "Done. Vendored dependencies are ready in ${THIRD_PART_DIR}."
