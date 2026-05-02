#!/usr/bin/env bash
set -ex

./meta/build.sh
ctest --test-dir build --output-on-failure $@
