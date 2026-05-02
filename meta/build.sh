#!/usr/bin/env bash
set -ex

cmake --build build -j`nproc`
