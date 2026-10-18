#!/usr/bin/env bash

# **Subtree helper script**
#
# This script reads the `.gitsubtrees` file from the repository's root and can
# perform init/update operations upon it.
#
# The syntax for the `.gitsubtrees` is heavily inspired from `.gitmodules`.
# - blank lines and those starting with `#` are ignored
# - subtree are specified by a group of 3 lines:
#   1. `[<path>]` where `<path>` is the subtree path relative to `.gitsubtrees`
#   2. `repo = <repository>` where `<repository>` is any valid git repository URI
#   3. `sha = <commit>` where `<commit>` can be any valid SHA-1 commit hash
# - Lines 2 and 3 of the subtree spec are interchangable.
#
# **USAGE**
# ./meta/subtrees.sh [-vqh]
#
# Each entry specified in `.gitsubtrees` will be processed and the following rules appied:
# - If the specified directory is missing, add the subtree at the specified revision
# - If the specified directory exists but has a different SHA, update the subtree and squash merge.
# - If the specified directory exists and has the right SHA, skip.
#
# Regardless of the operation being applied, the `.gitsubtrees` file must be modified before any
# changes will occur to a given subtree.
#
# For quiet execution, specify `-q`.

git_root=$(git rev-parse --show-toplevel)
spec_file="$git_root/.gitsubtrees"
verbose=0
quiet=0
git_flags="-q"

# Fatal exit handler
trap "exit 1" SIGTERM
set -e

while getopts 'vqh' opt; do
  case "$opt" in
  v)
    verbose=1
    git_flags=""
    ;;

  q)
    quiet=1
    ;;

  h)
    echo -e "USAGE: ./meta/subtrees.sh [-vqh]\nFor more, read the script's source header"
    exit 0
    ;;
  esac
done

log() {
  if [ $quiet -ne 1 ]; then
    echo -e "$(tput bold)\x1b[36m$@\x1b[36m$(tput sgr0)"
  fi
}

# Expects a line as the first and only argument
# Prints two values in the form `type value` where type is:
# 0: dir
# 1: repo
# 2: sha
# 3: none
parse_line() {
  local -a result
  local line="$1"
  case "${line:0:1}" in
  "#")
    result=(3 "")
    ;;

  "")
    result=(3 "")
    ;;

  "[")
    result=(0 "${line:1:-1}")
    ;;
  esac

  if [ "${line:0:7}" == "repo = " ]; then
    result=(1 "${line:7}")
  elif [ "${line:0:6}" == "sha = " ]; then
    result=(2 "${line:6}")
  fi

  echo -e "${result[0]}\n${result[1]}"
}

# Prints the current sha of the subtree specified at path $1
get_subtree_current_sha() {
  blame_commit=$(git log -n 1 --pretty="format:%H" "$1")

  # Merge commits take the form of: Merge commit '<sha>' as '<subtree dir>'
  merge_commit=$(git log -n 1 --pretty="format:%B" "$blame_commit" | cut -d\' -f2)

  # The upstream commit message takes the form of: git-subtree-split: <sha>
  git log -n 1 --pretty=format:%B "$merge_commit" | grep git-subtree-split | cut -d' ' -f2
}

process_subtree() {
  dir="$1"
  repo="$2"
  sha="$3"

  if [ $verbose -ne 1 ]; then
    exec 3>/dev/null
  else
    exec 3>&1
  fi

  if [ ! -d "$dir" ]; then
    log "Initializing subtree $dir from $repo at commit $sha"
    git subtree add $git_flags --prefix="$dir" "$repo" "$sha" --squash 2>&3 >&3
  else
    current=$(get_subtree_current_sha "$dir")

    if [ "$sha" == "$current" ]; then
      log "Skipping $dir @ $current"
    else
      log "Updating subtree $dir from $repo with $current..$sha"
      git subtree pull $git_flags --prefix="$dir" "$repo" "$sha" --squash 2>&3 >&3
    fi
  fi

  exec 3>&-
}

subtree_dir=""
subtree_repo=""
subtree_sha=""
while read -r line; do
  mapfile -t result < <(parse_line "$line")

  case "${result[0]}" in
  0)
    subtree_dir="${result[1]}"
    ;;

  1)
    subtree_repo="${result[1]}"
    ;;

  2)
    subtree_sha="${result[1]}"
    ;;

  3)
    ;;
  esac

  if [ "${subtree_dir}" != "" ] && [ "${subtree_repo}" != "" ] && [ "${subtree_sha}" != "" ]; then
    process_subtree "$subtree_dir" "$subtree_repo" "$subtree_sha"
    if [ $? -ne 0 ]; then
      kill -s TERM $$
    fi
    subtree_dir=""
    subtree_repo=""
    subtree_sha=""
  fi
done <"$spec_file"
